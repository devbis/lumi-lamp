import asyncio
from dataclasses import dataclass
from typing import Tuple

import serial_asyncio


@dataclass
class State:
    on: bool
    color: Tuple[int, int, int]
    brightness: int


led_r = '/sys/class/leds/red/brightness'
led_g = '/sys/class/leds/green/brightness'
led_b = '/sys/class/leds/blue/brightness'


async def process_state(state: State):
    leds = (
        (led_r, state.color[0]),
        (led_g, state.color[1]),
        (led_b, state.color[2]),
    )
    if not state.on:
        # print('switch off')
        value = 0
        for c, _ in leds:
            with open(c, 'w') as f:
                f.write(f'{value}\n')
    else:
        # print(f'switch to color: {state}')
        for c, value in leds:
            value = int(value * state.brightness/255)
            with open(c, 'w') as f:
                f.write(f'{value}\n')


async def amain():
    state = State(on=False, color=(255, 255, 255), brightness=255)

    reader, writer = await serial_asyncio.open_serial_connection(
        url='/dev/ttymxc1',
        baudrate=115200,
    )
    while True:
        need_change = False
        line = (await reader.readline()).strip()
        if not line:
            continue
        if line.startswith(b'$S:'):
            state.on = line[3] == 0x31

            need_change = True
        elif line.startswith(b'$L:'):
            state.brightness = int(line[3:])
            need_change = True
        elif line.startswith(b'$C:'):
            state.color = tuple(int(x) for x in line[3:].split(b','))
            need_change = True
        if need_change:
            await process_state(state)

if __name__ == '__main__':
    asyncio.run(amain())

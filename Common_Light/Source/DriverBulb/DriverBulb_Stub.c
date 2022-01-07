/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

/* SDK includes */
#include <jendefs.h>
#include "AppHardwareApi.h"
#include "dbg.h"
#include "dbg_uart.h"

///* DK4 includes */
//#include "LightingBoard.h"

/* Device includes */
#include "DriverBulb.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifndef DEBUG_DRIVER
#define TRACE_DRIVER FALSE
#else
#define TRACE_DRIVER TRUE
#endif

#define ADC_FULL_SCALE   1023
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Global Variables                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE bool_t bBulbOn = FALSE;
#ifdef CCT
PRIVATE	uint32 au32Mireds2RGB[17][4] =
{
/* Mireds,   Red, Green,  Blue */
	{   0,    50,   117,   255},
	{  63,    88,   153,   255},
	{ 127,   158,   208,   255},
	{ 191,   253,   255,   221},
	{ 255,   255,   202,   124},
	{ 319,   255,   163,    70},
	{ 383,   255,   135,    41},
	{ 447,   255,   112,    24},
	{ 511,   255,    95,    14},
	{ 575,   255,    80,     9},
	{ 639,   255,    70,     5},
	{ 703,   255,    60,     3},
	{ 767,   255,    52,     2},
	{ 831,   255,    45,     1},
	{ 895,   255,    40,     1},
	{ 959,   255,    35,     0},
	{1023,   255,    32,     0}
};
#endif
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void DriverBulb_vInit(void)
{

}

PUBLIC void DriverBulb_vOn(void)
{
	DriverBulb_vSetOnOff(TRUE);
}

PUBLIC void DriverBulb_vOff(void)
{
	DriverBulb_vSetOnOff(FALSE);
}

PUBLIC void DriverBulb_vSetOnOff(bool_t bOn)
{
     bBulbOn =  bOn;
     DBG_vPrintf(TRUE, "\n$S:%d\n",(bOn ? 1 : 0));
}

PUBLIC void DriverBulb_vSetLevel(uint32 u32Level)
{
	DBG_vPrintf(TRUE, "\n$L:%d\n",u32Level);
}

PUBLIC void DriverBulb_vSetColour(uint32 u32Red, uint32 u32Green, uint32 u32Blue)
{
	DBG_vPrintf(TRUE, "\n$C:%d,%d,%d\n",u32Red, u32Green, u32Blue);
#ifdef RGB
	bRGB_LED_SetLevel(u32Red,u32Green,u32Blue);
#endif
}

PUBLIC bool_t DriverBulb_bOn(void)
{
	return (bBulbOn);
}

PUBLIC bool_t DriverBulb_bReady(void)
{
	return (TRUE);
}

PUBLIC bool_t DriverBulb_bFailed(void)
{
	return (FALSE);
}

PUBLIC void DriverBulb_vTick(void)
{
/* No timing behaviour needed in DR1175 */
}

PUBLIC int16 DriverBulb_i16Analogue(uint8 u8Adc, uint16 u16AdcRead)
{
	if (u8Adc == E_AHI_ADC_SRC_VOLT)
	{
		return ((u16AdcRead * 3600)/ADC_FULL_SCALE);
	}
	else
	{
		return(ADC_FULL_SCALE);
	}
}

/* This function replicates the 'real bulb' set colour temperature function called */
/* on DR1221 drivers and allows the DR1175 to look like a CCT=TW bulb */
PUBLIC void DriverBulb_vSetTunableWhiteColourTemperature(int32 i32ColourTemperature)
{
//#ifdef CCT
//	uint16 u16Mireds;
//
//	/* Convert passed in temperature to mireds (passed in as kelvins) */
//	u16Mireds = (uint16) (1000000/i32ColourTemperature);
//
//	/* Value in range ? */
//	if (u16Mireds <= 1023)
//	{
//		uint8   u8Loop;
//		uint32 u32RangeM = 0;
//		uint32 u32DiffM;
//		uint32 u32RangeRGB;
//		uint32 u32DiffRGB;
//
//		/* Start with all values set to full */
//		uint8 u8Red   = 255;
//		uint8 u8Green = 255;
//		uint8 u8Blue  = 255;
//
//		/* Loop through table until we find entry above current temperature */
//		for (u8Loop = 1; u8Loop < 17 && u32RangeM == 0; u8Loop++)
//		{
//			/* Is this the value above the one we are looking for ? */
//			if (au32Mireds2RGB[u8Loop][0] > u16Mireds)
//			{
//				/* Calculate range and difference in mireds */
//				u32RangeM = au32Mireds2RGB[u8Loop][0] - au32Mireds2RGB[u8Loop-1][0];
//				u32DiffM  = u16Mireds                 - au32Mireds2RGB[u8Loop-1][0];
//				/* Calculate red */
//				u32RangeRGB = au32Mireds2RGB[u8Loop][1] - au32Mireds2RGB[u8Loop-1][1];
//				u32DiffRGB  = (u32DiffM * u32RangeRGB) / u32RangeM;
//				u8Red   = au32Mireds2RGB[u8Loop-1][1] + u32DiffRGB;
//				/* Calculate green */
//				u32RangeRGB = au32Mireds2RGB[u8Loop][2] - au32Mireds2RGB[u8Loop-1][2];
//				u32DiffRGB  = (u32DiffM * u32RangeRGB) / u32RangeM;
//				u8Green = au32Mireds2RGB[u8Loop-1][2] + u32DiffRGB;
//				/* Calculate blue */
//				u32RangeRGB = au32Mireds2RGB[u8Loop][3] - au32Mireds2RGB[u8Loop-1][3];
//				u32DiffRGB  = (u32DiffM * u32RangeRGB) / u32RangeM;
//				u8Blue = au32Mireds2RGB[u8Loop-1][3] + u32DiffRGB;
//			}
//		}
//
//		bRGB_LED_SetLevel(u8Red, u8Green, u8Blue);
//	}
//#endif
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

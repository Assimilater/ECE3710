#ifndef __TFTM032_H__
#define __TFTM032_H__

//---------------------------------------------------------------------------------------+
// Inlucded dependencies                                                                 |
//---------------------------------------------------------------------------------------+
#include "../Shared/Controller.h"

//---------------------------------------------------------------------------------------+
// Command Codes For The LCD                                                             |
//---------------------------------------------------------------------------------------+
#include "LCD_Codes.h"
extern code LCD_CODE_PWRA[SIZE_CODE_PWRA];
extern code LCD_CODE_PWRB[SIZE_CODE_PWRB];
extern code LCD_CODE_DTCA[SIZE_CODE_DTCA];

//---------------------------------------------------------------------------------------+
// Register definintions specific to the LCD                                             |
//---------------------------------------------------------------------------------------+
#define LCD_CSX BAND_GPIO_PD2
#define LCD_DCX BAND_GPIO_PD3
#define LCD_WRX BAND_GPIO_PD6
#define LCD_RDX BAND_GPIO_PD7


//---------------------------------------------------------------------------------------+
// Driver Functions                                                                      |
//---------------------------------------------------------------------------------------+





#endif

#ifndef __TFTM032_H__
#define __TFTM032_H__

//---------------------------------------------------------------------------------------+
// Inlucded dependencies                                                                 |
//---------------------------------------------------------------------------------------+
#include "../Shared/Controller.h"
#include "LCD_Codes.h"

//---------------------------------------------------------------------------------------+
// Color Codes For The LCD                                                               |
//---------------------------------------------------------------------------------------+
extern code LCD_COLOR_WHITE[SIZE_COLOR];
extern code LCD_COLOR_BLACK[SIZE_COLOR];
extern code LCD_COLOR_GREY[SIZE_COLOR];
extern code LCD_COLOR_BLUE[SIZE_COLOR];
extern code LCD_COLOR_RED[SIZE_COLOR];
extern code LCD_COLOR_MAGENTA[SIZE_COLOR];
extern code LCD_COLOR_GREEN[SIZE_COLOR];
extern code LCD_COLOR_CYAN[SIZE_COLOR];
extern code LCD_COLOR_YELLOW[SIZE_COLOR];

//---------------------------------------------------------------------------------------+
// Command Codes For The LCD                                                             |
//---------------------------------------------------------------------------------------+
extern code LCD_CODE_PWRA[SIZE_CODE_PWRA];
extern code LCD_CODE_PWRB[SIZE_CODE_PWRB];
extern code LCD_CODE_DTCA[SIZE_CODE_DTCA];
extern code LCD_CODE_DTCB[SIZE_CODE_DTCB];
extern code LCD_CODE_PSQC[SIZE_CODE_PSQC];
extern code LCD_CODE_PMRC[SIZE_CODE_PMRC];
extern code LCD_CODE_PCL1[SIZE_CODE_PCL1];
extern code LCD_CODE_PCL2[SIZE_CODE_PCL2];
extern code LCD_CODE_VCM1[SIZE_CODE_VCM1];
extern code LCD_CODE_VCM2[SIZE_CODE_VCM2];
extern code LCD_CODE_MACL[SIZE_CODE_MACL];
extern code LCD_CODE_PXFS[SIZE_CODE_PXFS];
extern code LCD_CODE_FMCL[SIZE_CODE_FMCL];
extern code LCD_CODE_DFCL[SIZE_CODE_DFCL];
extern code LCD_CODE_3GFD[SIZE_CODE_3GFD];
extern code LCD_CODE_GCSL[SIZE_CODE_GCSL];
extern code LCD_CODE_SGM0[SIZE_CODE_SGM0];
extern code LCD_CODE_SGM1[SIZE_CODE_SGM1];

//---------------------------------------------------------------------------------------+
// Register definintions specific to the LCD                                             |
//---------------------------------------------------------------------------------------+
#define LCD_CSX BAND_GPIO_PD2
#define LCD_DCX BAND_GPIO_PD3
#define LCD_WRX BAND_GPIO_PD6
#define LCD_RDX BAND_GPIO_PD7
#define LCD_RST BAND_GPIO_PA2

//---------------------------------------------------------------------------------------+
// LCD Global Constants                                                                  |
//---------------------------------------------------------------------------------------+
static const short LCD_ROWS = 320;
static const short LCD_COLS = 240;
static const int LCD_AREA = LCD_ROWS * LCD_COLS;//76800;//LCD_ROWS * (int)LCD_COLS;

//---------------------------------------------------------------------------------------+
// Driver Functions                                                                      |
//---------------------------------------------------------------------------------------+
void LCD_WriteCmd(const unsigned char);
void LCD_WriteData(const unsigned char*, const int);
void LCD_WriteBlock(const unsigned char*, const int, const int);

void LCD_SetColumn(const unsigned short, const unsigned short);
void LCD_SetPage(const unsigned short, const unsigned short);

void LCD_Init(void);

#endif

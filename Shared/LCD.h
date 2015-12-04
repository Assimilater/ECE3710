#ifndef __TFTM032_H__
#define __TFTM032_H__

//---------------------------------------------------------------------------------------+
// Inlucded dependencies                                                                 |
//---------------------------------------------------------------------------------------+
#include "LCD_Codes.h"
#include "../Shared/embedded_t.h"
#include "../Shared/fonts.h"

//---------------------------------------------------------------------------------------+
// Color Codes For The LCD                                                               |
//---------------------------------------------------------------------------------------+
extern const byte LCD_COLOR_WHITE[SIZE_COLOR];
extern const byte LCD_COLOR_BLACK[SIZE_COLOR];
extern const byte LCD_COLOR_GREY[SIZE_COLOR];
extern const byte LCD_COLOR_BLUE[SIZE_COLOR];
extern const byte LCD_COLOR_RED[SIZE_COLOR];
extern const byte LCD_COLOR_MAGENTA[SIZE_COLOR];
extern const byte LCD_COLOR_GREEN[SIZE_COLOR];
extern const byte LCD_COLOR_CYAN[SIZE_COLOR];
extern const byte LCD_COLOR_YELLOW[SIZE_COLOR];

//---------------------------------------------------------------------------------------+
// Command Codes For The LCD                                                             |
//---------------------------------------------------------------------------------------+
extern const byte LCD_CODE_PWRA[SIZE_CODE_PWRA];
extern const byte LCD_CODE_PWRB[SIZE_CODE_PWRB];
extern const byte LCD_CODE_DTCA[SIZE_CODE_DTCA];
extern const byte LCD_CODE_DTCB[SIZE_CODE_DTCB];
extern const byte LCD_CODE_PSQC[SIZE_CODE_PSQC];
extern const byte LCD_CODE_PMRC[SIZE_CODE_PMRC];
extern const byte LCD_CODE_PCL1[SIZE_CODE_PCL1];
extern const byte LCD_CODE_PCL2[SIZE_CODE_PCL2];
extern const byte LCD_CODE_VCM1[SIZE_CODE_VCM1];
extern const byte LCD_CODE_VCM2[SIZE_CODE_VCM2];
extern const byte LCD_CODE_MACL[SIZE_CODE_MACL];
extern const byte LCD_CODE_PXFS[SIZE_CODE_PXFS];
extern const byte LCD_CODE_FMCL[SIZE_CODE_FMCL];
extern const byte LCD_CODE_DFCL[SIZE_CODE_DFCL];
extern const byte LCD_CODE_3GFD[SIZE_CODE_3GFD];
extern const byte LCD_CODE_GCSL[SIZE_CODE_GCSL];
extern const byte LCD_CODE_SGM0[SIZE_CODE_SGM0];
extern const byte LCD_CODE_SGM1[SIZE_CODE_SGM1];

//---------------------------------------------------------------------------------------+
// Register definitions specific to the LCD                                             |
//---------------------------------------------------------------------------------------+
#define LCD_CSX BAND_GPIO_PD2
#define LCD_DCX BAND_GPIO_PD3
#define LCD_WRX BAND_GPIO_PD6
#define LCD_RDX BAND_GPIO_PD7
#define LCD_RST BAND_GPIO_PE2
#define TP_CSX BAND_GPIO_PA7

//---------------------------------------------------------------------------------------+
// LCD Global Constants                                                                  |
//---------------------------------------------------------------------------------------+
static const unsigned short LCD_COLS = 240;
static const unsigned short LCD_ROWS = 320;
static const unsigned int LCD_AREA = LCD_COLS * LCD_ROWS;

typedef struct {
	unsigned short ColumnStart, ColumnEnd, PageStart, PageEnd;
	const byte* Color;
} Region;

typedef struct {
	unsigned short x, y;
	const font* Font;
	const byte* BackColor;
	const byte* Color;
	char* Text;
} TextRegion;

typedef struct {
	unsigned short col, page;
} coord;

typedef enum {
	TOUCH_RESET,
	TOUCH_POLL,
	TOUCH_GET,
} SAMPLE_MODE;

//---------------------------------------------------------------------------------------+
// Driver Functions                                                                      |
//---------------------------------------------------------------------------------------+
bool LCD_GetXY(SAMPLE_MODE, coord*);
void LCD_WaitChip(void);
void LCD_WriteCmd(const byte);
void LCD_WriteData(const byte*, const int);
void LCD_WriteBlock(const byte*, const int, const int);
void LCD_WriteText(const TextRegion);

void LCD_SetColumn(const unsigned short, const unsigned short);
void LCD_SetPage(const unsigned short, const unsigned short);
void LCD_FillRegion(const Region);

void LCD_Init(void);

#endif

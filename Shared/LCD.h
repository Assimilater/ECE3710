#ifndef __TFTM032_H__
#define __TFTM032_H__
#include "fonts.h"

//---------------------------------------------------------------------------------------+
// Color Codes For The LCD                                                               |
//---------------------------------------------------------------------------------------+
#define SIZE_COLOR              2
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
// Communication Signals for the LCD                                                     |
//---------------------------------------------------------------------------------------+
#define LCD_CSX BAND_GPIO_PD2
#define LCD_DCX BAND_GPIO_PD3
#define LCD_WRX BAND_GPIO_PD6
#define LCD_RDX BAND_GPIO_PD7

#define LCD_RST BAND_GPIO_PA7
#define TP_CSX BAND_GPIO_PE0

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

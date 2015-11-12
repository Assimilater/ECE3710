#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "../Shared/bool.h"
#include "LCD.h"

//---------------------------------------------------------------------------------------+
// Precalculated constants for the dimensions of our boxes                               |
//---------------------------------------------------------------------------------------+
const short LENGTH_INNER = 70;
const short LENGTH_OUTER = 80;

const short AREA_OUTER = LENGTH_OUTER * LENGTH_OUTER;
const short AREA_INNER = LENGTH_INNER * LENGTH_INNER;

const short PADC_OUTER = (LCD_COLS - LENGTH_OUTER) / 2;
const short PADC_INNER = (LCD_COLS - LENGTH_INNER) / 2;

const short PADR_OUTER = (LCD_ROWS - (3 * LENGTH_OUTER)) / 4;
const short PADR_INNER = (LENGTH_OUTER - LENGTH_INNER) / 2;

const short COL_OUTER_Y0 = PADC_OUTER;
const short COL_INNER_Y0 = PADC_INNER;

const short COL_OUTER_YF = COL_OUTER_Y0 + LENGTH_OUTER;
const short COL_INNER_YF = COL_INNER_Y0 + LENGTH_INNER;

const short ROW_OUTER_1Y0 = PADR_OUTER;
const short ROW_INNER_1Y0 = ROW_OUTER_1Y0 + PADR_INNER;
const short ROW_OUTER_1YF = PADR_OUTER + LENGTH_OUTER;
const short ROW_INNER_1YF = ROW_OUTER_1YF - PADR_INNER;

const short ROW_OUTER_2Y0 = PADR_OUTER * 2 + LENGTH_OUTER;
const short ROW_INNER_2Y0 = ROW_OUTER_2Y0 + PADR_INNER;
const short ROW_OUTER_2YF = PADR_OUTER * 2 + LENGTH_OUTER * 2;
const short ROW_INNER_2YF = ROW_OUTER_2YF - PADR_INNER;

const short ROW_OUTER_3Y0 = PADR_OUTER * 3 + LENGTH_OUTER * 2;
const short ROW_INNER_3Y0 = ROW_OUTER_3Y0 + PADR_INNER;
const short ROW_OUTER_3YF = PADR_OUTER * 3 + LENGTH_OUTER * 3;
const short ROW_INNER_3YF = ROW_OUTER_3YF - PADR_INNER;

//---------------------------------------------------------------------------------------+
// Any program logic                                                                     |
//---------------------------------------------------------------------------------------+

void fillRed() {
	const static Region r = {
		COL_INNER_Y0,
		COL_INNER_YF,
		ROW_INNER_1Y0,
		ROW_INNER_1YF,
		LCD_COLOR_RED
	};
	LCD_FillRegion(r);
	GPIOA->DATA &= ~0x8;
}

void unfillRed() {
	const static Region r = {
		COL_INNER_Y0,
		COL_INNER_YF,
		ROW_INNER_1Y0,
		ROW_INNER_1YF,
		LCD_COLOR_BLACK
	};
	LCD_FillRegion(r);
	GPIOA->DATA |= 0x8;
}

void fillGreen() {
	const static Region r = {
		COL_INNER_Y0,
		COL_INNER_YF,
		ROW_INNER_2Y0,
		ROW_INNER_2YF,
		LCD_COLOR_GREEN
	};
	LCD_FillRegion(r);
	GPIOA->DATA &= ~0x10;
}

void unfillGreen() {
	const static Region r = {
		COL_INNER_Y0,
		COL_INNER_YF,
		ROW_INNER_2Y0,
		ROW_INNER_2YF,
		LCD_COLOR_BLACK
	};
	LCD_FillRegion(r);
	GPIOA->DATA |= 0x10;
}

void fillYellow() {
	const static Region r = {
		COL_INNER_Y0,
		COL_INNER_YF,
		ROW_INNER_3Y0,
		ROW_INNER_3YF,
		LCD_COLOR_YELLOW
	};
	LCD_FillRegion(r);
	GPIOA->DATA &= ~0x20;
}

void unfillYellow() {
	const static Region r = {
		COL_INNER_Y0,
		COL_INNER_YF,
		ROW_INNER_3Y0,
		ROW_INNER_3YF,
		LCD_COLOR_BLACK
	};
	LCD_FillRegion(r);
	GPIOA->DATA |= 0x20;
}

void exec() {
	unsigned int i;
	Region r;
	r.ColumnStart = COL_OUTER_Y0;
	r.ColumnEnd = COL_OUTER_YF;
	
	// Fill in the outer boxes
	r.PageStart = ROW_OUTER_1Y0;
	r.PageEnd = ROW_OUTER_1YF;
	r.Color = LCD_COLOR_RED;
	LCD_FillRegion(r);
	
	r.PageStart = ROW_OUTER_2Y0;
	r.PageEnd = ROW_OUTER_2YF;
	r.Color = LCD_COLOR_GREEN;
	LCD_FillRegion(r);
	
	r.PageStart = ROW_OUTER_3Y0;
	r.PageEnd = ROW_OUTER_3YF;
	r.Color = LCD_COLOR_YELLOW;
	LCD_FillRegion(r);
	
	// Fill in the inner boxes with black
	unfillRed();
	unfillGreen();
	unfillYellow();
	
	while (0) {
		fillRed();
		for(i = 0; i < 2483648; ++i){};
		unfillRed();
		for(i = 0; i < 2483648; ++i){};
		fillGreen();
		for(i = 0; i < 2483648; ++i){};
		unfillGreen();
		for(i = 0; i < 2483648; ++i){};
		fillYellow();
		for(i = 0; i < 2483648; ++i){};
		unfillYellow();
		for(i = 0; i < 2483648; ++i){};
	}
}

//---------------------------------------------------------------------------------------+
// Any configuration on the microcontroller                                              |
//---------------------------------------------------------------------------------------+
void init() {
	//REG* RCGC2 = (REG*)SYSCTL->RCGC2;
	//RCGC2->bit0 = 1; // Enable port A
	//RCGC2->bit1 = 1; // Enable port B
	//RCGC2->bit3 = 1; // Enable port D
	SYSCTL->RCGC2 = 0xB;
	GPIO.PortD->LOCK.word = GPIO_UNLOCK;
	
	GPIO.PortA->DEN.byte[0] = 0x3C;
	GPIO.PortA->DIR.byte[0] = 0x3C;
	
	GPIO.PortB->DEN.byte[0] = 0xFF;
	GPIO.PortB->DIR.byte[0] = 0xFF;
	
	GPIO.PortD->CR.byte[0] = 0xFF;
	GPIO.PortD->DEN.byte[0] = 0xFF;
	GPIO.PortD->DIR.byte[0] = 0xFF;
	
	LCD_Init();
}

//---------------------------------------------------------------------------------------+
// No program logic should be contained here                                             |
//---------------------------------------------------------------------------------------+
int main() {
	init();
	exec();
	return 0;
}

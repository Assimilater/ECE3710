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
	LCD_SetColumn(COL_OUTER_Y0, COL_OUTER_YF);
	LCD_SetPage(ROW_OUTER_1Y0, ROW_OUTER_1YF);
	LCD_WriteBlock(LCD_COLOR_RED, SIZE_COLOR, AREA_OUTER);
	GPIOA->DATA |= 0x8;
}

void unfillRed() {
	LCD_SetColumn(COL_INNER_Y0, COL_INNER_YF);
	LCD_SetPage(ROW_INNER_1Y0, ROW_INNER_1YF);
	LCD_WriteBlock(LCD_COLOR_BLACK, SIZE_COLOR, AREA_INNER);
	GPIOA->DATA |= !(!GPIOA->DATA | 0x8);
}

void fillGreen() {
	LCD_SetColumn(COL_OUTER_Y0, COL_OUTER_YF);
	LCD_SetPage(ROW_OUTER_2Y0, ROW_OUTER_2YF);
	LCD_WriteBlock(LCD_COLOR_GREEN, SIZE_COLOR, AREA_OUTER);
	GPIOA->DATA |= 0x10;
}

void unfillGreen() {
	LCD_SetColumn(COL_INNER_Y0, COL_INNER_YF);
	LCD_SetPage(ROW_INNER_2Y0, ROW_INNER_2YF);
	LCD_WriteBlock(LCD_COLOR_BLACK, SIZE_COLOR, AREA_INNER);
	GPIOA->DATA |= !(!GPIOA->DATA | 0x10);
}

void fillYellow() {
	LCD_SetColumn(COL_OUTER_Y0, COL_OUTER_YF);
	LCD_SetPage(ROW_OUTER_3Y0, ROW_OUTER_3YF);
	LCD_WriteBlock(LCD_COLOR_YELLOW, SIZE_COLOR, AREA_OUTER);
	GPIOA->DATA |= 0x20;
}

void unfillYellow() {
	LCD_SetColumn(COL_INNER_Y0, COL_INNER_YF);
	LCD_SetPage(ROW_INNER_3Y0, ROW_INNER_3YF);
	LCD_WriteBlock(LCD_COLOR_BLACK, SIZE_COLOR, AREA_INNER);
	GPIOA->DATA |= !(!GPIOA->DATA | 0x20);
}

void exec() {
	// Fill in the outer boxes
	LCD_SetColumn(COL_OUTER_Y0, COL_OUTER_YF);
	
	LCD_SetPage(ROW_OUTER_1Y0, ROW_OUTER_1YF);
	LCD_WriteBlock(LCD_COLOR_RED, SIZE_COLOR, AREA_OUTER);
	
	LCD_SetPage(ROW_OUTER_2Y0, ROW_OUTER_2YF);
	LCD_WriteBlock(LCD_COLOR_GREEN, SIZE_COLOR, AREA_OUTER);
	
	LCD_SetPage(ROW_OUTER_3Y0, ROW_OUTER_3YF);
	LCD_WriteBlock(LCD_COLOR_YELLOW, SIZE_COLOR, AREA_OUTER);
	
	// Fill in the inner boxes
	LCD_SetColumn(COL_INNER_Y0, COL_INNER_YF);
	
	LCD_SetPage(ROW_INNER_1Y0, ROW_INNER_1YF);
	LCD_WriteBlock(LCD_COLOR_BLACK, SIZE_COLOR, AREA_INNER);
	
	LCD_SetPage(ROW_INNER_2Y0, ROW_INNER_2YF);
	LCD_WriteBlock(LCD_COLOR_BLACK, SIZE_COLOR, AREA_INNER);
	
	LCD_SetPage(ROW_INNER_3Y0, ROW_INNER_3YF);
	LCD_WriteBlock(LCD_COLOR_BLACK, SIZE_COLOR, AREA_INNER);
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
	
	GPIO.PortA->DEN.bit2 = 1;
	GPIO.PortA->DIR.bit2 = 1;
	
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

	fillRed();
	for(int i = 0; i < 100000; ++i){};
	unfillRed();
	for(int i = 0; i < 100000; ++i){};
	fillYellow();
	for(int i = 0; i < 100000; ++i){};
	unfillYellow();
	for(int i = 0; i < 100000; ++i){};
	fillGreen();
	for(int i = 0; i < 100000; ++i){};
	unfillGreen();
	for(int i = 0; i < 100000; ++i){};
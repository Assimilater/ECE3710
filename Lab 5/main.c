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
// Helper functions to easily manage the boxes after initialization                      |
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
	GPIO.PortE->DATA.bit3 = 0; // PA &= ~0x8;
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
	GPIO.PortE->DATA.bit3 = 1;
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
	GPIO.PortE->DATA.bit4 = 0; // PA &= ~0x10
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
	GPIO.PortE->DATA.bit4 = 1;
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
	GPIO.PortE->DATA.bit5 = 0; // PA &= ~0x20
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
	GPIO.PortE->DATA.bit5 = 1;
}

//---------------------------------------------------------------------------------------+
// Touchscreen SPI handlers                                                              |
//---------------------------------------------------------------------------------------+
unsigned int sample_cnt = 0;
void GPIOA_Handler() {
	// User is pressing down
	GPIO.PortA->ICR.bit6 = 1;
	sample_cnt = 0;
	
	// Disable GPIOA interrupts
	GPIO.PortA->IM.bit6 = 0;
	
	// Enable SysTick interrupts
	NVIC_ST_CTRL_R |= 0x3;
}

void SysTick_Handler() {
	static const int SIZE = 10;
	static coord data[SIZE];
	
	if (GPIO.PortA->DATA.bit6) {
		// User let go
		
		// Do the fill/unfill operation
		
		// Disable Systick interrups
		NVIC_ST_CTRL_R &= ~0x3;
		
		// Enable GPIOA interrupts
		GPIO.PortA->ICR.bit6 = 1;
		GPIO.PortA->IM.bit6 = 1;
	} else {
		// User is pressing down
		data[sample_cnt % SIZE] = LCD_GetXY(); // Get value from SPI?
	}
}

void flash() {
	unsigned int i;
	while (1) {
		fillRed();
		for(i = 0; i < 2483648; ++i);
		
		unfillRed();
		for(i = 0; i < 2483648; ++i);
		
		fillGreen();
		for(i = 0; i < 2483648; ++i);
		
		unfillGreen();
		for(i = 0; i < 2483648; ++i);
		
		fillYellow();
		for(i = 0; i < 2483648; ++i);
		
		unfillYellow();
		for(i = 0; i < 2483648; ++i);
	}
}

//---------------------------------------------------------------------------------------+
// Program initialization logic                                                          |
//---------------------------------------------------------------------------------------+
void exec() {
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
	//flash();
}

//---------------------------------------------------------------------------------------+
// Any configuration on the microcontroller                                              |
//---------------------------------------------------------------------------------------+
void init() {
	SYSCTL->RCGCGPIO = 0x1B;
	SYSCTL->RCGCSSI = 0x1;
	GPIO.PortD->LOCK.word = GPIO_UNLOCK;
	
	GPIO.PortA->DEN.byte[0] = 0xFC;
	GPIO.PortA->AFSEL.byte[0] = 0x3C;
	
	//GPIO.PortA->DIR.byte[0] = 0xFF;
	//GPIO.PortA->DATA.bit3 = 0;
	//GPIO.PortA->DATA.bit3 = 1;
	//return;
	
	GPIO.PortB->DEN.byte[0] = 0xFF;
	GPIO.PortB->DIR.byte[0] = 0xFF;
	
	GPIO.PortD->CR.byte[0] = 0xFF;
	GPIO.PortD->DEN.byte[0] = 0xFF;
	GPIO.PortD->DIR.byte[0] = 0xFF;
	
	GPIO.PortE->DEN.byte[0] = 0xFF;
	GPIO.PortE->DIR.byte[0] = 0xFF;
	
	// Configure Systick
	NVIC_ST_RELOAD_R = 16000; // 1ms
	
	// Configure SSI
	SSI0->CR1 = 0;
	SSI0->CC = 0x5;
	SSI0->CPSR = 0x2;
	SSI0->CR0 = 0x707;
	SSI0->CR1 |= 0x2;
	
	// Configure interrupts
	GPIO.PortA->IM.word = 0;
	GPIO.PortA->IS.word = 0;
	GPIO.PortA->IBE.bit6 = 0;
	GPIO.PortA->IEV.bit6 = 0;
	GPIO.PortA->ICR.bit6 = 1;
	GPIO.PortA->IM.bit6 = 1;
	
	NVIC_EN0_R = 0x1;
	
	LCD_Init();
}

//---------------------------------------------------------------------------------------+
// No program logic should be contained here                                             |
//---------------------------------------------------------------------------------------+
int main() {
	unsigned int i;
	init();
	exec();
	while (1) { ++i; }
	return 1;
}

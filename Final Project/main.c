#include "../Shared/bool.h"
#include "../Shared/Controller.h"
#include "../Shared/LCD.h"
#include "enet.h"

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
void toggleRed() {
	static bool status = true;
	status = !status;
	if (status) {
		fillRed();
	} else {
		unfillRed();
	}
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
void toggleGreen() {
	static bool status = true;
	status = !status;
	if (status) {
		fillGreen();
	} else {
		unfillGreen();
	}
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
void toggleYellow() {
	static bool status = true;
	status = !status;
	if (status) {
		fillYellow();
	} else {
		unfillYellow();
	}
}

//---------------------------------------------------------------------------------------+
// Touchscreen SPI handlers                                                              |
//---------------------------------------------------------------------------------------+
void GPIOA_Handler() {
	// The screen is touched, for debouncing just start a timed sampler
	GPIO.PortA->ICR.bit6 = 1; // Clear the interrupt
	GPIO.PortA->IM.bit6 = 0; // Disable GPIOA interrupts
	SysTick->CTRL = 0x3; // Enable SysTick interrupts
}

void GPIOE_Handler() {
	if(GPIO.PortE->RIS.bit0)
	{
		//Read from ISP
		
		//Parse Data?? Probably not
		
		//Write to PC
		
		GPIO.PortE->ICR.bit0 = 1;
	}
	if(GPIO.PortE->RIS.bit1)
	{
		//Read from PC
		
		//Parse Data
		
		//write to ISP
		
		GPIO.PortE->ICR.bit1 = 1;
	}
}

void SysTick_Handler() {
	static coord data;
	if (!GPIO.PortA->DATA.bit6) { // User is pressing down, collect sample
		LCD_GetXY(TOUCH_POLL, &data);
	} else {
		SysTick->CTRL = 0x0; // Disable Systick interrups
		
		if (LCD_GetXY(TOUCH_GET, &data)) { // User let go, get the average from samples
			if ((COL_OUTER_Y0 < data.col) && (data.col < COL_OUTER_YF)) {
				if ((ROW_OUTER_1Y0 < data.page) && (data.page < ROW_OUTER_1YF)) {
					toggleRed();
				} else if ((ROW_OUTER_2Y0 < data.page) && (data.page < ROW_OUTER_2YF)) {
					toggleGreen();
				} else if ((ROW_OUTER_3Y0 < data.page) && (data.page < ROW_OUTER_3YF)) {
					toggleYellow();
				}
			}
		}
		
		// Enable GPIOA interrupts
		GPIO.PortA->ICR.bit6 = 1;
		GPIO.PortA->IM.bit6 = 1;
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
	toggleRed();
	toggleGreen();
	toggleYellow();
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
	GPIO.PortA->DIR.bit7 = 1;
	
	// Pull up configuration necessary to avoid electromagnetic interference between SSI pins
	GPIO.PortA->PUR.bit4 = 1; // Rx
	GPIO.PortA->PUR.bit5 = 1; // Tx
	
	GPIO.PortB->DEN.byte[0] = 0xFF;
	GPIO.PortB->DIR.byte[0] = 0xFF;
	
	GPIO.PortD->CR.byte[0] = 0xFF;
	GPIO.PortD->DEN.byte[0] = 0xFF;
	GPIO.PortD->DIR.byte[0] = 0xFF;
	
	GPIO.PortE->DEN.byte[0] = 0xFF;
	GPIO.PortE->DIR.byte[0] = 0xFF;
	
	// Configure SSI Freescale (SPH = 0, SPO = 0)
	SSI0->CR1 = 0; // Disable
	SSI0->CC = 0x5; // Use PIOsc for the clock
	SSI0->CPSR = 0x2; // Clock divisor = 2 (the minimum, or fastest we can make this divisor)
	SSI0->CR0 = 0x307; // SCR = 3 (clock divisor), SPH = 0, SPO = 0, FRF = 0 (freescale), DSS = 7 (8-bit data)
	SSI0->CR1 |= 0x2; // Enable
	
	// Configure Systick
	SysTick->LOAD = 16000; // 1ms
	NVIC_EN0_R = 0x1;
	
	// Configure interrupts
	GPIO.PortA->IM.word = 0;
	GPIO.PortA->IS.word = 0;
	GPIO.PortA->IBE.bit6 = 0;
	GPIO.PortA->IEV.bit6 = 0;
	GPIO.PortA->ICR.bit6 = 1;
	GPIO.PortA->IM.bit6 = 1;
	
	GPIO.PortE->IM.word = 0;
	GPIO.PortE->IS.word = 0;
	GPIO.PortE->IBE.bit0 = 0;
	GPIO.PortE->IEV.bit0 = 0; //Falling Edge triggers interuppt
	GPIO.PortE->ICR.bit0 = 1;
	GPIO.PortE->IM.bit0 = 1;
	GPIO.PortE->IBE.bit1 = 0;
	GPIO.PortE->IEV.bit1 = 0; //Falling Edge triggers interuppt
	GPIO.PortE->ICR.bit1 = 1;
	GPIO.PortE->IM.bit1 = 1;
	
	LCD_Init();
}

//---------------------------------------------------------------------------------------+
// No program logic should be contained here                                             |
//---------------------------------------------------------------------------------------+
int main() {
	init();
	exec();
	while (1);
}

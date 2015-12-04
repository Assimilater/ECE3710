#include "../Shared/embedded_t.h"
#include "../Shared/Controller.h"
#include "../Shared/LCD.h"
#include "enet.h"

//---------------------------------------------------------------------------------------+
// Precalculated constants for the dimensions of our boxes                               |
//---------------------------------------------------------------------------------------+
const short LENGTH_OUTER = 80;
const short LENGTH_INNER = 70;

const short ROW_OUTER_Y0 = 25;
const short ROW_INNER_Y0 = 30;
const short COL_OUTER_Y0 = 80;
const short COL_INNER_Y0 = 85;

const short ROW_OUTER_YF = ROW_OUTER_Y0 + LENGTH_OUTER;
const short ROW_INNER_YF = ROW_INNER_Y0 + LENGTH_INNER;
const short COL_OUTER_YF = COL_OUTER_Y0 + LENGTH_OUTER;
const short COL_INNER_YF = COL_INNER_Y0 + LENGTH_INNER;

//---------------------------------------------------------------------------------------+
// Status variables                                                                      |
//---------------------------------------------------------------------------------------+
typedef enum { FILTER_DISABLED = 0, FILTER_ENABLED = 1 } State;
State enable = FILTER_DISABLED;

uint block = 0;
char blocked_s[8] = "0000000";
TextRegion blocked_r;
TextRegion status_r;

//---------------------------------------------------------------------------------------+
// Helper functions to easily manage the display after initialization                    |
//---------------------------------------------------------------------------------------+
void toggleStatus() {
	static Region button_r = {
		COL_INNER_Y0, COL_INNER_YF,
		ROW_INNER_Y0, ROW_INNER_YF
	};
	
	if (enable) {
		enable = FILTER_DISABLED;
		button_r.Color = LCD_COLOR_RED;
		status_r.Color = LCD_COLOR_RED;
		status_r.Text = "Status: Disabled";
	} else {
		enable = FILTER_ENABLED;
		button_r.Color = LCD_COLOR_GREEN;
		status_r.Color = LCD_COLOR_GREEN;
		status_r.Text = "Status:  Enabled";
	}
	LCD_FillRegion(button_r);
	LCD_WriteText(status_r);
}

void reportBlock() {
	uint i = 7, t = ++block, mod;
	while (i > 0) {
		mod = t % 10;
		blocked_s[--i] = '0' + mod;
		t /= 10;
	}
	LCD_WriteText(blocked_r);
}

//---------------------------------------------------------------------------------------+
// Ethernet SPI handlers                                                                 |
//---------------------------------------------------------------------------------------+
void GPIOE_Handler() {
	if(GPIO.PortE->RIS.bit0) {
		//Read from ISP
		//NET_READDATA(give ISP CS if possible);
		//Parse Data?? Probably not
		
		//Write to PC
		
		GPIO.PortE->ICR.bit0 = 1;
	}
	if(GPIO.PortE->RIS.bit1) {
		//Read from PC
		//NET_READDATA(give PC CS if possible);
		//Parse Data
		
		//write to ISP
		
		GPIO.PortE->ICR.bit1 = 1;
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

void SysTick_Handler() {
	static coord data;
	SysTick->CTRL = 0x0; // Disable Systick interrups
	if (!GPIO.PortA->DATA.bit6) { // User is pressing down, collect sample
		LCD_GetXY(TOUCH_POLL, &data);
		SysTick->CTRL = 0x3; // Enable SysTick interrupts
	} else {
		if (LCD_GetXY(TOUCH_GET, &data)) { // User let go, get the average from samples
			if ((COL_OUTER_Y0 < data.col) && (data.col < COL_OUTER_YF)) {
				if ((ROW_OUTER_Y0 < data.page) && (data.page < ROW_OUTER_YF)) {
					toggleStatus();
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
	Region button_r = {
		COL_OUTER_Y0, COL_OUTER_YF,
		ROW_OUTER_Y0, ROW_OUTER_YF,
		LCD_COLOR_BLUE
	};
	LCD_FillRegion(button_r); // Fill in the outer box
	
	// Write the static text
	blocked_r.y = 95;
	blocked_r.x = ROW_OUTER_YF + 5;
	blocked_r.Font = &fonts()->Big;
	blocked_r.Text = "Pages Blocked";
	blocked_r.BackColor = LCD_COLOR_BLACK;
	blocked_r.Color = LCD_COLOR_YELLOW;
	LCD_WriteText(blocked_r);
	
	// Setup for the dynamic text
	blocked_r.y = 120;
	blocked_r.x = ROW_OUTER_YF + 25;
	blocked_r.Font = &fonts()->Ubuntu;
	blocked_r.Text = blocked_s;
	
	// Write initial value for dynamic text
	LCD_WriteText(blocked_r);
	
	// Setup status text (also dynamic)
	status_r.x = 25;
	status_r.y = 175;
	status_r.Font = &fonts()->Big;
	status_r.BackColor = LCD_COLOR_BLACK;
	
	// Start the filter as enabled
	toggleStatus();
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
	
	// Port A -> Touchscreen interrupts
	GPIO.PortA->IM.word = 0;
	GPIO.PortA->IS.word = 0;
	
	GPIO.PortA->IBE.bit6 = 0;
	GPIO.PortA->IEV.bit6 = 0; //Falling Edge triggers interuppt
	GPIO.PortA->ICR.bit6 = 1;
	GPIO.PortA->IM.bit6 = 1;
	
	// Port E -> Ethernet interrupts
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
	
	NET_Init();
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

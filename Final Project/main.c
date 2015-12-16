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
char status_e[17] = "Status:  Enabled";
char status_d[17] = "Status: Disabled";
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
		status_r.Text = status_d;
		button_r.Color = LCD_COLOR_RED;
		status_r.Color = LCD_COLOR_RED;
	} else {
		enable = FILTER_ENABLED;
		status_r.Text = status_e;
		button_r.Color = LCD_COLOR_GREEN;
		status_r.Color = LCD_COLOR_GREEN;
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
// Define Interrupt Singals                                                              |
//---------------------------------------------------------------------------------------+
#define INT_TOUCH			BAND_GPIO_PE3
#define INT_NET_SERVER		BAND_GPIO_PE4
#define INT_NET_CLIENT		BAND_GPIO_PE5

//---------------------------------------------------------------------------------------+
// Time based sampling of touchscreen with atomic SPI blocker                            |
//---------------------------------------------------------------------------------------+
volatile bool atomic_touch = false;
void SysTick_Handler() {
	static coord data;
	SysTick->CTRL = 0x0; // Disable Systick interrups
	if (!INT_TOUCH) { // User is pressing down, collect sample
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
		
		// Signal that touch interaction is over
		atomic_touch = false;
	}
}

//---------------------------------------------------------------------------------------+
// Uses a timer to sample; so data is averaged over a longer time and touch is debounced |
// This function is blocking, so as to give priority to user input on the touchscreen    |
// Because of interrupts, this acts like a multi-threaded portion of the application     |
//---------------------------------------------------------------------------------------+
void Touch_Handler() {
	atomic_touch = true;
	SysTick->CTRL = 0x3; // Enable SysTick interrupts
	while (atomic_touch); // Wait for touch interaction to finish
}

byte debug[30];
NET_Frame debug_frame;

void NET_SERVER_Handler() {
	byte Int = NET_GetInterrupt(NET_CHIP_SERVER);
	if (Int & NET_INT_RECV) {
		NET_ClearInterrupt(NET_CHIP_SERVER, NET_INT_RECV);
		NET_READDATA(NET_CHIP_SERVER);
		NET_WRITEDATA(NET_CHIP_CLIENT);
	}
}

void NET_CLIENT_Handler() {
	byte Int = NET_GetInterrupt(NET_CHIP_CLIENT);
	if (Int & NET_INT_RECV) {
		NET_ClearInterrupt(NET_CHIP_CLIENT, NET_INT_RECV);
		NET_READDATA(NET_CHIP_CLIENT);
		NET_WRITEDATA(NET_CHIP_SERVER);
	}
	
	
//	NET_READDATA(NET_CHIP_SERVER);
//	NET_WRITEDATA(NET_CHIP_CLIENT);
	
//	debug_frame.Control.mode = NET_MODE_VAR;
//	debug_frame.Control.reg = NET_REG_SOCKET;
//	debug_frame.Control.socket = 0;
//	debug_frame.Control.write = false;
//	
//	debug_frame.Address = NET_SOCKET_IR;
//	debug_frame.Data = debug;
//	debug_frame.N = 2;
//	
//	NET_SPI(NET_CHIP_CLIENT, &debug_frame);
}

void test() {
	NET_READDATA(NET_CHIP_CLIENT);
	NET_WRITEDATA(NET_CHIP_SERVER);
	
	NET_READDATA(NET_CHIP_CLIENT);
}

//---------------------------------------------------------------------------------------+
// Real interrups create the need for atomic use of SPI, so instead use a busy wait      |
//---------------------------------------------------------------------------------------+
#define ACTIVE_INT 0
void Busy_Interrupts() {
	//test();
	while (1) {
		if (INT_TOUCH == ACTIVE_INT) {
			Touch_Handler();
		}
		if (INT_NET_SERVER == ACTIVE_INT) {
			NET_SERVER_Handler();
		}
		if (INT_NET_CLIENT == ACTIVE_INT) {
			NET_CLIENT_Handler();
		}
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
	
	// Our program is a busy wait on interrupt conditions
	Busy_Interrupts();
}

//---------------------------------------------------------------------------------------+
// Any configuration on the microcontroller                                              |
//---------------------------------------------------------------------------------------+
void init() {
	// Enable clocks
	SYSCTL->RCGCGPIO = 0x3F; // 11 1111 => f, e, d, c, b, a
	SYSCTL->RCGCSSI = 0x3; // SSI0, SSI1
	GPIO.PortD->LOCK.word = GPIO_UNLOCK; // PD7 needs to be unlcoked
	GPIO.PortF->LOCK.word = GPIO_UNLOCK; // PF0 needs to be unlocked
	
	// PA[0:1] => Unavailable
	// PA[2:5] => SPI
	GPIO.PortA->DEN.byte[0] = 0xFC;
	GPIO.PortA->AFSEL.byte[0] = 0x3C;
	GPIO.PortA->DIR.bit6 = 1; // External NET Reset (Shared)
	GPIO.PortA->DIR.bit7 = 1; // External LCD Reset
	
	// Pull up configuration necessary to avoid electromagnetic interference between SSI pins
	//GPIO.PortA->PDR.bit2 = 1; // Clk
	//GPIO.PortA->PDR.bit4 = 1; // Rx
	GPIO.PortA->PDR.bit5 = 1; // Tx
	//GPIO.PortA->ODR.bit5 = 1; // Tx
	
	// PB[0:7] => LCD Data Bus
	GPIO.PortB->DEN.byte[0] = 0xFF;
	GPIO.PortB->DIR.byte[0] = 0xFF;
	
	// PC[0:3] => Unavailable
	// PC[4:5] => NET ready signals
	// PC[6:7] => NC
	GPIO.PortC->DEN.word |= 0xF0;
	GPIO.PortC->DIR.word &= 0x0F;
	
	// PD[0:1] is shared with PB[6:7] (for reasons beyond my comprehension)
	// PD[2:3,6:7] => LCD communication signals
	// PD[4:5] => Unavailable
	GPIO.PortD->CR.byte[0] = 0xFF;
	GPIO.PortD->DEN.byte[0] = 0xFF;
	GPIO.PortD->DIR.byte[0] = 0xFF;
	
	// PE[0:2] => CS Pins (output)
	// PE[3:5] => Interrupt Pins for SPI request (input)
	// PE[6:7] => Nonexistent
	GPIO.PortE->DEN.byte[0] = 0xFF;
	GPIO.PortE->DIR.byte[0] = 0x07;
	
	// SPI CS default state high
	GPIO.PortE->DATA.bit0 = 1;
	GPIO.PortE->DATA.bit1 = 1;
	GPIO.PortE->DATA.bit2 = 1;
	
	// PA[0:1] => Unavailable
	// PA[2:5] => SPI
	GPIO.PortF->CR.byte[0] = 0xF;
	GPIO.PortF->DEN.byte[0] = 0xF;
	GPIO.PortF->AFSEL.byte[0] = 0xF;
	GPIO.PortF->PCTL.half[0] = 0x2222;
	GPIO.PortF->PDR.bit1 = 1; // Tx
	
	// Configure SSI Freescale (SPH = 0, SPO = 0)
	SSI0->CR1 = 0; // Disable
	SSI0->CC = 0x5; // Use PIOsc for the clock
	SSI0->CPSR = 0x2; // Clock divisor = 2 (the minimum, or fastest we can make this divisor)
	SSI0->CR0 = 0x307; // SCR = 3 (divisor), SPH = 0, SPO = 0, FRF = 0 (freescale), DSS = 7 (8-bit data)
	SSI0->CR1 |= 0x2; // Enable
	
	// Configure SSI Freescale (SPH = 0, SPO = 0)
	SSI1->CR1 = 0; // Disable
	SSI1->CC = 0x5; // Use PIOsc for the clock
	SSI1->CPSR = 0x2; // Clock divisor = 2 (the minimum, or fastest we can make this divisor)
	SSI1->CR0 = 0x307; // SCR = 3 (divisor), SPH = 0, SPO = 0, FRF = 0 (freescale), DSS = 7 (8-bit data)
	SSI1->CR1 |= 0x2; // Enable
	
	// Configure Systick
	SysTick->LOAD = 16000; // 1ms
	
	LCD_Init();
	NET_Init();
}

//---------------------------------------------------------------------------------------+
// No program logic should be contained here                                             |
//---------------------------------------------------------------------------------------+
int main() {
	init();
	exec();
	while (1);
}

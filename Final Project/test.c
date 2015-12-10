#include "../Shared/embedded_t.h"
#include "../Shared/Controller.h"
#include "enet.h"

//---------------------------------------------------------------------------------------+
// Status variables                                                                      |
//---------------------------------------------------------------------------------------+
typedef enum { FILTER_DISABLED = 0, FILTER_ENABLED = 1 } State;
State enable = FILTER_DISABLED;

uint block = 0;
char blocked_s[8] = "0000000";

void reportBlock() {
	uint i = 7, t = ++block, mod;
	while (i > 0) {
		mod = t % 10;
		blocked_s[--i] = '0' + mod;
		t /= 10;
	}
	//LCD_WriteText(blocked_r);
}

//---------------------------------------------------------------------------------------+
// Define Interrupt Singals                                                              |
//---------------------------------------------------------------------------------------+
#define INT_TOUCH			BAND_GPIO_PE3
#define INT_NET_SERVER		BAND_GPIO_PE4
#define INT_NET_CLIENT		BAND_GPIO_PE5

void Touch_Handler() {}

void NET_SERVER_Handler() {
	//Read from ISP
	//NET_READDATA(give ISP CS if possible);
	
	//Write to PC
}

void NET_CLIENT_Handler() {
	//Read from PC
	//NET_READDATA(give PC CS if possible);
	//Parse Data
	
	//write to ISP
}

//---------------------------------------------------------------------------------------+
// Real interrups create the need for atomic use of SPI, so instead use a busy wait      |
//---------------------------------------------------------------------------------------+
void Busy_Interrupts() {
	byte i_touch, i_net_server, i_net_client;
	i_touch = i_net_server = i_net_client = 1;
	
	while (1) {
		if (i_touch != INT_TOUCH) {
			i_touch = INT_TOUCH;
			if (i_touch == 0) {
				Touch_Handler();
				//i_touch = 1;
			}
		}
		if (i_net_server != INT_NET_SERVER) {
			i_net_server = INT_NET_SERVER;
			if (i_net_server == 0) {
				NET_SERVER_Handler();
				//i_net_server = 1;
			}
		}
		if (i_net_client != INT_NET_CLIENT) {
			i_net_client = INT_NET_CLIENT;
			if (i_net_client == 0) {
				NET_CLIENT_Handler();
				//i_net_client = 1;
			}
		}
	}
}

//---------------------------------------------------------------------------------------+
// Program initialization logic                                                          |
//---------------------------------------------------------------------------------------+
void exec() {
	// Our program is a busy wait on interrupt conditions
	Busy_Interrupts();
}

//---------------------------------------------------------------------------------------+
// Any configuration on the microcontroller                                              |
//---------------------------------------------------------------------------------------+
void init() {
	// Enable clocks
	SYSCTL->RCGCGPIO = 0x1F; // 11111 => e, d, c, b, a
	SYSCTL->RCGCSSI = 0x1; // SSI0
	GPIO.PortD->LOCK.word = GPIO_UNLOCK; // Port D needs to be unlcoked
	
	// PA[0:1] => Unavailable
	// PA[2:5] => SPI
	GPIO.PortA->DEN.byte[0] = 0xFC;
	GPIO.PortA->AFSEL.byte[0] = 0x3C;
	GPIO.PortA->DIR.bit6 = 1; // External NET Reset (Shared)
	GPIO.PortA->DIR.bit7 = 1; // External LCD Reset
	
	// Pull up configuration necessary to avoid electromagnetic interference between SSI pins
	GPIO.PortA->PUR.bit2 = 1; // Clk
	GPIO.PortA->PUR.bit4 = 1; // Rx
	GPIO.PortA->PUR.bit5 = 1; // Tx
	
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
	
	// Configure SSI Freescale (SPH = 0, SPO = 0)
	SSI0->CR1 = 0; // Disable
	SSI0->CC = 0x5; // Use PIOsc for the clock
	SSI0->CPSR = 0xFE; // Clock divisor = 2 (the minimum, or fastest we can make this divisor)
	SSI0->CR0 = 0x3F07; // SCR = 3 (divisor), SPH = 0, SPO = 0, FRF = 0 (freescale), DSS = 7 (8-bit data)
	SSI0->CR1 |= 0x2; // Enable
	
	// Configure Systick
	SysTick->LOAD = 16000; // 1ms
	NVIC_EN0_R = 0x1;
	
	//NET_Init();
}

void test() {
	SPI_Frame frame;
	byte
		miso[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		mosi[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	init();
	frame.CS = &NET_CS_CLIENT;
	frame.MISO = miso;
	frame.MOSI = mosi;
	frame.N = 10;
	
	// Test output data
	mosi[1] = 0x39;
	
	while (1) {
		SPI_Transfer(SSI0, &frame);
	}
}

//---------------------------------------------------------------------------------------+
// No program logic should be contained here                                             |
//---------------------------------------------------------------------------------------+
int main() {
	test();
	//init();
	//exec();
	while (1);
}

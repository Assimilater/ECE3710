#include "../Shared/Register_Defs_C.h"

#define GPIO_UNLOCK 0x4C4F434B
#define RED_LIGHT (*((volatile unsigned int *)0x424A7F84)) // PF1: 0x4200000 + 32*0x253FC + 4*1
#define GREEN_LIGHT (*((volatile unsigned int *)0x424A7F8C)) // PF3: 0x4200000 + 32*0x253FC + 4*3

//---------------------------------------------------------------------------------------+
// Boolean implementation (minus the nice data compaction afforded to c++)               |
//---------------------------------------------------------------------------------------+
typedef char bool;
#define true 1
#define false 0

//---------------------------------------------------------------------------------------+
// Lookup table for converting ps/2 key strokes to ascii                                 |
// The table starts at 0x15, and ends at 0x4D (meaning last index is 0x38)               |
//---------------------------------------------------------------------------------------+
unsigned char ps2_to_ascii[] = {
	'q','1',0x00,0x00,0x00,'z','s','a','w','2',0x00,
	0x00,'c','x','d','e','4','3',0x00,0x00,' ','v','f','t','r','5',0x00,0x00,
	'n','b','h','g','y','6',0x00,0x00,0x00,'m','j','u','7','8',0x00,
	0x00,',','k','i','o','0','9',0x00,0x00,'.',0x00,'l','p'};
unsigned char ascii(unsigned char in) { return in > 0x38 ? 0 : ps2_to_ascii[in-0x25]; }

//---------------------------------------------------------------------------------------+
// Pointers for managing the ascii buffer (that is to say, the key log)                  |
//---------------------------------------------------------------------------------------+
// Insert variables used in UART1, and GPIOA

//---------------------------------------------------------------------------------------+
// Interrupt handler used to add characters to the key log buffer                        |
//---------------------------------------------------------------------------------------+
void GPIOA_Handler() {
	GPIO_PORTA_ICR_R = 0x2; // Clears interrupt
	
}

//---------------------------------------------------------------------------------------+
// Interrupt handler used fill the TX buffer when there's room and data left to send     |
//---------------------------------------------------------------------------------------+
void UART1_Handler() {
	UART1_ICR_R = 0x20; // Clears interrupt
	
	// Check bit 5 for txfull flag and ascii buffer for remaining data to send
	while (!(UART1_FR_R & 0x20) && 0) { // TODO: Check if ascii stream is finished
		UART1_DR_R = 0; // enter byte to write
	}
}

//---------------------------------------------------------------------------------------+
// Interrupt handler used respond to a button press                                      |
//---------------------------------------------------------------------------------------+
void GPIOF_Handler() {
	static bool enabled = true;
	GPIO_PORTF_ICR_R = 0x1; // Clears interrupt
	
	enabled = enabled == true ? false : true;
	if (enabled) {
		// Turn on green light
		GREEN_LIGHT = 1;
		RED_LIGHT = 0;
		
		GPIO_PORTA_ICR_R = 0x2; // Clears interrupt (just in case)
		GPIO_PORTA_IM_R = 0x4; // Enable GPIOA_Handler
		
	} else {
		// Turn on red light
		GREEN_LIGHT = 0;
		RED_LIGHT = 1;
		
		// Disable GPIOA_Handler
		GPIO_PORTA_IM_R = 0;
		UART1_Handler();
	}
}

void InitConfig() {
	//PA2 is keyboard clock
	//PA3 is keylogger data	
	//PB1 is UART1 Tx
	//PF0 is button
	
	// enable clocks
	SYSCTL_RCGC1_R = 0x2; //UART1
	SYSCTL_RCGC2_R = 0x23; //port A, B, and F

	// configure port A
	GPIO_PORTA_CR_R = 0xC;
	GPIO_PORTA_DIR_R = 0x8;
	GPIO_PORTA_PUR_R = 0x8;
	GPIO_PORTA_DEN_R = 0xC;
	GPIO_PORTA_IM_R = 0; //disable port A's interrupt handler while configuring
	GPIO_PORTA_IS_R = 0; //Sets interrupt to detect edge
	GPIO_PORTA_IBE_R = 0; //interrupt only detects one edge
	GPIO_PORTA_IEV_R = 0; //detects negative edge (button is active-low)
	GPIO_PORTA_IM_R = 0x4; //enables interrupts for PA2
	
	// configure port B
	GPIO_PORTB_CR_R = 0x2;
	GPIO_PORTB_AFSEL_R = 0x2;
	GPIO_PORTB_DEN_R = 0x2;
	
	// configure port F
	GPIO_PORTF_CR_R = 0xE;
	GPIO_PORTF_LOCK_R = GPIO_UNLOCK; //unlock portF
	
	GPIO_PORTF_PUR_R = 0x11; // Set Pull-Up Select
	GPIO_PORTF_DIR_R = 0x0E; // configure pins 0 and 4 as input and 1-3 as output
	GPIO_PORTF_AFSEL_R = 0x0; // Disable AF
	GPIO_PORTF_DEN_R = 0x1F; // Set Digital Enable
	GPIO_PORTF_IM_R = 0; //disable port F's interrupt handler while configuring
	GPIO_PORTF_IS_R = 0; //Sets interrupt to detect edge
	GPIO_PORTF_IBE_R = 0; //interrupt only detects one edge
	GPIO_PORTF_IEV_R = 0; //detects negative edge (button is active-low)
	GPIO_PORTF_IM_R = 0x11; //enables interrupts for PF0 and PF4
	
	//port A is num 0, port F is num 30
	NVIC_EN0_R = 0x40000041; //enable interrupts from ports A and F and UART
	
	NVIC_PRI0_R = 0xE0; //Sets priority of Port A to 7

	// disable UART1
	UART1_CTL_R = 0x0;

	// BRD = 16e6/(16*9600)= 104.1667
	UART1_IBRD_R = 104;	// integer portion: int(104.1667)=104
	UART1_FBRD_R = 0xB;	// fractional portion: int(.1667*2^6+0.5)=11

	// set serial parameters
	UART1_LCRH_R = 0x70; //FIFO enabled, 8-bit word
	
	UART1_IFLS_R = 0x3; // bit in UARTRIS_R is set when Tx FIFO <= 1/4 full
	UART1_IM_R = 0x20; // enables interrupts for Tx
	
	// enable tx and uart
	UART1_CTL_R = 0x101;

}

int main(void) {
	unsigned int z = 0;
	InitConfig();
	GPIOF_Handler();
	
	while (1) { ++z; }
	return 1;
}

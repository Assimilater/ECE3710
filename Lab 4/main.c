#include "../Shared/Register_Defs_C.h"

#define GPIO_UNLOCK 0x4C4F434B

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
// Insert variables used in UART0, and GPIOA

//---------------------------------------------------------------------------------------+
// Interrupt handler used to add characters to the key log buffer                        |
//---------------------------------------------------------------------------------------+
void GPIOA_Handler() {
	GPIO_PORTA_ICR_R = 0x2; // Clears interrupt
	
}

//---------------------------------------------------------------------------------------+
// Interrupt handler used fill the TX buffer when there's room and data left to send     |
//---------------------------------------------------------------------------------------+
void UART0_Handler() {
	UART0_ICR_R = 0x20; // Clears interrupt
	
	// Check bit 5 for txfull flag and ascii buffer for remaining data to send
	while (!(UART0_FR_R & 0x20) && 1) { // TODO: Check if ascii stream is finished
		UART0_DR_R = 0; // enter byte to write
	}
}

//---------------------------------------------------------------------------------------+
// Interrupt handler used respond to a button press                                      |
//---------------------------------------------------------------------------------------+
void GPIOF_Handler() {
	static bool enabled = false;
	GPIO_PORTF_ICR_R = 0x1; // Clears interrupt
	
	enabled = !enabled;
	if (enabled) {
		// Turn light green
		// Enable GPIOA_Handler
	} else {
		// Turn light red
		// Disable GPIOA_Handler
		UART0_Handler();
	}
}

void Init() {
	
	//PA0 is keylogger data
	//PA1 is UART0 Tx
	//PA2 is keyboard clock
	
	//PF0 is button
	
	
	// enable clock: uart then ports
	SYSCTL_RCGC1_R = 0x1; //uart0
	SYSCTL_RCGC2_R = 0x21; //portA and F
	
	GPIO_PORTB_LOCK_R = GPIO_UNLOCK; //unlock portF

	// configure port A
	GPIO_PORTA_CR_R = 0x7;
	GPIO_PORTA_AFSEL_R = 0x2;
	GPIO_PORTA_DIR_R = 0x1;
	GPIO_PORTA_PUR_R = 0x1;
	GPIO_PORTA_DEN_R = 0x7;
	GPIO_PORTA_IM_R = 0; //disable port A's interrupt handler while configuring
	GPIO_PORTA_IS_R = 0; //Sets interrupt to detect edge
	GPIO_PORTA_IBE_R = 0; //interrupt only detects one edge
	GPIO_PORTA_IEV_R = 0; //detects negative edge (button is active-low)
	GPIO_PORTA_IM_R = 0x4; //enables interrupts for PA2
	
	// configure port F
	GPIO_PORTF_CR_R = 0x1;
	GPIO_PORTF_PUR_R = 0x1; // Set Pull-Up Select
	GPIO_PORTF_DIR_R = 0x0; // configure pins as input
	GPIO_PORTF_AFSEL_R = 0x0; // Disable AF
	GPIO_PORTF_DEN_R = 0x1; // Set Digital Enable
	GPIO_PORTF_IM_R = 0; //disable port F's interrupt handler while configuring
	GPIO_PORTF_IS_R = 0; //Sets interrupt to detect edge
	GPIO_PORTF_IBE_R = 0; //interrupt only detects one edge
	GPIO_PORTF_IEV_R = 0; //detects negative edge (button is active-low)
	GPIO_PORTF_IM_R = 0x1; //enables interrupts for PF0
	
	//port A is num 0, port F is num 30
	NVIC_EN0_R = 0x40000001; //enable interrupts from ports A and F
	
	NVIC_PRI0_R = 0xE0; //Sets priority of Port A to 7

	// disable uart0
	UART0_CTL_R = 0x0;

	// BRD = 16e6/(16*9600)= 104.1667
	UART0_IBRD_R = 104;	// integer portion: int(104.1667)=104
	UART0_FBRD_R = 0xB;	// fractional portion: int(.1667*2^6+0.5)=11

	// set serial parameters
	UART0_LCRH_R = 0x70; //FIFO enabled, 8-bit word
	
	UART0_IFLS_R = 0x3; // bit in UARTRIS_R is set when Tx FIFO <= 1/4 full
	UART0_IM_R = 0x20; // enables interrupts for Tx
	// enable tx and uart
	//UART0[UART_CTL+1] = 0x3;
	UART0_CTL_R = 0x101;

}

int main(void) {
	unsigned int z = 0;
	Init();
	
	while (1) { ++z; }
	return 1;
}

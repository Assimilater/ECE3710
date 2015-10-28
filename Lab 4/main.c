#include "../Shared/Register_Defs_C.h"

volatile int PD_DATA_R __attribute__((at(0x400073FC)));
volatile int PD_DIR_R __attribute__((at(0x40007400)));
volatile int PD_AF_R __attribute__((at(0x40007420)));
volatile int PD_DEN_R __attribute__((at(0x4000751C)));
volatile int RCGC2_R __attribute__((at(0x400FE108)));

#define RCGC2_PD		0x00000008  //value for RCGC2 to enable clock for port D

#define GPIO_DIR		0x0400
#define GPIO_AFSEL		0x0420
#define GPIO_PUR		0x0510
#define GPIO_DEN		0x051C
#define GPIO_LOCK		0x0520
#define GPIO_PCTL		0x052C
#define GPIO_CR			0x0524

// system control base addr
unsigned int* SYSCTL = (unsigned int*)0x400FE000;

//UART0 shares pins with Port A (RX: PA0, TX: PA1)
unsigned char* PA = (unsigned char*)0x40004000;

// UART0 base
unsigned char* UART0 = (unsigned char*)0x4000C000;
unsigned char UART0_D __attribute__((at(0x4000C000)));
volatile unsigned int UART0_STAT __attribute__((at(0x4000C018)));

#define UART_FLAG		0x018
#define UART_IBRD		0x024
#define UART_FBRD		0x028
#define UART_LCRH		0x02C
#define UART_CTL		0x030
#define UART_CC			0xFC8

#define GPIO_UNLOCK		0x4C4F434B

// This is a look up table for basic key strokes. 
// This translates from ps/2 to ascii.
// The table starts at 0x15, and ends at 0x4D (meaning last index is 0x38)
unsigned char ps2_to_ascii[] = {
	'q','1',0x00,0x00,0x00,'z','s','a','w','2',0x00,
	0x00,'c','x','d','e','4','3',0x00,0x00,' ','v','f','t','r','5',0x00,0x00,
	'n','b','h','g','y','6',0x00,0x00,0x00,'m','j','u','7','8',0x00,
	0x00,',','k','i','o','0','9',0x00,0x00,'.',0x00,'l','p'};
unsigned char ascii(unsigned char in) { return in > 0x38 ? 0 : ps2_to_ascii[in-0x25]; }

void GPIOA_Handler() {
	// UART Tx Handler
	
	// Keyboard Clock Handler
}

void GPIOF_Handler() {
	// Button handler
	
}

void Init() {
	// enable clock: uart then ports
	SYSCTL_RCGC1_R = 0x1; //uart0
	SYSCTL_RCGC2_R = 0x21; //portA and F
	
	GPIO_PORTB_LOCK_R = GPIO_UNLOCK; //unlock portF

	// configure port A
	GPIO_PORTA_CR_R = 0x7;
	GPIO_PORTA_AFSEL_R = 0x7;
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
	
	//port A is num 1, port F is num 30
	NVIC_EN0_R = 0x20000001; //enable interrupts from ports A and F
	
	NVIC_PRI0_R = 0xE0; //Sets priority of Port A to 7

	// disable uart0
	UART1_CTL_R = 0x0;

	// BRD = 16e6/(16*9600)= 104.1667
	UART0_IBRD_R = 104;	// integer portion: int(104.1667)=104
	UART0_FBRD_R = 0xB;	// fractional portion: int(.1667*2^6+0.5)=11

	// set serial parameters
	UART0_LCRH_R = 0x70; //FIFO enabled, 8-bit word

	// enable tx rx and uart
	//UART0[UART_CTL+1] = 0x3;
	UART0_CTL_R = 0x301;

}

int main(void) {
	unsigned int z = 0;
	Init();
	
	while (1) { ++z; }
	return 1;
}

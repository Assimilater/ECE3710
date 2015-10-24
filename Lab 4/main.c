#include "../Shared/Register_Defs_C.h"

volatile int PD_DATA_R __attribute__((at(0x400073FC)));
volatile int PD_DIR_R __attribute__((at(0x40007400)));
volatile int PD_AF_R __attribute__((at(0x40007420)));
volatile int PD_DEN_R __attribute__((at(0x4000751C)));
volatile int RCGC2_R __attribute__((at(0x400FE108)));
#define RCGC2_PD      0x00000008  //value for RCGC2 to enable clock for port D


#define GPIO_DIR		0x0400
#define GPIO_AFSEL		0x0420
#define GPIO_PUR		0x0510
#define GPIO_DEN		0x051C
#define GPIO_LOCK		0x0520
#define GPIO_PCTL		0x052C
#define GPIO_CR			0x0524

// system control base addr
unsigned int *SYSCTL = (unsigned int *)0x400FE000;

//UART0 shares pins with Port A (RX: PA0, TX: PA1)
unsigned char *PA = (unsigned char *)0x40004000;

// UART0 base
unsigned char *UART0 = (unsigned char *)0x4000C000;
unsigned char UART0_D __attribute__((at(0x4000C000)));
volatile unsigned int UART0_STAT __attribute__((at(0x4000C018)));
#define UART_FLAG		0x018
#define UART_IBRD		0x024
#define UART_FBRD		0x028
#define UART_LCRH		0x02C
#define UART_CTL		0x030
#define UART_CC			0xFC8

void Init() {
	// enable clock: uart then ports
	SYSCTL_RCGC1_R = 0x1; //uart0
	SYSCTL_RCGC2_R = 0x1; //portA

	// PB1,0: enable alt. func. and pin
	GPIO_PORTA_CR_R = 0x3;
	GPIO_PORTA_AFSEL_R = 0x3;
	GPIO_PORTA_DEN_R = 0x3;

	// PB0: set as output
	GPIO_PORTA_DIR_R = 0x1;
	GPIO_PORTA_PUR_R = 0x1;

	//GPIO_PORTA_PCTL_R = 0x11; // configure alternate functionality see p.1344

	// disable uart0
	UART1_CTL_R = 0x0;

	// BRD = 16e6/(16*9600)= 104.1667
	UART0_IBRD_R = 104;	// integer portion: int(104.1667)=104
	UART0_FBRD_R = 0xB;	// fractional portion: int(.1667*2^6+0.5)=11

	// set serial parameters
	UART0_LCRH_R = 0x70; //FIFO enabled, 8-bit word

	// 6. enable tx rx and uart
	//UART0[UART_CTL+1] = 0x3;
	UART0_CTL_R = 0x301;
}

int main(void) {
	unsigned char z;

	Init();



	while (z != 50) {
		for (z = 0; z<16; z++)
			PD_DATA_R = z;
	}

	return 1;
}

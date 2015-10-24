
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
	unsigned char *SYSCTL = (unsigned char *) 0x400FE000;
	
	//UART0 shares pins with Port A (RX: PA0, TX: PA1)
	unsigned char *PA = (unsigned char *) 0x40004000;
	
	// UART0 base
	unsigned char *UART0 = (unsigned char *) 0x4000C000;
	unsigned char UART0_D __attribute__((at(0x4000C000)));
	volatile unsigned int UART0_STAT __attribute__((at(0x4000C018)));
	#define UART_FLAG		0x018
	#define UART_IBRD		0x024
	#define UART_FBRD		0x028
	#define UART_LCRH		0x02C
	#define UART_CTL		0x030
	#define UART_CC			0xFC8
	
void UART0Init()
{
	// 0. set sysclk to main osc (8MHz crystal)
	// final value should be 0x078E3B82 (remember little endian)
	SYSCTL[0x60] = 0x82;
	SYSCTL[0x61] = 0x3B;
	SYSCTL[0x62] = 0x8E;
	SYSCTL[0x63] = 0x07;

	// 1. enable clock: uart then port
	SYSCTL[0x104] = 0x1; //uart0
	SYSCTL[0x108] = 0x1; //portA

	// 2. PA1: enable alt. func. and pin
	PA[GPIO_AFSEL] = 0x2;
	PA[GPIO_DEN] = 0x2;
  
	// 3. disable uart0
	UART0[UART_CTL] = 0x0;
  
	// 4. set baudrate divisor
	// BRD = 16e6/(16*9600)= 104.1667
	UART0[UART_IBRD] = 104;	// integer portion: int(104.1667)=104
	UART0[UART_FBRD] = 0xB;	// fractional portion: int(.1667*2^6+0.5)=11

	// 5. set serial parameters: FIFO enabled, 8-bit word, start/stop/parity bits
	UART0[UART_LCRH] = 0x72;
  
	// 6. enable tx rx and uart
	UART0[UART_CTL+1] = 0x3;
	UART0[UART_CTL] = 0x01;
}

int main(void)
{
	UART0Init();
	
	unsigned char z;
	
	// initialize port: RMW-cycle so much better in C...
	RCGC2_R |= RCGC2_PD; //activate port D: RCGC2 = RCGC2 | RCGC2_PD
	PD_DIR_R |= 0x0F;    //make PD3-0 output
	PD_AF_R &= 0x00;    //disable alt. func. 
	PD_DEN_R |= 0x0F; //enable digital I/O on PD3-0
	

	
	while(1)
	{
		for(z=0;z<16;z++)
			PD_DATA_R = z;
	}
	
	return 1;
}

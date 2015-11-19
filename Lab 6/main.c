#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "../Shared/bool.h"

/** Generation of SIN table:
#include <iostream> // cout
#include <math.h> // sin
#define PI 3.14159265358979323846264

int main() {
	for (int n = 0; n < 40; ++n) {
		std::cout << sin(2 * n * PI / 40) << std::endl;
	}
	return 0;
}
*/
#define LOOKUP_SIZE 40
const double SIN[LOOKUP_SIZE] = {
	0, 0.156434, 0.309017, 0.45399, 0.587785, 0.707107, 0.809017, 0.891007, 0.951057, 0.987688,
	1, 0.987688, 0.951057, 0.891007, 0.809017, 0.707107, 0.587785, 0.45399, 0.309017, 0.156434,
	0, -0.156434, -0.309017, -0.45399, -0.587785, -0.707107, -0.809017, -0.891007, -0.951057, -0.987688,
	-1, -0.987688, -0.951057, -0.891007, -0.809017, -0.707107, -0.587785, -0.45399, -0.309017, -0.156434
};

unsigned int voltage = 0;
unsigned int adc_scnt = 0;

const unsigned int MAX_FREQUENCY = 400;
const unsigned char MAX_VOLTAGE = 5;
void ADC0SS0_Handler() {
	ADC0->ISC = 0x1; // acknowledge/clear interrupt
	TIMER0->ICR = 0x1; // clears the timer expiration flag
	voltage = ((voltage * adc_scnt) + ADC0->SSFIFO0) / (adc_scnt + 1); // update moving average for voltage based on sample
	++adc_scnt;
}

void TIMER1A_Handler() {
	TIMER1->ICR = 0x1;
	
	// Update systick freq. using data from ADC0SS0 handler
	SysTick->CTRL = 0;
	SysTick->LOAD = MAX_FREQUENCY * (voltage / MAX_VOLTAGE);
	SysTick->VAL = 1;
	SysTick->CTRL = 0x7;
	// Update DAC Timer
	
	voltage = adc_scnt = 0;
}

void SysTick_Handler() {
	// Send to I2C to update output voltage attached to speakers
	static unsigned int i;
	double a = SIN[++i];
	i %= LOOKUP_SIZE;
}

//---------------------------------------------------------------------------------------+
// Program initialization logic                                                          |
//---------------------------------------------------------------------------------------+
void exec() {
	
}

//---------------------------------------------------------------------------------------+
// Translation of code written in assembly before to configure for 20MHz                 |
//---------------------------------------------------------------------------------------+
void useXTAL() {
	SYSCTL->RCC = 0x04C00D40;
	while (!(SYSCTL->RIS & 0x40));
	SYSCTL->RCC &= ~0x800;
	/** Equivalent Assembly
		; Configure SysClk				; SYSDIV(0x9), BYPASS(0b1), XTAL(0x15)
		LDR R0, =SYSCTL					; OSCSRC(0b00), MOSCDIS(0b0)
		LDR R1, =0x04C00D40				; (0000)(0 100)(11 00) (0000) (0000)(1 101)(01 00) (0000)
		STR R1, [R0, #SYS_RCC]			; Bypass the PLL so the PLL can be validated before use
		
PLL_Validate
		LDR R2, [R0, #SYS_RIS]			; Poll the Raw Interrupt Status bit PLLLRIS until 1
		ANDS R2, #0x40					; PLLLRIS = bit 6
		BEQ PLL_Validate
		
		; Disable PLL Bypass now that it has reached TReady
		MVN R2, #0x800
		AND R1, R2
		STR R1, [R0, #SYS_RCC]
	**/
}

//---------------------------------------------------------------------------------------+
// Any configuration on the microcontroller                                              |
//---------------------------------------------------------------------------------------+
void init() {
	useXTAL();
	
	// enable clocks
	SYSCTL->RCGCTIMER = 0x7; // Enable timers 0, 1, 2
	SYSCTL->RCGCADC = 0x1; // enable ADC0
	SYSCTL->RCGCGPIO = 0xA; // Enable ports B and D
	SYSCTL->RCGCI2C = 0x1; // Enable I2C0
	GPIO.PortD->LOCK.word = GPIO_UNLOCK;
	
	//config PD
	//PD0 is AIN7/SS0 for ADC0
	GPIO.PortD->AFSEL.word = 0x1; // enable AF for PD0
	//GPIO.PortD->DEN.bit0 = 0; // Set PD as analog input
	GPIO.PortD->AMSEL.word = 0x1; // disable isolation for PD0 for analog
	GPIO.PortD->ADCCTL.word = 0x1; // PD0 triggers ADC
	
	//config PB
	//PB2 is I2CSCL, PB3 is I2CSDA
	GPIO.PortB->AFSEL.word = 0xC; // enable AF for PB2,3
	//GPIO.PortB->DEN.word = 0xC; // PB2,3
	GPIO.PortB->ODR.bit3 = 1;
	GPIO.PortB->PCTL.word = 0x3300; // See Pg.1351
	
	//config I2C
	I2C0->MCR = 0x10; // initialize I2C
	I2C0->MTPR = 0x1; // TPR = 20MHz/(2*(SCL_LP[6] + SCL_HP[4])*SCL_CLK[400000]) - 1 = 1.5
	
	//config ADC
	ADC0->ACTSS = 0; // Disable ADC0
	ADC0->EMUX = 0x5; // Timer triggers interrupt for sequencer0
	ADC0->SSMUX0 = 0x7; // set AIN7 for first sample
	ADC0->SSCTL0 = 0x6; // one sample sequence
	ADC0->IM = 0x1; // enable interrupts for SS0
	ADC0->ACTSS = 0x1; // enable SS0 in ADC0
	
	//GPTM0
	TIMER0->CTL = 0; // disable
	TIMER0->CFG = 0; // 32-bit
	TIMER0->TAMR = 0x2; // periodic mode
	TIMER0->TAILR = 0x9C40; // Reload value 2ms for 20 MHz clock
	TIMER0->CTL = 0x21; // enable Timer and ADC trigger
	
	TIMER1->CTL = 0; // disable
	TIMER1->CFG = 0; // 32-bit
	TIMER1->TAMR = 0x2; // periodic mode
	TIMER1->TAILR = 0x989680; // Reload value 500ms for 20 MHz clock
	TIMER1->IMR = 0x1; // Enable interrupts on time-out	
	TIMER1->CTL = 1; // enable
	
	//SysTick
	//SysTick->LOAD =;
	//SysTick->CTRL =;
	
	ADC0->ISC = 0x1; // acknowledge/clear interrupt
	NVIC_EN0_R = 0x204000; //enable interrupts for ADC0-SS0, and GPTM1
}

//---------------------------------------------------------------------------------------+
// No program logic should be contained here                                             |
//---------------------------------------------------------------------------------------+
int main() {
	init();
	exec();
	while (1);
}

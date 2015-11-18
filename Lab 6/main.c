#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "../Shared/bool.h"

//---------------------------------------------------------------------------------------+
// Program initialization logic                                                          |
//---------------------------------------------------------------------------------------+
void exec() {
	
}

//---------------------------------------------------------------------------------------+
// Translation of code written in assembly before to configure for 20MHz                 |
//---------------------------------------------------------------------------------------+
void useXTAL() {
	/** Equivalent Assembly
		; Configure SysClk				; SYSDIV(0x9), BYPASS(0b1), XTAL(0x15)
		LDR R0, =SYSCTL					; OSCSRC(0b00), MOSCDIS(0b0)
		LDR R1, =0x04C00D40				; (0000)(0 100)(11 00) (0000) (00 00)(11 01)(01 00) (0000)
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
	
	SYSCTL->RCC = 0x04C00D40;
	while (!(SYSCTL->RIS & 0x40));
	SYSCTL->RCC &= ~0x800;
}

//---------------------------------------------------------------------------------------+
// Any configuration on the microcontroller                                              |
//---------------------------------------------------------------------------------------+
void init() {
	useXTAL();
	
	//SYSCTL_RCGC1_R |= 0x10000; //GPTM0
	SYSCTL->RCGCTIMER = 0x7; //Enable timers 0, 1, 2
	//SYSCTL_RCGC2_R |= 0x8; //port D
	SYSCTL->RCGCGPIO = 0x1B;
	//SYSCTL_RCGC0_R |= 0x10000; //ADC0
	SYSCTL->RCGCADC = 0x1; //enable ADC0
	//SYSCTL->RCGCSSI = 0x1;
	//GPIO.PortD->LOCK.word = GPIO_UNLOCK;
		// enable clocks
	
	//config PD
	//PD0 is AIN7
	//GPIO_PORTD_AFSEL_R |= 0x1; //enable AF for PD0
	GPIO.PortD->AFSEL.word = 0x1; //enable AF for PD0
	//GPIO_PORTD_DEN_R = 0; //Set PD as analog input
	GPIO.PortD->DEN.bit0 = 0;
	/GPIO_PORTD_AMSEL_R |= 0x1; //disable isolation for PD0 for analog
	GPIO.PortD->AM
	//GPIO_PORTD_ADCCTL_R |= 0x1; //PD0 triggers ADC
	
	//config ADC
	//ADC0_ACTSS_R = 0; //Disable ADC0
	//ADC0_EMUX_R |= 0x5; //Timer triggers interrupt for sequencer0
	//ADC0_SSCTL0_R |= 0x6; //one sample sequence
	//ADC0_IM_R |= 0x1; //enable interrupts for SS0
	//ADC0_ACTSS_R |= 0x1; //enable SS0 in ADC0
	
	//NVIC_EN0_R |= 0x4000; //enable interrupts for ADC0 SS0
	
	//GPTM0
	//TIMER0_CTL_R = 0; //disable
	//TIMER0_CFG_R = 0; //32-bit
	//TIMER0_TAMR_R |= 0x2; //periodic mode
	//TIMER0_TAILR_R = 0x7D00; //Reload value 2ms for 16 MHz clock
	//TIMER0_CTL_R |= 1; //enable

}

//---------------------------------------------------------------------------------------+
// No program logic should be contained here                                             |
//---------------------------------------------------------------------------------------+
int main() {
	init();
	exec();
	while (1);
	return 1;
}

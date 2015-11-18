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
	
	SYSCTL->RCGCGPIO = 0x1B;
	//SYSCTL->RCGCSSI = 0x1;
	//GPIO.PortD->LOCK.word = GPIO_UNLOCK;
	
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

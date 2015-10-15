; main.s
; Runs on any Cortex M processor
	THUMB
	AREA DATA, ALIGN=2
	ALIGN
	AREA |.text|, CODE, READONLY, ALIGN=2
	EXPORT Start

GPIO_CLOCK		EQU 0x400FE108			; See users manual p. 464
GPIO_UNLOCK		EQU 0x4C4F434B			; See users manual p. 685

GPIO_PORTB		EQU 0x40005000
GPIO_PORTF		EQU 0x40025000

SYSCLK			EQU 0xE000E000
GPTM0			EQU 0x40030000

SYSCTL			EQU 0x400FE000
SYS_RIS			EQU 0x0050
SYS_RCC			EQU 0x0060
SYS_RCGC1		EQU 0x0104
SYS_RCGCUART	EQU 0x0618

GPIO_DATA		EQU 0x03FC
GPIO_DIR		EQU 0x0400
GPIO_AFSEL		EQU 0x0420
GPIO_PUR		EQU 0x0510
GPIO_DEN		EQU 0x051C
GPIO_LOCK		EQU 0x0520
GPIO_PCTL		EQU 0x052C
GPIO_CR			EQU 0x0524

; 13 => CR, 10 => NL
MESSAGE_0		DCB "Nope", 13, 10, 0
MESSAGE_1		DCB "You are doomed", 13, 10, 0
MESSAGE_2		DCB "Concentrate you fool", 13, 10, 0
MESSAGE_3		DCB "What a rubbish question", 13, 10, 0
MESSAGE_4		DCB "Only in your dreams", 13, 10, 0
MESSAGE_5		DCB "Yes now leave me alone", 13, 10, 0
MESSAGE_6		DCB "Heh you wish", 13, 10, 0
MESSAGE_7		DCB "Oh yeah that will happen", 13, 10, 0
MESSAGE_8		DCB "Stop bothering me", 13, 10, 0
MESSAGE_9		DCB "Not if you were the last person on earth", 13, 10, 0

ALIGN ; p. 149

Start
		; Turn on the clock for port B
		LDR R0, =GPIO_CLOCK
		MOV R1, #0x2
		STR R1, [R0]
		
		LDR R0, =SYSCTL
		
		; Enable UART 1
		MOV R1, #0x2
		STR R1, [R0, #SYS_RCGCUART]
		
		; Configure Port B
		LDR R0, =GPIO_PORTB				; Unlock Port B
		STR R2, [R0, #GPIO_LOCK]
		
		MOV R1, #0x3					; We are configuring only the last 2 bits of Port B
		STR R1, [R0, #GPIO_CR]			; Set CR to limit which bits are modified on write
		STR R1, [R0, #GPIO_DEN]			; Set Digital Enable
		STR R1, [R0, #GPIO_AFSEL]		; Enable Alternate Functionality
		
		MOV R1, #0x1					; Configure the output pins
		STR R1, [R0, #GPIO_DIR]			; Configure pins as input/output
		STR R1, [R0, #GPIO_PUR]			; Set Pull-Up Select
		
		MOV R1, #0x11					; See user's manual p.1344
		STR R1, [R0, #GPIO_PCTL]		; Configure which alternate functionality is used
		
		; SysTick
		LDR R0, =SYSCLK
		MOV R1, #0
		STR R1, [R0, #0x10]				; Disable Systick while doing init
		
		MOV R1, #9						; Used to cycle through 10 random messages
		STR R1, [R0, #0x14]				; Set Reload register
		STR R1, [R0, #0x18]				; Clear the Current register
		
		MOV R1, #0x5
		STR R1, [R0, #0x10]				; Enable SysTick and use core clock
		
	; R0 stores SYSCLK
	; R1 stores GPIO_PORTB
		
Program
		
		
Final
		
		B Final
		
	ALIGN
	END

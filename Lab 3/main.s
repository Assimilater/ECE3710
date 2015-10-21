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

UART1			EQU 0x4000D000
UART_FLAG		EQU 0x018
UART_IBRD		EQU 0x024
UART_FBRD		EQU 0x028
UART_LCRH		EQU 0x02C
UART_CTL		EQU 0x030
UART_CC			EQU 0xFC8

GPIO_DATA		EQU 0x03FC
GPIO_DIR		EQU 0x0400
GPIO_AFSEL		EQU 0x0420
GPIO_PUR		EQU 0x0510
GPIO_DEN		EQU 0x051C
GPIO_LOCK		EQU 0x0520
GPIO_PCTL		EQU 0x052C
GPIO_CR			EQU 0x0524

; 13 => CR, 10 => LF, 0 => NULL
MJUMPTABLE		EQU 0x0
MESSAGE0		DCB "Nope", 13, 10, 0
MESSAGE1		DCB "You are doomed", 13, 10, 0
MESSAGE2		DCB "Concentrate you fool", 13, 10, 0
MESSAGE3		DCB "What a rubbish question", 13, 10, 0
MESSAGE4		DCB "Only in your dreams", 13, 10, 0
MESSAGE5		DCB "Yes now leave me alone", 13, 10, 0
MESSAGE6		DCB "Heh you wish", 13, 10, 0
MESSAGE7		DCB "Oh yeah that will happen", 13, 10, 0
MESSAGE8		DCB "Stop bothering me", 13, 10, 0
MESSAGE9		DCB "Not if you were the last person on earth", 13, 10, 0

ALIGN ; p. 149

Start
		; Turn on the clock for ports B and F
		LDR R0, =GPIO_CLOCK
		MOV R1, #0x22
		STR R1, [R0]
		
		LDR R0, =SYSCTL
		LDR R2, =GPIO_UNLOCK
		
		; Enable UART 1
		MOV R1, #0x2
		STR R1, [R0, #SYS_RCGC1]
		
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
		
		; Configure Port F
		LDR R0, =GPIO_PORTF				; Unlock Port F
		STR R2, [R0, #GPIO_LOCK]
		
		MOV R1, #0x11					; We are configuring only the last 2 bits of Port B
		STR R1, [R0, #GPIO_CR]			; Set CR to limit which bits are modified on write
		STR R1, [R0, #GPIO_DEN]			; Set Digital Enable
		STR R1, [R0, #GPIO_PUR]			; Set Pull-Up Select
		
		MOV R1, #0x0
		STR R1, [R0, #GPIO_DIR]			; Configure pins as input
		STR R1, [R0, #GPIO_AFSEL]		; Disable Alternate Functionality
		
		; Configure UART 1
		LDR R0, =UART1
		
		MOV R1, #0
		STR R1, [R0, #UART_CTL]			; Disable UART
		
										; BRD = 16e6/(16*9600) = 104.1667
		MOV R1, #104					; int(104.1667) = 104
		STR R1, [R0, #UART_IBRD]
		MOV R1, #0xB					; int(.1667*64+.5)=11=0xB
		STR R1, [R0, #UART_FBRD]
		
		MOV R1, #0x60					; 8 bit word length
		STR R1, [R0, #UART_LCRH]
		
		MOV R1, #0						; Use the system clock
		STR R1, [R0, #UART_CC]
		
		MOV R1, #0x301					; Enable Tx and Rx and UART
		STR R1, [R0, #UART_CTL]
		
		; SysTick
		LDR R0, =SYSCLK
		MOV R1, #0
		STR R1, [R0, #0x10]				; Disable Systick while doing init
		
		MOV R1, #9						; Used to cycle through 10 random messages
		STR R1, [R0, #0x14]				; Set Reload register
		STR R1, [R0, #0x18]				; Clear the Current register
		
		MOV R1, #0x5
		STR R1, [R0, #0x10]				; Enable SysTick and use core clock
		
	; R0 stores MJUMPTABLE
	; R1 stores SYSCLK
	; R2 stores GPIO_PORTB
	; R3 Stores UART1
	; R4+ stores temporary/calculated values
	; R4 often used for passed and returned arguments of function calls (similar to %eax)
		
		LDR R0, =MJUMPTABLE
		LDR R1, =SYSCLK
		LDR R2, =GPIO_PORTB
		LDR R3, =UART1
		BL GenJump
		
		; Send CRLF
		MOV R4, #13
		STRB R4, [R3]
		MOV R4, #10
		STRB R4, [R3]
		
Program
		; Wait for button press or any character being received
		;BEQ Program
		
		BL SendStr
		B Program
		
		
GenJump
	; Create a jump table in RAM (analogous to setting up a switch statement in C)
		PUSH {R1}
		
		LDR R1, =MESSAGE0
		STR R1, [R0, #0x00]
		
		LDR R1, =MESSAGE1
		STR R1, [R0, #0x04]
		
		LDR R1, =MESSAGE2
		STR R1, [R0, #0x08]
		
		LDR R1, =MESSAGE3
		STR R1, [R0, #0x0C]
		
		LDR R1, =MESSAGE4
		STR R1, [R0, #0x10]
		
		LDR R1, =MESSAGE5
		STR R1, [R0, #0x14]
		
		LDR R1, =MESSAGE6
		STR R1, [R0, #0x18]
		
		LDR R1, =MESSAGE7
		STR R1, [R0, #0x1C]
		
		LDR R1, =MESSAGE8
		STR R1, [R0, #0x20]
		
		LDR R1, =MESSAGE9
		STR R1, [R0, #0x24]
		
		POP {R1}
		BX LR
		
		
SendStr
	; Send a random string to the UART module
		PUSH {R5, R6, R7}
		
		LDR R5, [R1, #0x18]				; Read random value from SysClk between 0-9 inclusive
		LSL R5, #2						; x4 to get word-aligned offset in jump table
		LDR R5, [R0, R5]				; Get the address of the string to send
		
		MOV R6, #0						; Offset counter for loop
StrLoop
		LDRB R7, [R5, R6]				; Load byte from message
		ADD R6, #1						; Increment offset counter
		CMP R7, #0						; Check for null terminator
		BEQ StrSent
		
UARTLoop
		LDR R8, [R3, #UART_FLAG]
		ANDS R8, #0x20					; Check full flag
		BNE UARTLoop					; Wait until buffer is not full
		STRB R7, [R3]					; Write to UART buffer
		B StrLoop
		
StrSent
		POP {R5, R6, R7}
		BX LR
		
		
Final
		B Final
		
	ALIGN
	END

; main.s
; Runs on any Cortex M processor
	THUMB
	AREA DATA, ALIGN=2
	ALIGN
	AREA |.text|, CODE, READONLY, ALIGN=2
	EXPORT Start

GPIO_CLOCK		EQU 0x400FE108			; See users manual p. 464
GPIO_UNLOCK		EQU 0x4C4F434B			; See users manual p. 685

GPIO_PORTA		EQU 0x40004000
GPIO_PORTB		EQU 0x40005000
GPIO_PORTF		EQU 0x40025000
BAND_DATAF		EQU 0x424A7F80			; 0x4200000 + 0x253FC * 32

SYSTICK			EQU 0xE000E000
GPTM0			EQU	0x40030000
RCGC1			EQU 0x400FE000

;DATA_R_OFF		EQU 0x03FC
;DIR_R_OFF		EQU 0x0400
;AFSEL_R_OFF	EQU 0x0420
;PUR_R_OFF		EQU 0x0510
;DEN_R_OFF		EQU 0x051C
;LOCK_R_OFF		EQU 0x0520
;CR_R_OFF		EQU 0x0524

Start
	; Enable ports A, B, and F
	; A[2-7] and B[0-3] correspond to the 10 LEDS
	; B[4-7] corresponds to the speed DIP switch
	; F2 is SW2 (right)	-> Player 2
	; F3 is SW1 (left)	-> Player 1
	
		; Turn on the clock
		LDR R0, =GPIO_CLOCK
		MOV R1, #0x23					; 0x23 = 0b10.0011 (ports a b and f)
		STR R1, [R0]
		
		; Load the GPIO unlock code
		LDR R2, =GPIO_UNLOCK			; in hindsight, I think we only need it for port F
		
		
		; Port F
		LDR R0, =GPIO_PORTF
		STR R2, [R0, #0x520]			; Unlock Port F
		
		MOV R1, #0x0C					; Configure pins 2 and 3
		STR R1, [R0, #0x524]			; Set CR to limit which bits are modified on write
		STR R1, [R0, #0x51C]			; Set Digital Enable
		
		MOV R1, #0
		STR R1, [R0, #0x510]			; Set Pull-Up Select
		STR R1, [R0, #0x400]			; Configure pins as input
		STR R1, [R0, #0x420]			; Disable Alternate Functionality
		
		
		; Port B
		LDR R0, =GPIO_PORTB				; Unlock Port B
		STR R2, [R0, #0x520]
		
		MOV R1, #0xFF					; We are configuring only the last 4 bits of Port B
		STR R1, [R0, #0x524]			; Set CR to limit which bits are modified on write
		STR R1, [R0,#0x51C]				; Set Digital Enable
		
		MOV R1, #0x0F					; Configure the output pins
		STR R1, [R0,#0x400]				; Configure pins as input/output
		STR R1, [R0, #0x510]			; Set Pull-Up Select
		
		MOV R1, #0						; Configure features we want diabled
		STR R1, [R0,#0x420]				; Disable Alternate Functionality
		
		
		; Port A
		LDR R0, =GPIO_PORTA				; Unlock Port A
		STR R2, [R0, #0x520]
		
		MOV R1, #0xFC					; We are configuring only the first 6 bits of Port A
		STR R1, [R0, #0x524]			; Set CR to limit which bits are modified on write
		STR R1, [R0,#0x400]				; Configure pins as output
		STR R1, [R0, #0x510]			; Set Pull-Up Select
		STR R1, [R0,#0x51C]				; Set Digital Enable
		
		MOV R1, #0						; Configure features we want diabled
		STR R1, [R0,#0x420]				; Disable Alternate Functionality
		
		; SysTick
		LDR R0, =SYSTICK
		MOV R1, #0
		STR R1, [R0, #0x10]				; Disable Systick while doing init
		
		LDR R1, =0x7A1200				; counts every 62.5us, .5 sec adds up to 0x7A1200
		STR R1, [R0, #0x14]				; Set Reload register
		STR R1, [R0, #0x18]				; Clear the Current register
		
		MOV R1, #0x5
		STR R1, [R0, #0x10]				; Enable SysTick and use core clock
		
		; General Purpose Timer
		LDR R0, =RCGC1
		MOV R1, #0x10000				; bit 16
		STR R1, [R0, #0x104]
		
		LDR R0, =GPTM0
		MOV R1, #0
		STR R1, [R0, #0xC]				; Disable GPTM
		STR R1, [R0]					; Configure it as a 32 bit timer
		
		MOV R1, #1
		STR R1, [R0, #0x4]				; One-shot mode
		
		
	; R0 stores a port address
	; R1 tracks player 1
	; R2 tracks player 2
	; R3 stores player 1's speed
	; R4 stores player 2's speed
	; R5 stores the button state
	; R6 stores the game state {
	;	0 => No Advantage
	;	1 => Player 1 Advantage,
	;	2 => Player 2 Advantage,
	;	3 => No Advancing
	;}
	; R6 is also used for blinking {
	;	0 => Blink on
	;	1 => Blink off
	;}
	; R7+ stores temporary values and calculations
		
Program
		; Init Player Position and State
		MOV R1, #0x20					;Binary 00 0010 0000 - Player 1 start place
		MOV R2, #0x10					;Binary 00 0001 0000 - Player 2 start place
		MOV R6, #0
		
		ORR R8, R1, R2					; R8 => On blink
		MOV R9, #0						; R9 => Off blink
		
		LDR R0, =GPIO_PORTF
		LDR R10, =SYSTICK
		
Begin
		; Check buttons
		LDR R7, [R0, #0x03FC]			; Load from GPIO_PORTF
		
		AND R11, R7, #0x08				; Get button for player 1
		CMP R11, #0
		ORRNE R9, R1
		
		AND R11, R7, #0x04				; Get button for player 2
		CMP R11, #0
		ORRNE R9, R2
		
		; Select which blink we want
		CMP R6, #0
		ITE EQ
		MOVEQ R7, R8
		MOVNE R7, R9
		BL LED
		
		; If timer expiration
		LDR R12, [R10, #0x10]
		ANDS R12, #0x10000
		MVNNE R6, R6					; Alternate betwen blink on and off
		
		; Loop until both players are ready (both are solid)
		CMP R8, R9
		BNE Begin
		
GetSpeed
		LDR R0, =GPIO_PORTB
		LDR R4, [R0, #0x03FC]			; Get speed values
		
		MOV R7, #3						; Select 2 bits
		AND R3, R7, R4, LSR #6			; put PB6 and 7 as Player 1 speed
		AND R4, R7, R4, LSR #4			; put PB4 and 5 as Player 2 speed
		BL PushBack
		B Idle
		
Player1
		CMP R6, #1
		BXEQ LR							; Exit out if player 1 already has advantage
		LSL R1, #1						; Advance Player 1
		
		CMP R6, #2
		BEQ Draw						; It's a draw if player 2 had the advantage
		
		; Set Timer A to speed based countdown and start
		
		MOV R6, #1						; Flag player 1 with the advantage
		B LEDUpdate
		
Player2
		CMP R6, #2
		BXEQ LR							; Exit out if player 2 already has advantage
		LSR R2, #1						; Advance Player 2
		
		CMP R6, #1
		BEQ Draw						; It's a draw if player 2 had the advantage
		
		; Set Timer A to speed based countdown and start
		
		MOV R6, #2						; Flag player 2 with the advantage
		B LEDUpdate
		
		
SpeedWin
		CMP R6, #1						; It is assumed if R6 is not 1, it is 2
		ITE EQ
		LSREQ R1, #1
		LSLNE R2, #1
		
		BL LEDUpdate
		BL PushBack
		B Idle
		
Draw
		POP {LR}
		ADD R8, #1
		BL PushBack
		B Idle
		
PushBack
		PUSH {R0}						; Preserver R0
		LDR R0, =SYSTICK
		LDR R11, =0x7A12000				; 1 Second
		LDR R12, [R0, #0x18]			; Read value from SYSTYC (0-0.5s)
		ADD R11, R12, LSL #1			; x2 to get a random number between 0-1 s and add 1s
		
		LDR R0, =GPTM0
		STR R11, [R0, #0x28]			; Set RELOAD value on GPTimer
		MOV R12, #1
		STR R12, [R0, #0xC]
		
PushWait
		; Loop to wait for expiration on GPTimer
		LDR R12, [R0, #0x1C]
		ANDS R12, #0x1
		BEQ PushWait
		
		; Clear timer expiration flag
		MOV R12, #1
		STR R12, [R0, #0x24]
		
		POP {R0}						; Restore R0
		
		; Retreat both players
		LSR R1, #1
		LSL R2, #1
		
LEDUpdate
		ORR R7, R1, R2
		
		PUSH {LR}
		BL LED
		POP {LR}
		
		BX LR
		
Idle
	; Idle - Where the majority of the program will be spent
		; Sample buttons
		LDR R0, =GPIO_PORTF
		LDR R8, [R0, #0x03FC]			; Get current state
		
		MVN R5, R5
		AND R5, R8						; R5 = ~R5 * R8 (1 on rising edge in button state)
		
		LSRS R5, #2						; Check if player 2 pressed based on carry bit
		BLHS Player2					; Branch if carry bit
		
		LSRS R5, #2						; Check if player 1 pressed based on carry bit
		BLHS Player1					; Branch if carry bit
		
		MOV R5, R8						; Store current state for next cycle
		
		; Check expiration on speed timer
		LDR R0, =GPTM0
		LDR R8, [R0, #0x1C]
		ANDS R8, #0x1
		BNE SpeedWin
		B Idle
		
		
SpeedDuel
		LDR R7, [R0, #0x03FC]
		CMP R7, #0
		BEQ SpeedDuel
		CMP R7, #0x11
		BEQ SpeedDuel
		
		; Check if SW1 is pressed
		CMP R7, #0x10
		MOVEQ R6, #4
		LSREQ R1, #1
		
		; Check if SW2 is pressed
		CMP R7, #0x1
		MOVEQ R6, #5
		LSLEQ R2, #1
		
Winner
		ORR R7, R1, R2
		BL LED
		BL Delay5
		MOV R7, #0
		BL LED
		BL Delay5
		B Winner
		
Delay5
		MOV32 R12, #0X225510
		B DELAY
		
DELAY
		SUBS R12, #1				; 1 MACHINE CYCLE
		BNE DELAY					; 1 MC + 2 if the branch is taken
		BX  LR
		
GAME_OVER
		
		
LED
	; LED - Output a 10-bit value in R7 to LED on GPIO Ports A and B
		PUSH {R0}
		PUSH {R8}
		
		; Write the data to GPIO_PORTA
		LDR R0, =GPIO_PORTA
		MVN R8, R7, LSL #2				; Shift left 2x so 6 LSB aligns with pins A2-7
		STR R8, [R0, #0x03FC]
		
		; Write the data to GPIO_PORTB
		LDR R0, =GPIO_PORTB
		MVN R8, R7, LSR #6				; Shift right 6x so 4 MSB aligns with pins B0-3
		STR R8, [R0, #0x03FC]
		
		POP {R8}
		POP {R0}
		BX LR
		
	ALIGN
	END

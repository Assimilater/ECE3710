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
	; F0 is SW2 (right) -> Player 2
	; F4 is SW1 (left)	-> Player 2
	
		; Turn on the clock
		LDR R0, =GPIO_CLOCK
		MOV R1, #0x23					; 0x23 = 0b10.0011 (ports a b and f)
		STR R1, [R0]
		
		; Load the GPIO unlock code
		LDR R2, =GPIO_UNLOCK			; in hindsight, I think we only need it for port F
		
		
		; Port F
		LDR R0, =GPIO_PORTF
		STR R2, [R0, #0x520]			; Unlock Port F
		
		MOV R1, #0x1F					; Configure pins 0-4
		STR R1, [R0, #0x524]			; Set CR to limit which bits are modified on write
		STR R1, [R0, #0x510]			; Set Pull-Up Select
		STR R1, [R0, #0x51C]			; Set Digital Enable
		STR R1, [R0, #0x400]			; Configure pins as input
		
		
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
		
		
	; R0 stores a port address
	; R1 tracks player 1
	; R2 tracks player 2
	; R3 stores player 1's speed
	; R4 stores player 2's speed
	; R5 stores the button state
	; R6 stores state as enum {
	;	0 => Inactive,
	;	1 => Player 1 Ready,
	;	2 => Player 2 Ready,
	;	3 => Stand off
	;	4 => Player 1 Advance,
	;	5 => Player 2 Advance,
	;	6 => Finish
	;}
	; R7+ stores temporary values and calculations
		
Program
		; Init Player Position and State
		MOV R1, #0x20					;Binary 0010 0000 - Player 1 start place
		MOV R2, #0x10					;Binary 0001 0000 - Player 2 start place
		MOV R6, #0

Ready
		; Sets R7 for "on" half of blinking
		ORR R7, R1, R2
		BL LED
		
		; TIMER is needed to be used here
		; Check for switch press
		
		; Sets R7 for the "off" half of the blinking
		CMP R6, #0
		MOVEQ R7, #0
		CMP R6, #1
		MOVEQ R7, R1
		CMP R6, #2
		MOVEQ R7, R2
		BL LED
		
		; Wait for TIMER again
		; Still check for switch presses
		
		CMP R6, #3
		BNE Ready
		
GetSpeed
		LDR R0, =GPIO_PORTB
		LDR R4, [R0]					; Get speed values
		
		MOV R7, #3						; Select 2 bits
		AND R3, R7, R4, LSR #6			; put PB6 and 7 as Player 1 speed
		AND R4, R7, R4, LSR #4			; put PB4 and 5 as Player 2 speed
		
		
PushBack
		; wait Random Time
		
		LSL R1, #1
		LSR R2, #1
		ORR R7, R1, R2
		BL LED
		
		
		LDR R0, =GPIO_PORTF
SpeedDuel
		LDR R7, [R0]
		CMP R7, #0
		BEQ SpeedDuel
		
		; Check if SW1 is pressed
		CMP R7, #0x8
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
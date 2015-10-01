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

;LED_MAX		EQU 0x03FF				; 10 bits of 1's is the max value you can store on the LED
PCMS_500		EQU 0x00053FD4			; Program cycles for 1/2 second (used to time our LED clock)
PCMS_001		EQU 0x0000FFFF			; Program cycles for approx. 1ms (used to time our button presses)

Start
	; Enable ports A, B, and F
	; A[2-7] and B[0-3] correspond to the 10 LEDS
	; F0 is SW2 (right) -> Stop counter
	; F2 is external 	-> Reset counter (disable alternate functionality)
	; F4 is SW1 (left)	-> Start counter
	
		; Turn on the clock
		LDR R0, =GPIO_CLOCK
		MOV R1, #0x23					; 0x23 = 0b10.0011 (ports a b and f)
		STR R1, [R0]
		
		; Load the GPIO unlock code
		LDR R2, =GPIO_UNLOCK			; in hindsight, I think we only need it for port F
		
		
		; Port F
		LDR R0, =GPIO_PORTF
		STR R2, [R0, #0x520]			; Unlock Port F
		
		;MOV R1, #0x15					; Configure pins 0, 2, and 4
		;STR R1, [R0, #0x524]			; Set CR to limit which bits are modified on write
		;STR R1, [R0, #0x510]			; Set Pull-Up Select
		;STR R1, [R0, #0x51C]			; Set Digital Enable
		
		;MOV R1, #0
		;STR R1, [R0, #0x400]			; Configure pins as input
		
		;MOV R1, #0x15					; We only want to disable pin 2's alternate functionality
		;STR R1, [R0, #0x420]			; Disable Alternate Functionality
		
		
		MOV R1, #0x1F					; Configure pins 0-4
		STR R1, [R0, #0x524]			; Set CR to limit which bits are modified on write
		STR R1, [R0, #0x510]			; Set Pull-Up Select
		STR R1, [R0, #0x51C]			; Set Digital Enable
		
		MOV R1, #0x0A
		STR R1, [R0, #0x400]			; Configure pins as input/output
		
		;MOV R1, #0x15					; We only want to disable pin 2's alternate functionality
		;STR R1, [R0, #0x420]			; Disable Alternate Functionality
		
		
		; Port B
		LDR R0, =GPIO_PORTB				; Unlock Port B
		STR R2, [R0, #0x520]
		
		MOV R1, #0x0F					; We are configuring only the last 4 bits of Port B
		STR R1, [R0, #0x524]			; Set CR to limit which bits are modified on write
		STR R1, [R0,#0x400]				; Configure pins as output
		STR R1, [R0, #0x510]			; Set Pull-Up Select
		STR R1, [R0,#0x51C]				; Set Digital Enable
		
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
		
		
	; Use R0 as the binary value for the counter
	; Use R1 as a 2Hz counter
	; Use R2 as a 1ms counter for buttons
	; Use R3 to track the state of the LED clock
	; Use R4 to store which button is being pressed (GPIO F)
	; Use R5 to load the address of a port we are writing to or reading from
	; Use R6 for calculations or as a temporary register
		
		MOV R0, #0						; Reset the clock
		LDR R1, =PCMS_500				; Reset the clock timer
		LDR R2, =PCMS_001				; Reset the button timer
		MOV R3, #0x15					; Program starts with buttons off and green light on
		B ClkStart						; Conveniently set the R3 and the LED to start with the clock on
		
Program
		; Check state of buttons
		LDR R5, =GPIO_PORTF
		LDR R6, [R5, #0x03FC]			; Fetch the data from port F
		AND R6, #0x15					; Ignore LED state in decisions with buttons
		CMP R3, R6						; Compare it with the button state we have stored
		BNE Buttons						; Set the timer and update R3 if they are different
		
		; If the timer is not yet to zero, jump to the clock, otherwise act on button input
		CMP R2, #0
		SUBNE R2, R2, #1
		BNE Clock
		
		; Check valid button states
		CMP R3, #0x05
		BEQ ClkStart
		
		CMP R3, #0x14
		BEQ ClkStop
		
		CMP R3, #0x11
		BNE Clock
		
ClkReset
		MOV R0, #0x400
		B LED
ClkStop
		MOV R4, #0
		
		LDR R5, =BAND_DATAF
		
		; Turn Green Light Off - Pin 3
		MOV R6, #0
		STR R6, [R5, #12]				; Band Address + 4*3 (bit accessed)
		
		; Turn Red Light On - Pin 1
		MOV R6, #1
		STR R6, [R5, #4]				; Band Address + 4*1 (bit accessed)
		
		B Program
ClkStart
		MOV R4, #1
		
		LDR R5, =BAND_DATAF
		
		; Turn Green Light On - Pin 3
		MOV R6, #1
		STR R6, [R5, #12]				; Band Address + 4*3 (bit accessed)
		
		; Turn Red Light Off - Pin 1
		MOV R6, #0
		STR R6, [R5, #4]				; Band Address + 4*1 (bit accessed)
		
		B Clock
	
Buttons
		MOV R3, R6
		LDR R2, =PCMS_001
		
Clock
		CMP R4, #1						; Exit out if clock is disabled
		BNE Program
		
		SUBS R1, R1, #1					; Reduce the LED timer
		BNE Program
		
		CMP R0, #0						; Check for overflow
		MOVEQ R0, #0x400				; Reset the LED Clock
		LDR R1, =PCMS_500				; Reset the LED Timer
		
		SUB R0, R0, #1					; Reduce the LED count by 0
		
LED
		; Write the data to GPIO_PORTA
		LDR R5, =GPIO_PORTA
		MOV R6, R0, LSL #2				; Shift left 2x so 6 LSB aligns with pins A2-7
		STR R6, [R5, #0x03FC]
		
		; Write the data to GPIO_PORTB
		LDR R5, =GPIO_PORTB
		MOV R6, R0, LSR #6				; Shift right 6x so 4 MSB aligns with pins B0-3
		STR R6, [R5, #0x03FC]
		
		B Program
		
	ALIGN
	END
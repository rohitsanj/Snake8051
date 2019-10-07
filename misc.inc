; Constants and data definitions
ROW 					EQU P1
COL 					EQU P2

REG_1 					EQU 01H
REG_0 					EQU 00H
	
X 						EQU 02H
Y 						EQU 03H
	
DIS_BUF_UND_FLOW_ADDR 	EQU 1FH
DIS_BUF_START_ADDR 		EQU 20H
DIS_BUFF_END_ADDR		EQU 27H		
DIS_BUF_OVR_FLOW_ADDR 	EQU 28H		

X_MASK 					EQU 00111000B
Y_MASK 					EQU 00000111B	
	
SNAKE_ARRAY_START_ADDR 	EQU 40H
SNAKE_LENGTH 			EQU 02H


;---------------------
_paintScreen:
; this function prints the bytes present from 20 to 27H
; before printing, the byte is complemented and given to LED Matrix
	MOV R0, #10
	back: PUSH REG_0
	MOV R1, #8
	MOV R0, #DIS_BUF_START_ADDR
	MOV A, #1
	CLR C
	up: MOV ROW, A
	RL A
	PUSH 0E0H
	MOV A, @R0
	CPL A
	MOV COL, A
	POP 0E0H
	ACALL DELAY
	INC R0
	DJNZ R1, up
	POP REG_0
	DJNZ R0, back
RET

; --------------------
delay: MOV TMOD, #01H
	MOV TH0, #0F0H
	MOV TL0, #000H
	SETB TR0
	loop: JNB TF0, loop
	CLR TR0
	CLR TF0
RET
;----------------------
; Arguments: A
; Returns: X, Y

_X_Y_convert_A:
PUSH 0E0H
ANL A, #X_MASK
RR A
RR A
RR A
MOV X, A

POP 0E0H
ANL A, #Y_MASK
MOV Y, A
RET
; ---------------------

; Function: 	_display_X_Y
; Arguments: 	X, Y
; Description: 
; 	This function takes in (X,Y) and sets the particular bit
; 	For example, if (X,Y) = (1,2) then DIS_BUF_START_ADDR + Y = 22H
; 	Since X = 1, now set the 1st bit from the left
_display_X_Y:
	MOV A, #DIS_BUF_START_ADDR
	ADD A, Y
	MOV R0, A
	MOV A, #80H
	rotate_right: RR A
	DJNZ X, rotate_right
	MOV R1, A

	MOV A, @R0
	ORL A, R1

	MOV @R0, A
RET
; ------------------

; Function: 	_printSnake
; Arguments:	Array of length SNAKE_LENGTH starting from memory location 40H
; Description:
;	Loops through array and prints the location of each byte


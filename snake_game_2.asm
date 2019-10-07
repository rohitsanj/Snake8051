ORG 0000H
ACALL MAIN
ORG 0003H
$INCLUDE(misc.inc)
; ----------------------------------------------

MAIN: 
MOV A, #00001000B
ACALL _X_Y_convert_A
ACALL _display_X_Y
ACALL _paintScreen
SJMP $
end
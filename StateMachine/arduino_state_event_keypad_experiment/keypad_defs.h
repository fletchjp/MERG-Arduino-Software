// keypad_defs.h

// This defines the keyboard layout

ALIAS_C_BYTE(ROWS,4)
ALIAS_C_BYTE(COLS,4)

ALIAS_UINT8(rows,ROWS)
ALIAS_UINT8(cols,COLS)

LAYOUT(layout,"123A456B789C*0#D")

BYTE_ARRAY_4(rowPins,ROWS,9,8,7,6)
BYTE_ARRAY_4(colPins,COLS,5,4,3,2)

ALIAS_BYTE(key_pressed,'Z')

# byte code

### Legend:

- \_INSTR\_<...>\_ - instruction markers, always size of 1B

## block:

_INSTR_STARTBLOCK_  
...CONTENT  
_INSTR_ENDBLOCK_

## function call:

_INSTR_CALL_  
{4B} table id index of function id

## operation:

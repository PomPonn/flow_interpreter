## ast nodes

### root:

- first node of the compilation
- _no data_
- _children_:
  first child is a root block,
  may contain meta data too

### block:

- _no data_
- _children_:
  block's nested nodes

### literal:

- _data_: literal type
- _no children_

### id chain:

- _data_: vector of chained id's
- _no children_

### operator:

- _data_: operator order
- _children_:
  first one is an operator id chain,
  second and third are the arguments

### call:

- _no data_
- _children_:
  first one is a function id chain,
  next are arguments of varying node type

## byte code

### Example byte code:

_INSTR_START
\_INSTR_STARTBLOCK_

_INSTR_ENDBLOCK_

#### Legend:

- \_INSTR\_<...>\_ - instruction markers, always size of 1B

### block:

_INSTR_STARTBLOCK_  
...CONTENT  
_INSTR_ENDBLOCK_

### function call:

_INSTR_CALL_  
{4B} table id index of function id

### operation:

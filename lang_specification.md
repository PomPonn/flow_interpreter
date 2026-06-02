## ast nodes

### root:

- *_data_*: none
- _children_:
  first child is a main program block,
  may contain meta data too (?)

### block:

- _data_: none
- _children_: nested nodes

### literal:

- _data_: literal value (fl_value)
- _children_: none

### id:

- _data_: id path (fl_id_chain)
- _children_: none

### operator:

- _data_: operator order (?)
- _children_:
  - #1: operation id,
  - #2&3: operation arguments

### call:

- _data_: none
- _children_:
  first one is a function id chain,
  next are arguments of varying node type

## byte code

#### Legend:

- _[INSTR]_ - instruction marker, always size of 1B

### Header section:

```
[HEADER SECTION]
(version): 10B
--wip:
(compilation timestamp): xB
```

### Literals section:

[LITERALS SECTION]
(count): 4B
... <count> times: {
(value type): 1B
(value data): xB
}

### Code section:

[CODE SECTION]
... x instructions { ... }

### Code section instructions:

#### id access:

(offset): 4B
(indices count): 4B
... <indices count> times: {
(index): 4B
}

#### getters/setters:

##### get id:

[GET ID]
<id access>

#### new id:

[NEW ID]
???

##### get literal:

[GET LITERAL]
(index): 4B

#### scope:

[BLOCK START] 
... content
[BLOCK END]

#### call:

[CALL]
<id access>
... arguments
[ARGS END]

#### callable instructions:

(
|ADD|
|IMPORT|
|ASSIGN|
)
... arguments
[ARGS END]
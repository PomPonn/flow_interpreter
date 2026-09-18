# Language specification

## ast nodes

### block:

- _data_: none
- _children_: nested nodes

### literal:

- _data_: literal value (fl_value)
- _children_: none

### id:

- _data_: id chain (fl_id_chain)
- _children_: none

### operator:

- _data_: is grouped flag (fl_bool)
- _children_:
    - #1: operation id,
    - #2&3: operation arguments

### call:

- _data_: none
- _children_:
  first one is a function id chain,
  then n arguments

## byte code

#### Legend:

- `[<INSTR>]` - instruction marker, always size of 1B

### Header section:

```
[HEADER SECTION]
(version): 10B + 1B for '\0' (end of string)
(compilation timestamp): 8B
```

### Literals section (optional):

```
[LITERALS SECTION]
(lits count): 4B
... <count> times: {
(value type): 1B
(value data): xB
}
```

### Code section:

```
[CODE SECTION]
... x instructions { ... }
```

### Code section instructions:

#### id access by indices:

```
(offset): 4B
(indices count): 4B
... <indices count> times: {
(index): 4B
}
```

#### id access by names:

```
(id chain): xB (string, names separated with dots)
```

#### getters/setters:

##### get id:

```
[ID]
<id access>
```

#### id declaration:

```
[DECL]
(id name): xB (string)
```

#### get literal:

```
[LITERAL]
(literal table index): 4B
```

#### block:

```
[BLOCK START]
... content
[BLOCK END]
```

#### call:

```
[CALL]
<id access>
... arguments
[ARGS END]
```

#### callable instructions:

```
|(ADD | IMPORT |ASSIGN)|
... arguments
[ARGS END]
```

### special values

#### function

```
(parameters count) 4B
... <parameters count> times: {
(parameter label) xB (string)
}
... body block
```

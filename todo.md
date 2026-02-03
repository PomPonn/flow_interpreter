# todo

- think if we can use only single id ast node instead of id chain if there is no chaining
- cleanup ast generator
- cleanup tokenizer
- abstract syntax tree generation
    - expression & operators handling
    - unused tokens handling
- implement time measurement on unix systems
- remember to implement convertion of literal info data field to appropiate type

# done

- fix optimalizer space handling
- revise delimeters (it seems like some of them will be operators)
- write profiler
- new lines are not counted when creating comment and string tokens (tokenizer)
- whitespace tokenizer context member makes no sense
- make wrappers single character, not string, to apply some optimalization (since we leave universality)
- remove operators and keyword parsing from tokenizer (we have no predefined stuff)
- revise delimeters (reduce them to minimum)
- work on tokenizer - optimalization, string escape, etc.
- write text optimalizer (include whitespace, but reduce it to minimum)
- cleanup tokenizer
- (optimalization) consider use of inline functions

# Q&A

- how to handle types? Are they predefined?
    - Yes, they are. a use of type is essentially a id chain and (optionally) dimensions qualifier
    - We won't now in advance if use of type should be next
- How to represent language structures in interpreter code?

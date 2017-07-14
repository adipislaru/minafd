# AFN to AFD

This is a simple tool made for a homework at University that transforms AFD automata to AFD automata equivalent and with minimum number of states.

# Info
- States should be integers
- The initial state will exist and will be 0 
- The final states will have values greater than others
- The alphabet will contain a-z A-Z

# Input
- On the first line, 3 integers: **n** ( number of states ), **nf** ( number of final states ) and **s** ( number of simbols in the alphabet )
- On the second line, a sequence of **nf** integer values ( the final states )
- On the next lines, values **st** ( current state ), **c** ( character ) and **end** ( next state ).

# Output
- An AFD equivalent and with minimum number of states
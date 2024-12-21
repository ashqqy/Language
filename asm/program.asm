push 5
pop [0]
push 1
pop [1]
0:
push [0]
push 1
jne 1:
jmp 2:
1:
push [1]
push [0]
mul
pop [1]
push [0]
push 1
sub
pop [0]
jmp 0:
2:
push [1]
out
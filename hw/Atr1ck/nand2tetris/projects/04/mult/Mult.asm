// RAM[2] = RAM[0] * RAM[1]
// mul = 0
// i = 0
// n = RAM[1]
// while (i < n) mul = mul + RAM[0]
// RAM[2] = mul

@mul
M=0

@i
M=0

@R1
D=M

@n
M=D //n=RAM[1]    

(LOOP)
@i
D=M
M=M+1

@n
D=D-M

@STOP
D;JEQ

@R0
D=M

@mul
M=D+M

@LOOP
0;JMP

(STOP)
@mul
D=M

@R2
M=D


(END)
@END
0;JMP
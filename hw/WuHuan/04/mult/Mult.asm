// RAM[2] = RAM[0] * RAM[1]
// 
// n = RAM[1]
// i = 0
// RAM[2] = 0
// while (i < n): RAM[2] += RAM[0]

    @R1
    D=M

    @n
    M=D

    @i
    M=0

    @R2
    M=0

(LOOP)
    @i
    D=M

    @n
    D=D-M

    @END
    D;JEQ


    @R0
    D=M

    @R2
    M=M+D

    @i
    M=M+1

    @LOOP
    0;JMP


(END)
    @END
    0;JMP
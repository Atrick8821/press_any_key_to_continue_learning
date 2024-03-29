// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen
// by writing 'black' in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen by writing
// 'white' in every pixel;
// the screen should remain fully clear as long as no key is pressed.

//// Replace this comment with your code.
// screenaddr = SCREEN
// CHECK KBD
// if KBD != 0 BLACK
// else WHITE

(LOOP)
    @SCREEN
    D=A

    @screenaddr
    M=D


    @KBD
    D=M

    @WHITE
    D;JEQ

(BLACK)
    @i
    M=0
(BLACKLOOP)
    @KBD
    D=A
    
    @screenaddr
    D=D-M

    @LOOP
    D;JEQ

    @screenaddr
    A=M
    M=-1

    @screenaddr
    M=M+1

    @BLACKLOOP
    0;JMP

    
(WHITE)
    @i
    M=0
    (WHITELOOP)
    @KBD
    D=A

    @screenaddr
    D=D-M

    @LOOP
    D;JEQ

    @screenaddr
    A=M
    M=0

    @screenaddr
    M=M+1

    @WHITELOOP
    0;JMP
    
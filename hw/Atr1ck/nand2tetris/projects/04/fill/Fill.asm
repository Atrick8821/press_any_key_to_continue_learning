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
@SCREEN
D=A

@screenaddr
M=D

@KBD
D=A

@kbdaddr
M=D


(LOOP)
@KBD
D=M

@i
M=0

@LOOPWHITE
D;JEQ

(LOOPBLACK)
@screenaddr
D=M

@i
D=D+M

@kbdaddr
D=D-M

@LOOP
D;JEQ

@screenaddr
D=M

@i
A=D+M
M=-1

@1
D=A

@i
M=D+M

@LOOPBLACK
0;JMP

(LOOPWHITE)
@screenaddr
D=M

@i
D=D+M

@kbdaddr
D=D-M

@LOOP
D;JEQ

@screenaddr
D=M

@i
A=D+M
M=0

@1
D=A

@i
M=D+M

@LOOPWHITE
0;JMP
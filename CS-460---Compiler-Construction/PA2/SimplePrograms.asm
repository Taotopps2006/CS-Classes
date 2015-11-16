; Author: Tim Kwist
; Date: 2 September 2015
; Description: Compiler Construction 460 - Dr. Frederick C. Harris, University of Nevada, Reno. Assignment 2, simple programs in MIPS
.data
helloWorld: .asciiz "Hello, World!\n"
initialFactorial: .asciiz "Calculating the factorial of: "

.text
.ent main
main:
jal HelloWorld
la $a0, initialFactorial  # Load string to print
li $v0, 4                 # '4' tells syscall to print string
syscall                   # Print initial factorial message
li $v0, 5                 # '5' tells syscall to read integer
syscall                   # Get factorial number from input
move $a0, $v0             # $a0 = $v0; load inputed integer into argument register for IterativeFactorial function
jal IterativeFactorial    # $v0 = fact($a0)
move $a0, $v0             # $a0 = $v0; load calculated factorial into argument register for output
li $v0, 1                 # '1' tells syscall to print intger
syscall                   # Print integer
li $v0, 10                # Tell the system we're ready to terminate (10 -> terminate)
syscall                   # Terminate program
.end main

.ent HelloWorld
HelloWorld:
la $a0, helloWorld        # Load our string into the first argument register
li $v0, 4                 # '4' tells syscall to print string
syscall                   # Print 'Hello, World!'
jr $ra                    # Return to where we were called
.end HelloWorld

.ent IterativeFactorial
IterativeFactorial:       # Expects an integer n in $a0 to calculate the factorial of; Calculates up to factorial of 12
move $v0, $a0             # Prep return value with initial value n
loop:
subi $a0, $a0, 1          # Subtract 1 from current value of n
mul $v0, $v0, $a0         # $v0 holds our (current n + 1) * (current n + 2) ... (original n); multiply that by current n
bgt $a0, 1, loop          # If current n > 1, continue loop
jr $ra                    # Otherwise, return to where we were called
.end IterativeFactorial

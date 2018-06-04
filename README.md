# dense_coding_in_c
Author: Josh Wolff
06/04/2018
This code performs a simple quantum dense coding operation simulation using 2 qubits.

----CODE MECHANICS----
The code in quantum.c uses several modules that I developed in CS107E. This allows me to
interface with the Raspberry Pi easily. 

----CODE LIMITATIONS----
I attempted to find patterns in the output of a dense coding operation, but the only files
I found regarding a 3 qubit dense coding operation were vague and did not match up with
the textbook. Due to time constraints, I had to move and hard-code a 2 qubit dense coding
operation. In the future, it would be cool to implement the actual math behind a >2 dense
coding operation.

----INSTRUCTION GUIDE----
The prompts on the screen largely lead the user. First, one must type in the "dense" command
defined in shell.c (not in this repository). Then the user must enter '2' because only 2
qubits are supported (see above). Then the user should follow the instructions on the screen.
Incorrect input is not supported - thus, the user cannot be led astray. The operation will
work every time as long as the user registers the proper buttons. Only the correct buttons in
the correct order are acknowledged on the screen.

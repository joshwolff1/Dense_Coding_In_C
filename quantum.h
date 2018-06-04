#ifndef QUANTUM_H
#define QUANTUM_H

/*
 * Interface to for a simple dense coding operation.
 *
 * Author: Josh Wolff <jw1@cs.stanford.edu>
 * Date: June 4 2018
 */

// Find the tensor product of two matrices
void dense_coding_op_init(int qubits);
// i.e. apply_operator('Z', 'A'); or apply_operator('X', 'B');
int apply_operator(char op, char person);

#endif

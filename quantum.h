#ifndef QUANTUM_H
#define QUANTUM_H

/*
 * Interface to for a simple dense coding operation.
 *
 * Author: Josh Wolff <jw1@cs.stanford.edu>
 * Date: June 4 2018
 */

/*
 * Runs a basic dense coding operation with the specified amount of qubits.
 * Made for range of 2 to 3 qubits.
 * Returns -1 if operation did not initialize or complete successfully.
*/
int dense_coding_op(void);

#endif

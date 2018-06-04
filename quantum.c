#include "quantum.h"
#include "malloc.h"
#include "printf.h"
#include "shell.h"
#include "strings.h"

/*
 * Author: Josh Wolff
 * Uses buttons as an interface.
*/
static int (*shell_printf)(const char * format, ...);

// MARK:- GLOBAL VARIABLES

volatile static void *state_start = NULL;
volatile static int qubits = 0;
volatile static char current_op = '_';

/*
 * Change after a single button press. Only changed in the complete_alice_operation(...)
 * and in the complete_bob_operation(...) operation.
*/
volatile static int alice_op_completed = 0;
volatile static int bob_cnot_completed = 0;
volatile static int bob_h_completed = 0;


// MARK:- CONSTANTS
static const int MIN_QUBITS = 2;
static const int MAX_QUBITS = 3;
static const int LINE_LEN = 80;

static const char ALICE = 'A';
static const char BOB = 'B';

static const char Z_OP = 'Z';
static const char X_OP = 'X';
static const char I_OP = 'I';
static const char Y_OP = 'Y';

static const char CNOT_OP = 'C';
static const char H_OP = 'H';


// MARK:- PRIVATE HELPER FUNCTIONS

/*
 * Prints the current state of the system.
*/
void print_state() {
  // can print based on completed global values
}

/*
 * Checks if the operation inputted is a valid operation for the specified person.
 * Returns 1 if successful. 0 if not.
*/
int is_valid_op(char op, char person) {
  if (person == ALICE) {
    if (op == Z_OP || op == X_OP || op == I_OP || op == Y_OP) {
      return 1;
    }
  } else if (person == BOB) {
    if (op == CNOT_OP || op == H_OP) {
      return 1;
    }
  }
  return 0;
}

void tensor_product() {

}

void initialize_state() {
  // initialize as a bell state
}

/*
 * Applies an operator to the system. Note that this function is made specifically
 * for a dense coding operation. Therefore, the person whose qubits the operation
 * is being applied to does not matter because this can be inferred from the
 * operation itself. X, Y, Z, I apply only to Alice. CNOT and H apply only to Bob.
 *
 * Called when button is pressed.
 * Applies U x I ^ (qubits) if Alice's qubit or I ^ (qubits) x U if Bob's qubit.
*/
int apply_operator(char op) {

  print_state();
}

/*
 * Completes Alice's respective operation on the system for a dense coding
 * simulation. This consists of one application of U x I ^ (# of qubits).
*/
void complete_alice_operation(void) {
  shell_printf("\nFirst, encode the desired value by transforming Alice's qubit.");
  shell_printf("\nThis can be done by pressing X, Y, Z, or I.");
  shell_printf("\nAlice can transform using U tensor I ^ (# of qubits).");
  while(1) {
    if (is_valid_op(current_op, ALICE)) {
      if (!alice_op_completed) {
        apply_operator(current_op);
        alice_op_completed = 1;
        print_state();
        break;
      }
    }
  }
}

/*
 * Completes Bob's respective operation on the system for a dense coding
 * simulation. This consists of one application of I ^ (# of qubits) x U.
*/
void complete_bob_operation(void) {
  shell_printf("\nGreat! Now complete Bob's operation by pressing the CNOT gate followed by the H gate.");
  shell_printf("\nThis can be done by pressing X, Y, Z, or I.");
  shell_printf("\nBob can transform using I ^ (# of qubits) tensor U.");
  while(1) {
    if (is_valid_op(current_op, BOB)) {
      if (current_op == CNOT_OP && !bob_cnot_completed) {
        apply_operator(current_op);
        bob_cnot_completed = 1;
        print_state();
      } else if (current_op == H_OP && !bob_h_completed && bob_cnot_completed) {
        apply_operator(current_op);
        bob_h_completed = 1;
        print_state();
        break;
      }
    }
  }
}

/*
 * Requests the amount of qubits to initialize the state.
 * Returns 1 if successful. 0 if user quit.
*/
int request_amt_of_qubits(void) {
  while (1) {
    char line[LINE_LEN]; // LINE_LEN defined in shell.h
    shell_printf("\nEnter amount of qubits desired: ");
    shell_readline(line, sizeof(line));
    if (strlen(line) == 1) {
      const char **endptr = NULL;
      if (line[0] == 'q' || line[0] == 'Q' || line[0] == 27 /*escape ASCII key*/) {
        shell_printf("\nQuiting...");
        return 0;
      } else if (strtonum(line, endptr) >= MIN_QUBITS && strtonum(line, endptr) <= MAX_QUBITS) {
        qubits = strtonum(line, endptr);
        state_start = malloc(qubits);
        return 1;
      } else {
        shell_printf("\nError: Invalid amount of qubits entered.");
      }
    } else {
      shell_printf("\nError: Invalid amount of characters entered.");
    }
  }
}



// MARK:- PUBLIC FUNCTIONS

/* Find the tensor product of two matrices
 * Returns -1 if failed. 0 if successful.
*/
int dense_coding_op(void) {
  shell_printf("\f");
  shell_printf("Welcome! Let's perform a simple dense coding operation simulation!");
  if (!request_amt_of_qubits()) return -1;
  initialize_state();
  print_state();
  complete_alice_operation();
  complete_bob_operation();
  shell_printf("\nCongratulations! You have decoded the value!");
  print_state();
  free(state_start);
  return 0;
}

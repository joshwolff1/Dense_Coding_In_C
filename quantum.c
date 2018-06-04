#include "quantum.h"
// #include "malloc.h"
#include "printf.h"
#include "shell.h"
#include "strings.h"
#include "gpio.h"
#include "uart.h"
#include "gpioextra.h"
#include "interrupts.h"
#include "assert.h"

/*
 * Author: Josh Wolff
 * Uses buttons as an interface.
*/
//static int (*printf)(const char * format, ...);

// MARK:- GLOBAL VARIABLES

// volatile static void *state_start = NULL;
volatile static int qubits = 0;

/*
 * Change after a single button press. Only changed in the complete_alice_operation(...)
 * and in the complete_bob_operation(...) operation.
*/
volatile static int alice_op_completed = 0;
volatile static int bob_cnot_completed = 0;
volatile static int bob_h_completed = 0;
volatile static int current_op = '_';
volatile static int alice_op_chosen = '_';


// MARK:- CONSTANTS
static const int MIN_QUBITS = 2;
static const int MAX_QUBITS = 2;
static const int LINE_LEN = 80;

static const char ALICE = 'A';
static const char BOB = 'B';

static const char Z_OP = 'Z';
static const char X_OP = 'X';
static const char I_OP = 'I';
static const char Y_OP = 'Y';
static const char CNOT_OP = 'C';
static const char H_OP = 'H';

static const char TENSOR_SYM = '*';

static const int I_OP_BUTTON = GPIO_PIN27;
static const int X_OP_BUTTON = GPIO_PIN26;
static const int Y_OP_BUTTON = GPIO_PIN25;
static const int Z_OP_BUTTON = GPIO_PIN22;

static const int CNOT_OP_BUTTON = GPIO_PIN21;
static const int H_OP_BUTTON = GPIO_PIN20;


// MARK:- PRIVATE HELPER FUNCTIONS

/*
 * Print the final state...
 * Applies a CNOT gate.
*/
void print_final_state_helper(void) {
  printf("\nAnd the measured state is...");
  if (alice_op_chosen == I_OP) {
    printf("\n|00>");
  } else if (alice_op_chosen == X_OP) {
    printf("\n|01>");
  } else if (alice_op_chosen == Z_OP) {
    printf("\n|10>");
  } else if (alice_op_chosen == Y_OP) {
    printf("\n|11>");
  }
}

/*
 * Helper function for print_state(...)
 * Applies a CNOT gate.
*/
void print_bob_state_helper(void) {
  printf("\nCNOT: Flipping the second bit if the first is 1...");
  if (alice_op_chosen == I_OP) {
    printf("\n(1/sqrt(2))(|00> + |10>)");
    printf("\n = ");
    printf("\n(1/sqrt(2))(|0> + |1>) %c |0>", TENSOR_SYM);
  } else if (alice_op_chosen == X_OP) {
    printf("\n(1/sqrt(2))(|11> + |01>)");
    printf("\n = ");
    printf("\n(1/sqrt(2))(|1> + |0>) %c |1>", TENSOR_SYM);
  } else if (alice_op_chosen == Z_OP) {
    printf("\n(1/sqrt(2))(|00> - |10>)");
    printf("\n = ");
    printf("\n(1/sqrt(2))(|0> - |1>) %c |0>", TENSOR_SYM);
  } else if (alice_op_chosen == Y_OP) {
    printf("\n(1/sqrt(2))(-|11> + |01>)");
    printf("\n = ");
    printf("\n(1/sqrt(2))(-|1> + |0>) %c |1>", TENSOR_SYM);
  }
}

/*
 * Helper function for print_state(...)
*/
void print_alice_state_helper(void) {
  printf("\nAPPLYING: (%c %c I)(1/sqrt(2))(|00> + |11>)...", alice_op_chosen, TENSOR_SYM);
  if (alice_op_chosen == I_OP) {
    printf("\n(1/sqrt(2))(|00> + |11>)");
  } else if (alice_op_chosen == X_OP) {
    printf("\n(1/sqrt(2))(|10> + |01>)");
  } else if (alice_op_chosen == Z_OP) {
    printf("\n(1/sqrt(2))(|00> - |11>)");
  } else if (alice_op_chosen == Y_OP) {
    printf("\n(1/sqrt(2))(-|10> + |01>)");
  }
}

/*
 * Prints the current state of the system.
*/
void print_state() {
  if (alice_op_completed) {
    if (bob_cnot_completed) {
      if (bob_h_completed) {
        print_final_state_helper();
      } else {
        print_bob_state_helper();
      }
    } else {
      print_alice_state_helper();
    }
  } else {
    printf("\n(1/sqrt(2))(|00> + |11>)");
  }
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

/*
 * Applies a tensor product to two matrices.
*/
void tensor_product(void) {
  // Hard coded
}

/*
 * Initializes the state as an EPR pair.
*/
void initialize_state() {
  // Hard coded
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
  // Hard coded
  return 0;
}

/*
 * Completes Alice's respective operation on the system for a dense coding
 * simulation. This consists of one application of U x I ^ (# of qubits).
 * Need to apply operation to each qubit. Alice manipulates qubits - 1.
 * This is very helpful in understanding why: https://arxiv.org/pdf/quant-ph/0105096v1.pdfs
*/
void complete_alice_operation() {
  printf("\nFirst, Alice must encode the desired value by transforming her qubit.");
  printf("\nThis can be done by pressing X, Y, Z, or I.");
  // printf("\nAlice can transform using U tensor I ^ (# of qubits).");
  while(1) {
    if (is_valid_op(current_op, ALICE)) {
      if (!alice_op_completed) {
        apply_operator(current_op);
        alice_op_completed = 1;
        alice_op_chosen = current_op;
        print_state();
        break;
      }
    }
    if (alice_op_completed) break;
  }
}

/*
 * Completes Bob's respective operation on the system for a dense coding
 * simulation. This consists of one application of I ^ (# of qubits) x U.
*/
void complete_bob_operation() {
  printf("\nGreat, Alice! Bob, now comlete your operation by pressing the CNOT gate followed by the H gate.");
  printf("\nThis can be done by pressing CNOT followed by H.");
  // printf("\nYou can transform using I ^ (# of qubits) tensor U.");
  while(1) {
    if (is_valid_op(current_op, BOB)) {
      if (current_op == CNOT_OP && !bob_cnot_completed) {
        apply_operator(current_op);
        bob_cnot_completed = 1;
        print_state();
        printf("\nNice, Bob! Now apply the Hadamard transformation and measure the two qubits.");
      } else if (current_op == H_OP && !bob_h_completed && bob_cnot_completed) {
        apply_operator(current_op);
        bob_h_completed = 1;
        break;
      }
    }
    if (bob_h_completed) break;
  }
}

/*
 * Requests the amount of qubits to initialize the state.
 * Returns 1 if successful. 0 if user quit.
*/
int request_amt_of_qubits(void) {
  while (1) {
    char line[LINE_LEN]; // LINE_LEN defined in shell.h
    printf("\nEnter amount of qubits desired: ");
    shell_readline(line, sizeof(line));
    if (strlen(line) == 1) {
      const char **endptr = NULL;
      if (line[0] == 'q' || line[0] == 'Q' || line[0] == 27 /*escape ASCII key*/) {
        printf("\nQuiting...");
        return 0;
      } else if (strtonum(line, endptr) >= MIN_QUBITS && strtonum(line, endptr) <= MAX_QUBITS) {
        qubits = strtonum(line, endptr);
        // state_start = malloc(qubits);
        return 1;
      } else {
        printf("\nError: Invalid amount of qubits entered.");
        printf("\nMinimum Qubits Allowed: %d", MIN_QUBITS);
        printf("\nMaximum Qubits Allowed: %d", MAX_QUBITS);
      }
    } else {
      printf("\nError: Invalid amount of qubits entered.");
      printf("\nMinimum Qubits Allowed: %d", MIN_QUBITS);
      printf("\nMaximum Qubits Allowed: %d", MAX_QUBITS);
    }
  }
}

static void button_press(unsigned int pc) {
  if (gpio_check_event(Z_OP_BUTTON)) {
    gpio_clear_event(Z_OP_BUTTON);
    current_op = Z_OP;
  } else if (gpio_check_event(X_OP_BUTTON)) {
    gpio_clear_event(X_OP_BUTTON);
    current_op = X_OP;
  } else if (gpio_check_event(I_OP_BUTTON)) {
    gpio_clear_event(I_OP_BUTTON);
    current_op = I_OP;
  } else if (gpio_check_event(Y_OP_BUTTON)) {
    gpio_clear_event(Y_OP_BUTTON);
    current_op = Y_OP;
  } else if (gpio_check_event(CNOT_OP_BUTTON)) {
    gpio_clear_event(CNOT_OP_BUTTON);
    current_op = CNOT_OP;
  } else if (gpio_check_event(H_OP_BUTTON)) {
    gpio_clear_event(H_OP_BUTTON);
    current_op = H_OP;
  }
}



static void setup_interrupts(void) {
    gpio_enable_event_detection(Z_OP_BUTTON, GPIO_DETECT_FALLING_EDGE);
    gpio_enable_event_detection(X_OP_BUTTON, GPIO_DETECT_FALLING_EDGE);
    gpio_enable_event_detection(I_OP_BUTTON, GPIO_DETECT_FALLING_EDGE);
    gpio_enable_event_detection(Y_OP_BUTTON, GPIO_DETECT_FALLING_EDGE);
    gpio_enable_event_detection(CNOT_OP_BUTTON, GPIO_DETECT_FALLING_EDGE);
    gpio_enable_event_detection(H_OP_BUTTON, GPIO_DETECT_FALLING_EDGE);

    bool ok = interrupts_attach_handler(button_press);
    assert(ok);
    interrupts_enable_source(INTERRUPTS_GPIO3);
    interrupts_global_enable();
}

static void button_init(void)
{
  gpio_init();
  uart_init();

  gpio_set_input(Z_OP_BUTTON);
  gpio_set_pullup(Z_OP_BUTTON);
  gpio_set_input(X_OP_BUTTON);
  gpio_set_pullup(X_OP_BUTTON);
  gpio_set_input(I_OP_BUTTON);
  gpio_set_pullup(I_OP_BUTTON);
  gpio_set_input(Y_OP_BUTTON);
  gpio_set_pullup(Y_OP_BUTTON);
  gpio_set_input(CNOT_OP_BUTTON);
  gpio_set_pullup(CNOT_OP_BUTTON);
  gpio_set_input(H_OP_BUTTON);
  gpio_set_pullup(H_OP_BUTTON);

  setup_interrupts();
}

// MARK:- PUBLIC FUNCTIONS

/* Find the tensor product of two matrices
 * Returns -1 if failed. 0 if successful.
*/
int dense_coding_op(void) {

  button_init();
  printf("\r");
  printf("Welcome! Let's perform a simple dense coding operation simulation!");
  if (!request_amt_of_qubits()) return -1;
  initialize_state();
  print_state();
  complete_alice_operation();
  complete_bob_operation();
  printf("\nCongratulations! You have decoded the value!");
  printf("\nRESULTS...");
  print_state();
  printf("\nTOTAL QUBITS: %d", qubits);
  printf("\nBITS OF INFORMATION SENT: %d", qubits);
  printf("\nQUBITS ALICE USED: %d", qubits - 1);
  // free(state_start);
  return 0;
}

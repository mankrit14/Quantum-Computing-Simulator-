#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

const float R = 0.7071067812;



// Define the qubit struct
typedef struct qubit {
    float zero;
    float one;
   
}qubit;


// Function to print the value of a qubit
void printQubit(struct qubit q) {
    //printf("(%f), %f)\n", x.zero, x.one);
     printf("(%g)|0> + (%g)|1>\n", q.zero, q.one);
}




// Function to apply the NOT (or X) gate to a qubit
void NOT(struct qubit* q) {
    struct qubit temp;
    temp.zero = 0 * q->zero + 1 * q->one;
    temp.one = 1 * q->zero + 0 * q->one;
    q->zero = temp.zero;
    q->one = temp.one;
}

// Function to apply the Hadamard gate to a qubit
void H(struct qubit* q) {
    struct qubit temp;
    temp.zero = R * q->zero + R * q->one;
    temp.one = R * q->zero - R * q->one;
    q->zero = temp.zero;
    q->one = temp.one;
}

// Function to apply the CNOT gate between a control qubit and a target qubit
void CNOT(struct qubit* control, struct qubit* target) {
    if (control->one == 1) {
        NOT(target);
    }
}

// Function to apply the Z gate to a qubit (rotation around Z axis by pi radians)
void Z(struct qubit* q) {
    struct qubit temp;
    temp.zero = R * q->zero + R * q->one;
    temp.one = R * q->zero - R * q->one;
    q->zero = temp.zero;
    q->one = temp.one;
}

// Function to apply the phase shift gate to a qubit
void phaseShift(struct qubit* q, int k, int n) {
    q->one *= exp(2 * M_PI * I * k / (1 << n));
}

// Function to apply the controlled phase shift gate to two qubits
void controlPhaseShift(struct qubit* q1, struct qubit* q2, int k, int n) {
    if (q1->one != 0) {
        phaseShift(q2, k, n);
    }
}

// Function to apply the quantum Fourier transform (QFT) to an array of qubits
void QFT(qubit *q, int n) {
    int i, j;

    // Apply the Hadamard gate to each qubit
    for (i = 0; i < n; i++) {
        H(&q[i]);

        // Apply the controlled phase shift gates to each subsequent qubit
        for (j = i + 1; j < n; j++) {
            controlPhaseShift(&q[j], &q[i], 1 << (j - i - 1), n);
        }
    }

    // Reverse the order of the qubits

    for (i = 0; i < n; i++) {
        phaseShift(&q[i], i, n);
    }
}

int main(){
  qubit q= {1,0};
    qubit b= {0,1};
    qubit c= {0.5,0.5};
    printf("qubit q = \n");
    printQubit(q);
    printf("qubit q after NOT gate is applied\n");
   NOT(&q);
   printQubit(q);
   printf("qubit q after NOT and H gate is applied\n");

    H(&q);
    printQubit(q);
    printf("qubit q after H gate is applied again\n");
     H(&q);
    printQubit(q);
    CNOT(&b,&q);
    printf(" qubit q after CNOT gate is applied using qubit b as control\n");
    Z(&q);
    
    printf("qubit q after Z gate is applied\n");
    printQubit(q);
    
    

}

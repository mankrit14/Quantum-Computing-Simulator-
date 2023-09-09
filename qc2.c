#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

const float R = 0.7071067812;
int n = 3;


// Define the qubit struct
typedef struct qubit {
    float zero;
    float one;
   
}qubit;

qubit qa[3] = {{0, 1}, {1, 0}, {0.707, 0.707}};
// Function to print the value of a qubit
void printQubit(qubit q) {
    printf("(%g)|0> + (%g)|1>\n", q.zero, q.one);
}

// Function to apply the X gate to a qubit
struct qubit X(struct qubit q) {
    struct qubit n;
    n.zero = 0 * q.zero + 1 * q.one;
    n.one = 1 * q.zero + 0 * q.one;
    return n;
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
    if (fabs(control->one-1.0)< 0.00001) {
        NOT(target);
    }
}

// Function to apply the Z gate to a qubit
void Z(struct qubit* q) {
    struct qubit temp;
    temp.zero = 1 * q->zero + 0 * q->one;
    temp.one = 0 * q->zero - 1 * q->one;
    q->zero = temp.zero;
    q->one = temp.one;
}

// Function to apply the phase shift gate to a qubit
void phaseShift(struct qubit* q, int k, int n) {
    if (fabs(q->one - 1.0) < 0.00001) {
        q->one *= cos(2 * M_PI * k / (1 << n));
    }
}

// Function to apply the controlled phase shift gate to two qubits
void controlPhaseShift(struct qubit* q1, struct qubit* q2, int k, int n) {
    if (fabs(q1->one - 1.0) < 0.00001) {
        phaseShift(q2, k, n);
    }

}

// Function to apply the quantum Fourier transform (QFT) to an array of qubits
void QFT(qubit *qa, int n) {
    int i, j;

    // Apply the Hadamard gate to each qubit
    for (i = 0; i < n; i++) {
        H(&qa[i]);

        // Apply the controlled phase shift gates to each subsequent qubit
        for (j = i + 1; j < n; j++) {
            controlPhaseShift(&qa[j], &qa[i], 1 << (j - i - 1), n);
        }
    }

    // Reverse the order of the qubits

    for (i = 0; i < n/2; i++) {
       struct qubit temp=qa[i];
       qa[i]=qa[n-i-1];
       qa[n-i-1]=temp;
    }
}

int main(){
   
    
    
    int i;
    
    QFT(qa, n);
    for (i = 0; i < n; i++) {
        printQubit(qa[i]);
    }
    return 0;
    

}

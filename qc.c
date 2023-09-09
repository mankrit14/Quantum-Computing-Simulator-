#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

const double R = 0.7071067812;
int n = 3;

// Define the qubit struct
typedef struct qubit {
    double complex zero;
    double complex one;
}qubit;

qubit qa[3] = {{0 + 0*I, 1 + 0*I}, {1 + 0*I, 0 + 0*I}, {0.707 + 0*I, 0.707 + 0*I}};

// Function to print the value of a qubit
void printQubit(qubit q) {
    printf("(%g + %gi)|0> + (%g + %gi)|1>\n", creal(q.zero), cimag(q.zero), creal(q.one), cimag(q.one));
}

void NOT(struct qubit* q) {
    struct qubit temp;
    temp.zero = q->one;
    temp.one = q->zero;
    *q = temp;
}
// Function to apply the Z gate to a qubit
void Z(struct qubit* q) {
    struct qubit temp;
    temp.zero = 1 * q->zero + 0 * q->one;
    temp.one = 0 * q->zero - 1 * q->one;
    q->zero = temp.zero;
    q->one = temp.one;
}

void H(struct qubit* q) {
    struct qubit temp;
    temp.zero = R * (q->zero + q->one);
    temp.one = R * (q->zero - q->one);
    *q = temp;
}

void CNOT(struct qubit* control, struct qubit* target) {
    if (cabs(control->one - 1.0) < 0.00001) {
        NOT(target);
    }
}

void phaseShift(struct qubit* q, int k, int n) {
    double complex phase = cexp(2 * M_PI * I * k / (1 << n));
    q->one *= phase;
}

void controlPhaseShift(struct qubit* q1, struct qubit* q2, int k, int n) {
    if (cabs(q1->one) > 0.00001) {
        phaseShift(q2, k, n);
    }
}

void QFT(qubit *qa, int n) {
    int i, j;

    for (i = 0; i < n; i++) {
        H(&qa[i]);
        for (j = i + 1; j < n; j++) {
            controlPhaseShift(&qa[j], &qa[i], 1 << (j - i), n);
        }
    }

    // Reverse the order of the qubits
    for (i = 0; i < n / 2; i++) {
        struct qubit temp = qa[i];
        qa[i] = qa[n - i - 1];
        qa[n - i - 1] = temp;
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

const int n = 3;  // Number of qubits
const int N = 1 << n;  // 2^n state vector size
double complex state[N];

void normalize() {
    double magnitude = 0;
    for (int i = 0; i < N; i++) {
        magnitude += creal(state[i]) * creal(state[i]) + cimag(state[i]) * cimag(state[i]);
    }
    magnitude = sqrt(magnitude);
    for (int i = 0; i < N; i++) {
        state[i] /= magnitude;
    }
}

void initialize() {
    state[1] = 1;  // |001>
    state[2] = 1;  // |010>
    state[6] = 1/sqrt(2);  // Part of |110>
    state[7] = 1/sqrt(2);  // Part of |111>
    normalize();
}

void printState() {
    for (int i = 0; i < N; i++) {
        printf("%g + %gi |", creal(state[i]), cimag(state[i]));
        for (int j = n-1; j >= 0; j--) {  // print the binary representation of i
            printf("%d", (i >> j) & 1);
        }
        printf(">\n");
    }
}

void applyGate(double complex gate[2][2], int q) {
    double complex newState[N] = {0};
    
    for (int i = 0; i < N; i++) {
        int bit = (i >> q) & 1;
        int targetIndex = bit ? i & ~(1 << q) : i | (1 << q);

        newState[i] += gate[bit][0] * state[i];
        newState[targetIndex] += gate[bit][1] * state[i];
    }

    for (int i = 0; i < N; i++) {
        state[i] = newState[i];
    }

    normalize();
}
void H(int q) {
    double complex Hgate[2][2] = {
        {1/sqrt(2), 1/sqrt(2)},
        {1/sqrt(2), -1/sqrt(2)}
    };
    applyGate(Hgate, q);
}


void NOT(int q) {
    double complex X[2][2] = {{0, 1}, {1, 0}};
    applyGate(X, q);
}
void Z(int q) {
    double complex ZGate[2][2] = {{1, 0}, {0, -1}};
    applyGate(ZGate, q);
}
void phaseShift(int q, double theta) {
    double complex R[2][2] = {{1, 0}, {0, cexp(I*theta)}};
    applyGate(R, q);
}
void controlledPhaseShift(int control, int target, double theta) {
    for (int i = 0; i < N; i++) {
        if (((i >> control) & 1) && ((i >> target) & 1)) {
            state[i] *= cexp(I*theta);
        }
    }
    normalize();
}
void CNOT(int control, int target) {
    for (int i = 0; i < N; i++) {
        if ((i >> control) & 1) { // Control qubit is set
            if ((i >> target) & 1) {
                state[i ^ (1 << target)] += state[i];
                state[i] = 0;
            } else {
                state[i | (1 << target)] += state[i];
                state[i] = 0;
            }
        }
    }
    normalize();
}


void QFT() {
    double complex H[2][2] = {{1/sqrt(2), 1/sqrt(2)}, {1/sqrt(2), -1/sqrt(2)}};
    for (int i = 0; i < n; i++) {
        applyGate(H, i);
        for (int j = i+1; j < n; j++) {
            int k = 1 << (j - i);
            for (int l = 0; l < N; l++) {
                if ((l >> j) & 1 && (l >> i) & 1) {  // if both j-th and i-th bits are set
                    double complex phase = cexp(2 * M_PI * I / k);
                    state[l] *= phase;
                }
            }
            normalize();
        }
    }

    // Reverse the order of qubits (swap bits in the binary representation)
    for (int i = 0; i < N/2; i++) {
        int reversed = 0;
        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) {
                reversed |= 1 << (n - j - 1);
            }
        }
        if (i < reversed) {
            double complex temp = state[i];
            state[i] = state[reversed];
            state[reversed] = temp;
        }
    }

    normalize();
}

int main() {
    initialize();
    printf("Initial state:\n");
    printState();
    QFT();
    printf("Final state:\n");
    printState();
    return 0;
}

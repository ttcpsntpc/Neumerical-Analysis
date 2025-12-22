#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_N 60

int n[5] = {20, 30, 40, 50, 60};
double B[MAX_N][MAX_N], A[MAX_N][MAX_N], b[MAX_N];

void init_B() {
    srand( time(NULL) );
    for(int i = 0; i < MAX_N; i++) {
        for(int j = 0; j < MAX_N; j++) {
            B[i][j] = (double)rand() / RAND_MAX;
            if(i == j)
                B[i][i] += 2;
        }
    }
}

void init_A(int N) {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            A[i][j] = 0;
            for(int k = 0; k < N; k++) {
                A[i][j] += B[i][k] * B[j][k];
            }
        }
    }
}
void init_b(int N) {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            b[i] += A[i][j];
        }
    }
}
double inner_product(double a[], double b[], int N) {
    double sum = 0;
    for(int i = 0; i < N; i++) 
        sum += a[i]*b[i];
    return sum;
}
double inf_norm(double v[], int N) {
    double norm = 0;
    for(int i = 0; i < N; i++) {
        norm = abs(v[i])>norm ? v[i] : norm;
    }
    return norm;
}
double A_conjugate(double a[], double b[], int n) {
    double temp[60] = { 0 };
    double sum = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp[j] += A[i][j] * b[j];
        }
        sum += inner_product(a, temp, n);
    }
    return sum;
}

int main() {
    init_B();
    for(int i = 0; i < 5; i++) {
        int N = n[i];
        init_A(N);
        init_b(N);

    }
    return 0;
}

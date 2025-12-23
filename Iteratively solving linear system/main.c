#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX_N 60
#define EPSILON 10e-7
FILE *fp_iter, *fp_time, *fp_dir; // dir -> conjugate gradient's searching direction
int n[5] = {20, 30, 40, 50, 60};
double B[MAX_N][MAX_N], A[MAX_N][MAX_N], b[MAX_N], x0[MAX_N], x[MAX_N];

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
void init_x() {
    srand( time(NULL) );
    for(int i = 0; i < MAX_N; i++) {
        x0[i] = (double)rand() / RAND_MAX;
    }    

}
void copy_x(int N) {
    for(int i = 0; i < N; i++) {
        x[i] = x0[i];
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
        b[i] = 0;
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
double norm_inf(double v[], int N) {
    double norm = 0;
    for(int i = 0; i < N; i++) {
        norm = fabs(v[i])>norm ? fabs(v[i]) : norm;
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

void conjugate_gradient(int N) {
    double d[MAX_N], g[MAX_N], h[MAX_N];
    double error;
    int iteration = 0;
    for(int i = 0; i < N; i++) {
        double Ax0 = 0;
        for(int j = 0; j < N; j++) {
            Ax0 += A[i][j]*x[j];
        }
        d[i] = b[i] - Ax0;
        g[i] = -d[i];
    }
    double newG2 = inner_product(g, g, N), oldG2;
    error = 100;
    
    while(error > EPSILON) {
        // h = A * d
        for(int i = 0; i < N; i++) {
            double Ad = 0;
            for(int j = 0; j < N; j++) {
                Ad += A[i][j]*d[j];
            }
            h[i] = Ad;
        }
        // oldG2 = <g, g>
        oldG2 = newG2;
        double alpha = oldG2 / inner_product(d, h, N);
        for(int i = 0; i < N; i++) {
            x[i] += alpha * d[i];
            g[i] += alpha * h[i];
        }
        newG2 = inner_product(g, g, N);
        double beta = newG2 / oldG2;
        for(int i = 0; i < N; i++) {
            d[i] = -g[i] + beta * d[i];
        }
        error = norm_inf(d, N);
        iteration++;

        for(int i = 0; i < 10; i++)
            printf("%lf ", x[i]);
        printf("\n");
    }
    fprintf(fp_iter, "%d\n", iteration);
}

int main() {
    fp_iter = fopen("src/iteration.txt", "w");
    fp_time = fopen("src/time.txt", "w");
    fp_dir = fopen("src/direction.txt", "w");
    init_B();
    init_x();
    // conjugate gradient
    for(int i = 0; i < 5; i++) {
        int N = n[i];
        init_A(N);
        init_b(N);
        copy_x(N);
        conjugate_gradient(N);
    }
    
    // SOR
    for(int i = 0; i < 5; i++) {
        int N = n[i];
        init_A(N);
        init_b(N);

    }

    // SSOR -> conjugate gradient
    for(int i = 0; i < 5; i++) {
        int N = n[i];
        init_A(N);
        init_b(N);
        copy_x(N);

    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAX_N 60
#define EPSILON 10e-7
FILE *fp_iter, *fp_time, *fp_dir, *fp_conj_relation, *fp_sol; // dir -> conjugate gradient's searching direction
int n[5] = {20, 30, 40, 50, 60};
double B[MAX_N][MAX_N], A[MAX_N][MAX_N], b[MAX_N], x0[MAX_N], x[MAX_N], d[MAX_N*2][MAX_N];

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
            temp[i] += A[i][j] * b[j];
        }
    }
    sum += inner_product(a, temp, n);
    return sum;
}

int conjugate_gradient(int N) {
    double g[MAX_N], h[MAX_N];
    double error;
    int iteration = 0;
    // initial direction & gradient
    for(int i = 0; i < N; i++) {
        double Ax0 = 0;
        for(int j = 0; j < N; j++) {
            Ax0 += A[i][j]*x[j];
        }
        d[iteration][i] = b[i] - Ax0;
        g[i] = -d[iteration][i];
    }
    // record initial direction and solution x
    fprintf(fp_dir, "%d ", 0);
    fprintf(fp_sol, "%d ", 0);
    for(int i = 0; i < N; i++){
        fprintf(fp_dir, "%lf ", d[iteration][i]);
        fprintf(fp_sol, "%lf ", x[i]);  
    }
    fprintf(fp_dir, "\n");
    fprintf(fp_sol, "\n");

    double newG2 = inner_product(g, g, N), oldG2;
    error = norm_inf(d[iteration], N);
    while(error > EPSILON) {
        iteration++;
        // h = A * d
        for(int i = 0; i < N; i++) {
            double Ad = 0;
            for(int j = 0; j < N; j++) {
                Ad += A[i][j]*d[iteration - 1][j];
            }
            h[i] = Ad;
        }
        oldG2 = newG2;
        double alpha = oldG2 / inner_product(d[iteration - 1], h, N);
        for(int i = 0; i < N; i++) {
            x[i] += alpha * d[iteration - 1][i];
            g[i] += alpha * h[i];
        }
        newG2 = inner_product(g, g, N);
        double beta = newG2 / oldG2;
        for(int i = 0; i < N; i++) {
            d[iteration][i] = -g[i] + beta * d[iteration - 1][i];
        }

        error = norm_inf(g, N);
        
        fprintf(fp_dir, "%d ", iteration);
        fprintf(fp_sol, "%d ", iteration);
        for(int i = 0; i < N; i++) {
            fprintf(fp_dir, "%lf ", d[iteration][i]);
            fprintf(fp_sol, "%lf ", x[i]);
        }
        fprintf(fp_dir, "\n");
        fprintf(fp_sol, "\n");
    }
    return iteration;
}
void record_conjugate_relation(int N, int iteration) {
    for(int i = 0; i <= iteration; i++) {
        for(int j = 0; j <= iteration; j++) {
            double val = A_conjugate(d[i], d[j], N);
            fprintf(fp_conj_relation, "%lf ", val);
        }
        fprintf(fp_conj_relation, "\n");
    }
}

int SOR(int N, double w) {
    double r[MAX_N];
    double error;
    int iteration = 0;
    error = 100;
    while(error > EPSILON) {
        iteration++;
        for(int i = 0; i < N; i++) {
            double sum = b[i];
            for(int j = 0; j < N; j++) 
                sum -= A[i][j]*x[j];
            x[i] += w * sum / A[i][i];
        }
        // compute the residual
        for(int i = 0; i < N; i++) {
            double Ax = 0;
            for(int j = 0; j < N; j++)
                Ax += A[i][j]*x[j];
            r[i] = b[i] - Ax;
        }
        error = norm_inf(r, N);
    }
    return iteration;
}

int SSOR(int N, double w, int max_iter) {
    double r[MAX_N];
    double error;
    int iteration = 0;
    error = 100;
    if(max_iter <= 0) 
        max_iter = 1e9;
    while(error > EPSILON && iteration < max_iter) {
        if(iteration % 2 == 0) {
            for(int i = 0; i < N; i++) {
                double sum = b[i];
                for(int j = 0; j < N; j++) 
                    sum -= A[i][j]*x[j];
                x[i] += w * sum / A[i][i];
            }
            // compute the residual
            for(int i = 0; i < N; i++) {
                double Ax = 0;
                for(int j = 0; j < N; j++)
                    Ax += A[i][j]*x[j];
                r[i] = b[i] - Ax;
            }
        } else {
            for(int i = N - 1; i >= 0; i--) {
                double sum = b[i];
                for(int j = 0; j < N; j++) 
                    sum -= A[i][j]*x[j];
                x[i] += w * sum / A[i][i];
            }
            // compute the residual
            for(int i = 0; i < N; i++) {
                double Ax = 0;
                for(int j = 0; j < N; j++)
                    Ax += A[i][j]*x[j];
                r[i] = b[i] - Ax;
            }
        }
        error = norm_inf(r, N);
        iteration++;
    }
    return iteration;
}

int main() {
    fp_iter = fopen("src/iteration.txt", "w");
    fp_time = fopen("src/time.txt", "w");
    fp_dir = fopen("src/direction.txt", "w");
    fp_conj_relation = fopen("src/conjugate_relation.txt", "w");
    fp_sol = fopen("src/solution.txt", "w");
    clock_t start, end;
    double cpu_time_used;
    int iteration;
    init_B();
    init_x();

    // conjugate gradient
    fprintf(fp_iter, "# Conjugate Gradient Iteration:\n");
    fprintf(fp_time, "# Conjugate Gradient Time:\n");
    fprintf(fp_dir, "# Conjugate Gradient Direction:\n");
    fprintf(fp_conj_relation, "# Conjugate Relation (A conjugate):\n");
    fprintf(fp_sol, "# Conjugate Gradient Solution:\n");
    for(int i = 0; i < 5; i++) {
        fprintf(fp_dir, "# N=%d\n", n[i]);
        fprintf(fp_conj_relation, "# N=%d\n", n[i]);
        fprintf(fp_sol, "# N=%d\n", n[i]);
        
        int N = n[i];
        init_A(N);
        init_b(N);
        copy_x(N);

        start = clock();
        iteration = conjugate_gradient(N);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        record_conjugate_relation(N, iteration);

        fprintf(fp_iter, "%d %d\n", N, iteration);
        fprintf(fp_time, "%d %lf\n", N, cpu_time_used);
        fprintf(fp_dir, "\n\n");
        fprintf(fp_conj_relation, "\n\n");
        fprintf(fp_sol, "\n\n");
    }
    
    // SOR
    fprintf(fp_iter, "\n\n# SOR Iteration:\n");
    fprintf(fp_time, "\n\n# SOR Time:\n");
    for(int i = 0; i < 5; i++) {
        double best_w, best_time;
        int min_iteration = 1e9;
        printf("N=%d:\n", n[i]);
        for(double w = 1.00; w <= 1.5; w += 0.1) {
            int N = n[i];
            init_A(N);
            init_b(N);
            copy_x(N);

            start = clock();
            iteration = SOR(N, w);
            end = clock();

            printf("(%.2lf, %d) ", w, iteration);
            if(iteration < min_iteration) {
                min_iteration = iteration;
                best_w = w;
                best_time = ((double) (end - start)) / CLOCKS_PER_SEC;
            }
        }
        printf("\nBest w: %.2lf with %d iterations\n", best_w, min_iteration);
        fprintf(fp_iter, "# best w: %.2lf\n", best_w);
        fprintf(fp_iter, "%d %d\n", n[i], min_iteration);
        fprintf(fp_time, "# best w: %.2lf\n", best_w);
        fprintf(fp_time, "%d %lf\n", n[i], best_time);
    }

    // SSOR -> conjugate gradient
    fprintf(fp_iter, "\n\n# SSOR -> Conjugate Gradient Iteration:\n");
    fprintf(fp_time, "\n\n# SSOR -> Conjugate Gradient Time:\n");
    fprintf(fp_dir, "\n\n# SSOR -> Conjugate Gradient Direction:\n");
    fprintf(fp_sol, "\n\n# SSOR -> Conjugate Gradient Solution:\n");
    for(int i = 0; i < 5; i++) {
        fprintf(fp_dir, "# N=%d\n", n[i]);
        fprintf(fp_sol, "# N=%d\n", n[i]);
        double best_w, best_time;
        int min_iteration = 1e9;
        printf("N=%d:\n", n[i]);
        for(double w = 1.00; w <= 1.5; w += 0.1) {
            int N = n[i];
            init_A(N);
            init_b(N);
            copy_x(N);

            start = clock();
            iteration = SSOR(N, w, 10);
            iteration += conjugate_gradient(N);
            end = clock();

            printf("(%.2lf, %d) ", w, iteration);
            if(iteration < min_iteration) {
                min_iteration = iteration;
                best_w = w;
                best_time = ((double) (end - start)) / CLOCKS_PER_SEC;
            }
            fprintf(fp_dir, "\n\n");
            fprintf(fp_sol, "\n\n");
        }
        printf("\nBest w: %.2lf with %d iterations\n", best_w, min_iteration);
        fprintf(fp_iter, "# best w: %.2lf\n", best_w);
        fprintf(fp_iter, "%d %d\n", n[i], min_iteration);
        fprintf(fp_time, "# best w: %.2lf\n", best_w);
        fprintf(fp_time, "%d %lf\n", n[i], best_time);
    }

    fclose(fp_iter);
    fclose(fp_time);
    fclose(fp_dir);
    fclose(fp_conj_relation);
    fclose(fp_sol);
    return 0;
}

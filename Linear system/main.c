#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "gnuplot.h"
#define PI 3.14159265358979323846

int r, n, m[5] = {5, 6, 7, 8, 9}; // degree of polynomial
double A[16][16]; // coefficient matrix
double B[16][16]; // A^T * A
double cx[16]; // unknown vector -> a0, a1, a2, ...
double cy[16]; // unknown vector -> b0, b1, b2, ...
double dx[16]; // A^T * x
double dy[16]; // A^T * y
double t[16]; // sample points
double x[16], y[16]; // function values

void initial(int degree) {
    r = 10;  // radius
    n = 15; // divided circle into n parts -> n+1 sample points
    for(int i = 0; i <= n; i++) {
        t[i] = 2 * PI / n * i;
        x[i] = r * cos(2 * PI / n * i);
        y[i] = r * sin(2 * PI / n * i);

        for(int j = 0; j < 16; j++) { // initial all A matrix elements at once
            A[i][j] = pow(t[i], j);
        }
    }
}

void AT_multiply_A(int rows, int cols) {
    // B = A^T * A
    for(int i = 0; i < cols; i++) {
        for(int j = 0; j < cols; j++) {
            B[i][j] = 0;
            for(int k = 0; k < rows; k++) {
                B[i][j] += A[k][i] * A[k][j];
            }
        }
    }
}

void AT_multiply_V(int rows, int cols, double* vec, double* result) {
    // result = A^T * vec
    for(int i = 0; i < cols; i++) {
        result[i] = 0;
        for(int j = 0; j < rows; j++) {
            result[i] += A[j][i] * vec[j];
        }
    }
}

void gaussian_elimination(int rows, int cols) {
    // forward elimination
    AT_multiply_A(rows, cols);
    AT_multiply_V(rows, cols, x, dx);
    AT_multiply_V(rows, cols, y, dy);

    for(int i = 0; i < cols - 1; i++) {
        // partial pivoting
        int k = i;
        for(int j = i + 1; j < cols; j++) {
            if(fabs(B[j][i]) > fabs(B[k][i])) {
                k = j;
            }
        }
        // is swap neccessary?
        if(i != k) {
            double temp;
            for(int j = i; j < cols; j++) {
                temp = B[i][j];
                B[i][j] = B[k][j];
                B[k][j] = temp;
            }
            // rhs swap
            temp = dx[i];
            dx[i] = dx[k];
            dx[k] = temp;
            temp = dy[i];
            dy[i] = dy[k];
            dy[k] = temp;
        }
        // elimination
        for(int j = i + 1; j < cols; j++) {
            double factor = B[j][i] / B[i][i];
            for(int l = i; l < cols; l++) {
                B[j][l] -= factor * B[i][l];
            }
            dx[j] -= factor * dx[i];
            dy[j] -= factor * dy[i];
        }
    }

    // back substitution
    for(int i = cols - 1; i >= 0; i--) {
        cx[i] = dx[i]/B[i][i];
        cy[i] = dy[i]/B[i][i];
        for(int j = i - 1; j >= 0; j--) {
            dx[j] -= B[j][i] * cx[i];
            dy[j] -= B[j][i] * cy[i];
        }
    }
}

void horner_s_algorithm(int samples, int degree, FILE *fp, FILE *err_x_fp, FILE *err_y_fp) {
    double t_s = 2 * PI / (samples - 1);
    double x_2_norm = 0.0, y_2_norm = 0.0;
    double x_inf_norm = 0.0, y_inf_norm = 0.0;
    for(int i = 0; i < samples; i++) {
        double t_i = t_s * i;
        double fx = cx[degree];
        double fy = cy[degree];
        for(int j = degree - 1; j >= 0; j--) {
            fx = fx * t_i + cx[j];
            fy = fy * t_i + cy[j];
        }
        fprintf(fp, "%lf %lf %lf\n", t_i, fx, fy);

        // calculate relative error
        // 2-norm
        double x_i = r * cos(t_i);
        double y_i = r * sin(t_i);
        x_2_norm += pow((x_i - fx), 2);
        y_2_norm += pow((y_i - fy), 2);
        // infinity-norm
        if(fabs(x_i - fx) > x_inf_norm)
            x_inf_norm = fabs(x_i - fx);
        if(fabs(y_i - fy) > y_inf_norm)
            y_inf_norm = fabs(y_i - fy);
    }
    x_2_norm = sqrt(x_2_norm);
    y_2_norm = sqrt(y_2_norm);
    fprintf(err_x_fp, "%d %lf %lf\n", degree, x_2_norm, x_inf_norm);
    fprintf(err_y_fp, "%d %lf %lf\n", degree, y_2_norm, y_inf_norm);

    fprintf(fp, "\n\n");
}

void record_polynomials(int degree, FILE *fp);

int main() {
    FILE *fp_polynomials = fopen("polynomial.txt", "w");
    FILE *fp_points; // 50 drawing points
    FILE *err_x_fp = fopen("error_x.txt", "w");
    FILE *err_y_fp = fopen("error_y.txt", "w");

    fprintf(fp_polynomials, "# Gaussian elimination\n");
    fprintf(err_x_fp, "# Gaussian elimination error in x\n# degree 2-norm infinity-norm\n");
    fprintf(err_y_fp, "# Gaussian elimination error in y\n# degree 2-norm infinity-norm\n");
    fp_points = fopen("gaussian.txt", "w");
    fprintf(fp_points, "# t x y\n");
    for(int i = 0; i < 5; i++) {
        fprintf(fp_points, "# Degree = %d\n", m[i]);

        initial(m[i]);
        gaussian_elimination(n + 1, m[i] + 1);
        horner_s_algorithm(50, m[i], fp_points, err_x_fp, err_y_fp);
        
        record_polynomials(m[i], fp_polynomials);
    }
    fclose(fp_points);
    
    
    
    
    
    gnuplot_gauss();

    fclose(fp_polynomials);
    fclose(err_x_fp);
    fclose(err_y_fp);
    return 0;
}

void record_polynomials(int degree, FILE *fp) {
    fprintf(fp, "Degree %d:\n", degree);
    fprintf(fp, "x(t) = ");
    for(int i = 0; i <= degree; i++) {
        if(i == 0)
            fprintf(fp, "%lf ", cx[i]);
        else
            fprintf(fp, "+ %lf*t^%d ", cx[i], i);
    }
    fprintf(fp, "\n");

    fprintf(fp, "y(t) = ");
    for(int i = 0; i <= degree; i++) {
        if(i == 0)
            fprintf(fp, "%lf ", cy[i]);
        else
            fprintf(fp, "+ %lf*t^%d ", cy[i], i);
    }
    fprintf(fp, "\n\n");
}
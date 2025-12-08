#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "gnuplot.h"
#define PI 3.14159265358979323846
#define M 13 // number of polynomial degrees to test

int r, n, m[M] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; // degree of polynomial
double A[16][16]; // coefficient matrix
double B[16][16]; // A^T * A
double cx[16]; // unknown vector -> a0, a1, a2, ...
double cy[16]; // unknown vector -> b0, b1, b2, ...
double dx[16]; // A^T * x
double dy[16]; // A^T * y
double t[16]; // sample points
double x[16], y[16]; // function values

void initial() {
    r = 10;  // radius
    n = 15; // divided circle into n parts -> n+1 sample points
    for(int i = 0; i <= n; i++) {
        t[i] = 2 * PI / n * i; // input value
        x[i] = r * cos(t[i]); // ground truth
        y[i] = r * sin(t[i]);

        for(int j = 0; j < 16; j++) { // initial all A matrix elements at once
            A[i][j] = pow(t[i], j);
        }
    }
}

// B = A^T * A
void AT_multiply_A(int rows, int cols) {
    for(int i = 0; i < cols; i++) {
        for(int j = 0; j < cols; j++) {
            B[i][j] = 0;
            for(int k = 0; k < rows; k++) {
                B[i][j] += A[k][i] * A[k][j];
            }
        }
    }
}

// result = A^T * vec
void AT_multiply_V(int rows, int cols, double* vec, double* result) {
    for(int i = 0; i < cols; i++) {
        result[i] = 0;
        for(int j = 0; j < rows; j++) {
            result[i] += A[j][i] * vec[j];
        }
    }
}

double VT_multiply_V(int rows, double* v1, double* v2, int i) {
    double result = 0.0;
    for(i; i < rows; i++) {
        result += v1[i] * v2[i];  
    }
    return result;
}

// v = t + |t|*e
void create_reflect_vector(double A[16][16], double* v, int start_column, int rows) {
    for(int i = start_column; i < rows; i++) 
        v[i] = A[i][start_column];
    double vTv = VT_multiply_V(rows, v, v, start_column);
    double norm_v = sqrt(vTv);
    if(v[start_column] >= 0)
        v[start_column] += norm_v;
    else 
        v[start_column] -= norm_v;
}

void gaussian_elimination(int rows, int cols) {
    // Ax = b -> (A^T * A)x = A^T * b
    AT_multiply_A(rows, cols);
    AT_multiply_V(rows, cols, x, dx);
    AT_multiply_V(rows, cols, y, dy);

    // forward elimination
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

    // backward substitution
    for(int i = cols - 1; i >= 0; i--) {
        cx[i] = dx[i]/B[i][i];
        cy[i] = dy[i]/B[i][i];
        for(int j = i - 1; j >= 0; j--) {
            dx[j] -= B[j][i] * cx[i];
            dy[j] -= B[j][i] * cy[i];
        }
    }
}

void copy_matrix(int rows, int cols, double src[16][16], double dst[16][16]) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

void QR_decomposition_new(int rows, int cols, int isNew) {
    if(isNew) {
        AT_multiply_A(rows, cols);
        AT_multiply_V(rows, cols, x, dx);
        AT_multiply_V(rows, cols, y, dy);
        rows = cols;
    }
    else {
        for(int i = 0; i < rows; i++)
            dx[i] = x[i];
        for(int i = 0; i < rows; i++)
            dy[i] = y[i];
    }

    double R[16][16];
    if(isNew)
        copy_matrix(rows, cols, B, R);
    else
        copy_matrix(rows, cols, A, R); // we can directly modify B, but for clarity, use R to store result
    // forward elimination
    for(int i = 0; i < cols - 1; i++) { // i-th elimination 
        double v[16] = {0};
        create_reflect_vector(R, v, i, rows);
        double vTv = VT_multiply_V(rows, v, v, i);
        double t[16];
        // H*A = R
        for(int j = i; j < cols; j++) { // calculate j-th column of R
            for(int k = i; k < rows; k++) // copy j-th column to t
                t[k] = R[k][j];
            // householder transformation
            double vTt = VT_multiply_V(rows, v, t, i);
            for(int k = i; k < rows; k++) {
                R[k][j] -= 2.0 * (vTt / vTv) * v[k];
            }
        }
        // H*b = new b
        double vTdx = VT_multiply_V(rows, v, dx, i);
        double vTdy = VT_multiply_V(rows, v, dy, i);
        for(int k = i; k < rows; k++) {
            dx[k] -= 2.0 * (vTdx / vTv) * v[k];
            dy[k] -= 2.0 * (vTdy / vTv) * v[k];
        }
    }
    if(isNew == 0) { // last column need elimination for origin system
        double v[16] = {0};
        create_reflect_vector(R, v, cols - 1, rows);
        double vTv = VT_multiply_V(rows, v, v, cols - 1);
        double t[16];
        for(int k = cols - 1; k < rows; k++) // copy j-th column to t
            t[k] = R[k][cols - 1];
            // householder transformation
            double vTt = VT_multiply_V(rows, v, t, cols - 1);
            for(int k = cols - 1; k < rows; k++) {
                R[k][cols - 1] -= 2.0 * (vTt / vTv) * v[k];
        }

        double vTdx = VT_multiply_V(rows, v, dx, cols - 1);
        double vTdy = VT_multiply_V(rows, v, dy, cols - 1);
        for(int k = cols - 1; k < rows; k++) {
            dx[k] -= 2.0 * (vTdx / vTv) * v[k];
            dy[k] -= 2.0 * (vTdy / vTv) * v[k];
        }
    }

    // backward substitution
    for(int i = cols - 1; i >= 0; i--) {
        cx[i] = dx[i] / R[i][i];
        cy[i] = dy[i] / R[i][i];
        for(int j = i - 1; j >= 0; j--) {
            dx[j] -= R[j][i] * cx[i];
            dy[j] -= R[j][i] * cy[i];
        }
    }
}

// calculate polynomaial values and relative errors
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

        // calculate absolute error
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
    fprintf(err_x_fp, "%d %.10lf %.10lf\n", degree, x_2_norm, x_inf_norm);
    fprintf(err_y_fp, "%d %.10lf %.10lf\n", degree, y_2_norm, y_inf_norm);

    fprintf(fp, "\n\n");
}

void record_polynomials(int degree, FILE *fp);

int main() {
    FILE *fp_polynomials = fopen("polynomial.txt", "w");
    FILE *fp_points; // 50 drawing points on different methods
    FILE *err_x_fp = fopen("error_x.txt", "w");
    FILE *err_y_fp = fopen("error_y.txt", "w");

    // method 1: Gaussian elimination
    fp_points = fopen("50p_gaussian.txt", "w");
    fprintf(fp_points, "# t x y\n");
    fprintf(fp_polynomials, "# Gaussian elimination\n");
    fprintf(err_x_fp, "# Gaussian elimination error in x\n# degree 2-norm infinity-norm\n");
    fprintf(err_y_fp, "# Gaussian elimination error in y\n# degree 2-norm infinity-norm\n");
    initial();
    for(int i = 0; i < M; i++) {
        fprintf(fp_points, "# Degree = %d\n", m[i]);

        gaussian_elimination(n + 1, m[i] + 1);
        horner_s_algorithm(50, m[i], fp_points, err_x_fp, err_y_fp);
        
        record_polynomials(m[i], fp_polynomials);
    }
    fprintf(fp_polynomials, "\n");
    fprintf(err_x_fp, "\n\n");
    fprintf(err_y_fp, "\n\n");
    fclose(fp_points);
    
    // method 2: QR decomposition ( New system )
    fp_points = fopen("50p_new_QR_decomposition.txt", "w");
    fprintf(fp_points, "# t x y\n");
    fprintf(fp_polynomials, "# QR decomposition ( New system )\n");
    fprintf(err_x_fp, "# QR decomposition ( New system ) error in x\n# degree 2-norm infinity-norm\n");
    fprintf(err_y_fp, "# QR decomposition ( New system ) error in y\n# degree 2-norm infinity-norm\n");
    initial();
    for(int i = 0; i < M; i++) {
        fprintf(fp_points, "# Degree = %d\n", m[i]);

        QR_decomposition_new(n + 1, m[i] + 1, 1);
        horner_s_algorithm(50, m[i], fp_points, err_x_fp, err_y_fp);
        
        record_polynomials(m[i], fp_polynomials);
    }
    fprintf(fp_polynomials, "\n");
    fprintf(err_x_fp, "\n\n");
    fprintf(err_y_fp, "\n\n");
    fclose(fp_points);
    
    // method 3: QR decomposition ( Origin system )
    fp_points = fopen("50p_origin_QR_decomposition.txt", "w");
    fprintf(fp_points, "# t x y\n");
    fprintf(fp_polynomials, "# QR decomposition ( Origin system )\n");
    fprintf(err_x_fp, "# QR decomposition ( Origin system ) error in x\n# degree 2-norm infinity-norm\n");
    fprintf(err_y_fp, "# QR decomposition ( Origin system ) error in y\n# degree 2-norm infinity-norm\n");
    initial();
    for(int i = 0; i < M; i++) {
        fprintf(fp_points, "# Degree = %d\n", m[i]);

        QR_decomposition_new(n + 1, m[i] + 1, 0);
        horner_s_algorithm(50, m[i], fp_points, err_x_fp, err_y_fp);
        
        record_polynomials(m[i], fp_polynomials);
    }
    fprintf(fp_polynomials, "\n");
    fprintf(err_x_fp, "\n\n");
    fprintf(err_y_fp, "\n\n");

    fclose(fp_points);
    fclose(fp_polynomials);
    fclose(err_x_fp);
    fclose(err_y_fp);
    
    gnuplot_gauss();
    plot_error();
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


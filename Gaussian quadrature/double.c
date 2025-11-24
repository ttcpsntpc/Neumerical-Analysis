#include <stdio.h>
#include <math.h>
#define PI 3.14159265358979323846

// 0, A_i(w_i)
double A2[3] = {1.0, 1.0};
double A3[4] = {0.8888888888888888, 0.5555555555555556, 0.5555555555555556};
double A4[5] = {0.652145154862546, 0.652145154862546, 0.347854845137453, 0.347854845137453};
double A5[6] = {0.568888888888888, 0.478628670499366, 0.478628670499366, 0.236926885056189, 0.236926885056189};
double A6[7] = {0.360761573048138, 0.360761573048138, 0.467913934572691, 0.467913934572691, 0.171324492379170, 0.171324492379170};
double A7[8] = { 0.417959183673469, 0.381830050505118, 0.381830050505118, 0.279705391489276, 0.279705391489276, 0.129484966168869, 0.129484966168869};
// x, x_i
double x2[3] = {-0.577350269189625, 0.577350269189625};
double x3[4] = {0.000000000000000, -0.774596669241483, 0.774596669241483};
double x4[5] = {-0.339981043584856, 0.339981043584856, -0.861136311594052, 0.861136311594052};
double x5[6] = {0.000000000000000, -0.538469310105683, 0.538469310105683, -0.906179845938664, 0.906179845938664};
double x6[7] = {0.661209386466264, -0.661209386466264, -0.238619186083196, 0.238619186083196, -0.932469514203152, 0.932469514203152};
double x7[8] = {0.000000000000000, 0.405845151377397, -0.405845151377397, -0.741531185599394, 0.741531185599394, -0.949107912342758, 0.949107912342758};

int grid[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32}; // divide D into n x n cells
double x_start = 2.0, x_end = 6.0, y_start = 2.0, y_end = 6.0; // integration domain

double f(double x, double y) {
    return 3 * sin(8.0 * PI * x) * sin(8.0 * PI * y) + x + y + 1;
}

double gaussian_quadrature_2d(double x[], double A[], int n, int grid_n) {
    // (b - a)
    double hx = (x_end - x_start) / grid_n;
    double hy = (y_end - y_start) / grid_n;
    double sum = 0.0;

    for (int i = 0; i < grid_n; i++) {
        for (int j = 0; j < grid_n; j++) {
            // (a + b) / 2
            double x_center = x_start + (i + 0.5) * hx;
            double y_center = y_start + (j + 0.5) * hy;

            for (int k = 0; k < n; k++) {
                for (int l = 0; l < n; l++) {
                    double xi = (hx / 2.0) * x[k] + x_center;
                    double yi = (hy / 2.0) * x[l] + y_center;
                    sum += A[k] * A[l] * f(xi, yi);
                }
            }
        }
    }

    sum *= (hx / 2.0) * (hy / 2.0);
    return sum;
}

void write_block(FILE *fp, int N, double x[], double A[]) {
    fprintf(fp, "# N=%d ( absolute error )\n", N); // 寫入標題
    int grid_size = sizeof(grid) / sizeof(grid[0]);
    for (int i = 0; i < grid_size; i++) {
        double area = gaussian_quadrature_2d(x, A, N, grid[i]);
        double error = fabsf(area - 144.0f);
        
        if (error < 1e-15f) {
            fprintf(fp, "%d   1e-16\n", grid[i]);
        } else {
            fprintf(fp, "%d   %.15f\n", grid[i], error);
        }
    }
    fprintf(fp, "\n\n"); // 寫入兩個換行，區隔數據塊 (Gnuplot index 需求)

    fprintf(fp, "# N=%d ( relative error )\n", N); // 寫入標題
    for(int i = 0; i < grid_size; i++) {
        double area = gaussian_quadrature_2d(x, A, N, grid[i]);
        double relative_error = fabs(area - 144.0) / 144.0;

        if (relative_error < 1e-15) {
            fprintf(fp, "%d   1e-16\n", grid[i]);
        } else {
            fprintf(fp, "%d   %.15f\n", grid[i], relative_error);
        }
    }
    fprintf(fp, "\n\n");
}

int main() {
    double area;
    FILE *fp = fopen("../double.txt", "w");

    write_block(fp, 2, x2, A2);
    write_block(fp, 3, x3, A3);
    write_block(fp, 4, x4, A4);
    write_block(fp, 5, x5, A5);
    write_block(fp, 6, x6, A6);
    write_block(fp, 7, x7, A7);
    
    fclose(fp);
    return 0;
}
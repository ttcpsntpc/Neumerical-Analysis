#include <stdio.h>
#include <math.h>

// 將 PI 也加上 f 確保運算時不會轉成 float
#define PI 3.14159265358979323846f 

// 權重 A_i (改為 float)
float A2[3] = {1.0f, 1.0f};
float A3[4] = {0.8888888888888888f, 0.5555555555555556f, 0.5555555555555556f};
float A4[5] = {0.652145154862546f, 0.652145154862546f, 0.347854845137453f, 0.347854845137453f};
float A5[6] = {0.568888888888888f, 0.478628670499366f, 0.478628670499366f, 0.236926885056189f, 0.236926885056189f};
float A6[7] = {0.360761573048138f, 0.360761573048138f, 0.467913934572691f, 0.467913934572691f, 0.171324492379170f, 0.171324492379170f};
float A7[8] = { 0.417959183673469f, 0.381830050505118f, 0.381830050505118f, 0.279705391489276f, 0.279705391489276f, 0.129484966168869f, 0.129484966168869f};

// 節點 x_i (改為 float)
float x2[3] = {-0.577350269189625f, 0.577350269189625f};
float x3[4] = {0.000000000000000f, -0.774596669241483f, 0.774596669241483f};
float x4[5] = {-0.339981043584856f, 0.339981043584856f, -0.861136311594052f, 0.861136311594052f};
float x5[6] = {0.000000000000000f, -0.538469310105683f, 0.538469310105683f, -0.906179845938664f, 0.906179845938664f};
float x6[7] = {0.661209386466264f, -0.661209386466264f, -0.238619186083196f, 0.238619186083196f, -0.932469514203152f, 0.932469514203152f};
float x7[8] = {0.000000000000000f, 0.405845151377397f, -0.405845151377397f, -0.741531185599394f, 0.741531185599394f, -0.949107912342758f, 0.949107912342758f};

int grid[] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32}; // divide D into n x n cells
float x_start = 2.0f, x_end = 6.0f, y_start = 2.0f, y_end = 6.0f; // integration domain

// 輸入輸出都改為 float，sin 改為 sinf
float f(float x, float y) {
    return 3.0f * sinf(8.0f * PI * x) * sinf(8.0f * PI * y) + x + y + 1.0f;
}

float gaussian_quadrature_2d(float x[], float A[], int n, int grid_n) {
    // (b - a)
    float hx = (x_end - x_start) / grid_n;
    float hy = (y_end - y_start) / grid_n;
    float sum = 0.0f;

    for (int i = 0; i < grid_n; i++) {
        for (int j = 0; j < grid_n; j++) {
            // (a + b) / 2
            float x_center = x_start + (i + 0.5f) * hx;
            float y_center = y_start + (j + 0.5f) * hy;

            for (int k = 0; k < n; k++) {
                for (int l = 0; l < n; l++) {
                    float xi = (hx / 2.0f) * x[k] + x_center;
                    float yi = (hy / 2.0f) * x[l] + y_center;
                    sum += A[k] * A[l] * f(xi, yi);
                }
            }
        }
    }

    sum *= (hx / 2.0f) * (hy / 2.0f);
    return sum;
}

void write_block(FILE *fp, int N, float x[], float A[]) {
    fprintf(fp, "# N=%d ( absolute error )\n", N); // 寫入標題
    int grid_size = sizeof(grid) / sizeof(grid[0]);
    for (int i = 0; i < grid_size; i++) {
        float area = gaussian_quadrature_2d(x, A, N, grid[i]);
        float error = fabsf(area - 144.0f);
        
        if (error < 1e-15f) {
            fprintf(fp, "%d   1e-16\n", grid[i]);
        } else {
            fprintf(fp, "%d   %.15f\n", grid[i], error);
        }
    }
    fprintf(fp, "\n\n"); // 寫入兩個換行，區隔數據塊 (Gnuplot index 需求)
    
    fprintf(fp, "# N=%d ( relative error )\n", N); // 寫入標題
    for(int i = 0; i < grid_size; i++) {
        float area = gaussian_quadrature_2d(x, A, N, grid[i]);
        float relative_error = fabsf(area - 144.0) / 144.0;

        if (relative_error < 1e-15) {
            fprintf(fp, "%d   1e-16\n", grid[i]);
        } else {
            fprintf(fp, "%d   %.15f\n", grid[i], relative_error);
        }
    }
    fprintf(fp, "\n\n");
}

int main() {
    float area;
    FILE *fp = fopen("../float.txt", "w");
    
    write_block(fp, 2, x2, A2);
    write_block(fp, 3, x3, A3);
    write_block(fp, 4, x4, A4);
    write_block(fp, 5, x5, A5);
    write_block(fp, 6, x6, A6);
    write_block(fp, 7, x7, A7);

    return 0;
}
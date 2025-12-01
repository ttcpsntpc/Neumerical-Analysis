#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.14159265358979323846

int r, n, m[5] = {5, 6, 7, 8, 9}; // degree of polynomial
double A[16][16]; // coefficient matrix
double B[16][16]; // A^T * A
double cx[16]; // unknown vector
double cy[16]; // unknown vector
double dx[16]; // A^T * x
double dy[16]; // A^T * y
double t[16]; // sample points
double x[16], y[16]; // answer points

void initial(int degree) {
    r = 10;  // radius
    n = 15; // divided circle into n parts -> n+1 sample points
    for(int i = 0; i <= n; i++) {
        t[i] = 2 * PI / n * i;
        x[i] = r * cos(2 * PI / n * i);
        y[i] = r * sin(2 * PI / n * i);

        for(int j = 0; j <= degree; j++) {
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

void horner_s_algorithm(int samples, int degree) {
    FILE *fp = fopen("gaussian.txt", "w");
    double ps = 2 * PI / samples;
    for(int i = 0; i < samples; i++) {
        double px = ps * i;
        double py = ps * i;
        for(int j = degree; j >= 0; j--) {
            px = cx[j] + px * px;
            py = cy[j] + py * py;
        }
    }
}

void gnuplot();

int main() {
    initial(m[2]);
    gaussian_elimination(n + 1, m[2] + 1);
    printf("%lfx^7 + %lfx^6 + %lfx^5 + %lfx^4 + %lfx^3 + %lfx^2 + %lfx + %lf\n", cx[7], cx[6], cx[5], cx[4], cx[3], cx[2], cx[1], cx[0]); 
    printf("%lfy^7 + %lfy^6 + %lfy^5 + %lfy^4 + %lfy^3 + %lfy^2 + %lfy + %lf\n", cy[7], cy[6], cy[5], cy[4], cy[3], cy[2], cy[1], cy[0]);
    horner_s_algorithm();
    gnuplot();
    return 0;
}

void gnuplot() {
    // r*cos(x)
    FILE* gnu = _popen("gnuplot -persist", "w");
    fprintf(gnu, "set terminal gif animate delay 50\n");
    fprintf(gnu, "set output 'gauss x.gif'\n");
    fprintf(gnu, "set xrange [0:2*pi]\n");
    fprintf(gnu, "set yrange [-10:10]\n");
    fprintf(gnu, "set xlabel 'x'\n");
    fprintf(gnu, "set ylabel 'y'\n");
    fprintf(gnu, "plot 10*cos(x) title 'r*cos(x)'\n");
    fprintf(gnu, "replot %lf*x**7 + %lf*x**6 + %lf*x**5 + %lf*x**4 + %lf*x**3 + %lf*x**2 + %lf*x + %lf title 'gauss-Jordan %d deg p(x)'\n", cx[7], cx[6], cx[5], cx[4], cx[3], cx[2], cx[1], cx[0], m[2]);
    _pclose(gnu);
    // r*sin(y)
    gnu = _popen("gnuplot -persist", "w");
    fprintf(gnu, "set terminal gif animate delay 50\n");
    fprintf(gnu, "set output 'gauss y.gif'\n");
    fprintf(gnu, "set xrange [0:2*pi]\n");
    fprintf(gnu, "set yrange [-10:10]\n");
    fprintf(gnu, "set xlabel 'x'\n");
    fprintf(gnu, "set ylabel 'y'\n");
    fprintf(gnu, "plot 10*sin(x) title 'r*sin(y)'\n");
    fprintf(gnu, "replot %lf*x**7 + %lf*x**6 + %lf*x**5 + %lf*x**4 + %lf*x**3 + %lf*x**2 + %lf*x + %lf title 'gauss-Jordan %d deg p(x)'\n", cy[7], cy[6], cy[5], cy[4], cy[3], cy[2], cy[1], cy[0], m[2]);
    _pclose(gnu);
    // circle
    // circle with polynomial approximation
    gnu = _popen("gnuplot -persist", "w");
    fprintf(gnu, "set terminal gif animate delay 50\n");
    fprintf(gnu, "set output 'gauss circle.gif'\n");
    fprintf(gnu, "set parametric\n");
    fprintf(gnu, "set samples 50\n");
    fprintf(gnu, "set trange [0:2*pi]\n");
    fprintf(gnu, "set xrange [-12:12]\n");
    fprintf(gnu, "set yrange [-12:12]\n");
    fprintf(gnu, "set xlabel 'x'\n");
    fprintf(gnu, "set ylabel 'y'\n");
    fprintf(gnu, "set size square\n");
    // 真正的圓
    fprintf(gnu, "plot 10*cos(t), 10*sin(t) title 'circle'\n");
    // polynomial approximation
    fprintf(gnu,
            "replot (%lf*t**7 + %lf*t**6 + %lf*t**5 + %lf*t**4 + %lf*t**3 + %lf*t**2 + %lf*t + %lf), "
            "(%lf*t**7 + %lf*t**6 + %lf*t**5 + %lf*t**4 + %lf*t**3 + %lf*t**2 + %lf*t + %lf) "
            "title 'gauss-Jordan %d deg'\n",
            cx[7], cx[6], cx[5], cx[4], cx[3], cx[2], cx[1], cx[0],
            cy[7], cy[6], cy[5], cy[4], cy[3], cy[2], cy[1], cy[0],
            m[2]);
    _pclose(gnu);

}

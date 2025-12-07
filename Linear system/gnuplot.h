#include <stdio.h>

void gnuplot_gauss() {
    // 1. plot x(t)
    FILE* gnu = _popen("gnuplot -persist", "w");
    fprintf(gnu, "set terminal gif animate delay 50\n");
    fprintf(gnu, "set output 'gauss x.gif'\n");
    fprintf(gnu, "set xrange [0:2*pi]\n");
    fprintf(gnu, "set yrange [-12:12]\n");
    fprintf(gnu, "set xlabel 't'\n");
    fprintf(gnu, "set ylabel 'x'\n");
    
    fprintf(gnu, "plot 10*cos(x) lc rgb 'blue' title 'Target: 10*cos(t)', \
                  '50p_gaussian.txt' index 2 using 1:2 with lines lw 2 lc rgb 'red' title 'Poly approx'\n");
    _pclose(gnu);

    // 2. plot y(t)
    gnu = _popen("gnuplot -persist", "w");
    fprintf(gnu, "set terminal gif animate delay 50\n");
    fprintf(gnu, "set output 'gauss y.gif'\n");
    fprintf(gnu, "set xrange [0:2*pi]\n");
    fprintf(gnu, "set yrange [-12:12]\n");
    fprintf(gnu, "set xlabel 't'\n");
    fprintf(gnu, "set ylabel 'y'\n");
    
    fprintf(gnu, "plot 10*sin(x) lc rgb 'blue' title 'Target: 10*sin(t)', \
                  '50p_gaussian.txt' index 2 using 1:3 with lines lw 2 lc rgb 'red' title 'Poly approx'\n");
    _pclose(gnu);

    // 3. plot parametric (x(t), y(t)) -> circle
    gnu = _popen("gnuplot -persist", "w");
    fprintf(gnu, "set terminal gif animate delay 50\n");
    fprintf(gnu, "set output 'gauss circle.gif'\n");
    fprintf(gnu, "set size square\n"); 
    fprintf(gnu, "set xrange [-15:15]\n");
    fprintf(gnu, "set yrange [-15:15]\n");
    fprintf(gnu, "set xlabel 'x'\n");
    fprintf(gnu, "set ylabel 'y'\n");
    fprintf(gnu, "set parametric\n");
    fprintf(gnu, "set trange [0:2*pi]\n");
    
    fprintf(gnu, "plot 10*cos(t), 10*sin(t) lc rgb 'blue' title 'Target Circle', \
                  '50p_gaussian.txt' index 2 using 2:3 with lines lw 2 lc rgb 'red' title 'Poly approx'\n");
                  
    _pclose(gnu);
}

void plot_error() {
    // plot x 2-norm error vs degree
    FILE* gnu = _popen("gnuplot -persist", "w");
    fprintf(gnu, "set terminal pngcairo size 1024,768 enhanced font 'Verdana,12'\n");
    fprintf(gnu, "set output 'x(t) 2-norm error.png'\n");

    fprintf(gnu, "set xrange [3:15]\n");
    fprintf(gnu, "set logscale y\n"); 
    fprintf(gnu, "set format y '10^{%%L}'\n");
    fprintf(gnu, "set yrange [1e-7:20]\n");
    
    fprintf(gnu, "set title 'x(t) 2-norm Errors'\n");
    fprintf(gnu, "set xlabel 'degree of polynomial'\n");
    fprintf(gnu, "set ylabel '2-norm'\n");
    fprintf(gnu, "set grid\n");
    
    fprintf(gnu, "plot 'error_x.txt' index 0 using 1:2 with linespoints lw 2 pt 7 title 'Gaussian (New)', \
                       'error_x.txt' index 1 using 1:2 with linespoints lw 2 pt 5 title 'QR (New)', \
                       'error_x.txt' index 2 using 1:2 with linespoints lw 2 pt 9 title 'QR (Origin)'\n");
    _pclose(gnu);

    // plot y 2-norm error vs degree
    gnu = _popen("gnuplot -persist", "w");
    fprintf(gnu, "set terminal pngcairo size 1024,768 enhanced font 'Verdana,12'\n");
    fprintf(gnu, "set output 'y(t) 2-norm error.png'\n");

    fprintf(gnu, "set xrange [3:15]\n");
    fprintf(gnu, "set logscale y\n"); 
    fprintf(gnu, "set format y '10^{%%L}'\n");
    fprintf(gnu, "set yrange [1e-7:20]\n");

    fprintf(gnu, "set title 'y(t) 2-norm Errors'\n");
    fprintf(gnu, "set xlabel 'degree of polynomial'\n");
    fprintf(gnu, "set ylabel '2-norm'\n");
    fprintf(gnu, "set grid\n");
    
    fprintf(gnu, "plot 'error_y.txt' index 0 using 1:2 with linespoints lw 2 pt 7 title 'Gaussian (New)', \
                       'error_y.txt' index 1 using 1:2 with linespoints lw 2 pt 5 title 'QR (New)', \
                       'error_y.txt' index 2 using 1:2 with linespoints lw 2 pt 9 title 'QR (Origin)'\n");
    _pclose(gnu);

    // plot x infinity-norm error vs degree
    gnu = _popen("gnuplot -persist", "w");
    fprintf(gnu, "set terminal pngcairo size 1024,768 enhanced font 'Verdana,12'\n");
    fprintf(gnu, "set output 'x(t) infinite-norm error.png'\n");

    fprintf(gnu, "set xrange [3:15]\n");
    fprintf(gnu, "set logscale y\n"); 
    fprintf(gnu, "set format y '10^{%%L}'\n");
    fprintf(gnu, "set yrange [1e-7:20]\n");

    fprintf(gnu, "set title 'x(t) infinite-norm Errors'\n");
    fprintf(gnu, "set xlabel 'degree of polynomial'\n");
    fprintf(gnu, "set ylabel 'infinite-norm'\n");
    fprintf(gnu, "set grid\n");
    
    fprintf(gnu, "plot 'error_x.txt' index 0 using 1:3 with linespoints lw 2 pt 7 title 'Gaussian (New)', \
                       'error_x.txt' index 1 using 1:3 with linespoints lw 2 pt 5 title 'QR (New)', \
                       'error_x.txt' index 2 using 1:3 with linespoints lw 2 pt 9 title 'QR (Origin)'\n");
    _pclose(gnu);

    // plot y infinity-norm error vs degree
    gnu = _popen("gnuplot -persist", "w");
    fprintf(gnu, "set terminal pngcairo size 1024,768 enhanced font 'Verdana,12'\n");
    fprintf(gnu, "set output 'y(t) infinite-norm error.png'\n");

    fprintf(gnu, "set xrange [3:15]\n");
    fprintf(gnu, "set logscale y\n"); 
    fprintf(gnu, "set format y '10^{%%L}'\n");
    fprintf(gnu, "set yrange [1e-7:20]\n");

    fprintf(gnu, "set title 'y(t) infinite-norm Errors'\n");
    fprintf(gnu, "set xlabel 'degree of polynomial'\n");
    fprintf(gnu, "set ylabel 'infinite-norm'\n");
    fprintf(gnu, "set grid\n");
    
    fprintf(gnu, "plot 'error_y.txt' index 0 using 1:3 with linespoints lw 2 pt 7 title 'Gaussian (New)', \
                       'error_y.txt' index 1 using 1:3 with linespoints lw 2 pt 5 title 'QR (New)', \
                       'error_y.txt' index 2 using 1:3 with linespoints lw 2 pt 9 title 'QR (Origin)'\n");
    _pclose(gnu);
}
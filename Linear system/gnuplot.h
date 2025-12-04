#include <stdio.h>

void gnuplot_gauss() {
    // 1. ø�s X ���q (x vs t)
    FILE* gnu = _popen("gnuplot -persist", "w");
    fprintf(gnu, "set terminal gif animate delay 50\n");
    fprintf(gnu, "set output 'gauss x.gif'\n");
    fprintf(gnu, "set xrange [0:2*pi]\n");
    fprintf(gnu, "set yrange [-12:12]\n");
    fprintf(gnu, "set xlabel 't'\n");
    fprintf(gnu, "set ylabel 'x'\n");
    // using 1:2 �N���ϥβ�1�C(t)����b�A��2�C(x)���a�b
    fprintf(gnu, "plot 10*cos(x) lc rgb 'blue' title 'Target: 10*cos(t)', \
                  '50p_gaussian.txt' index 2 using 1:2 with lines lw 2 lc rgb 'red' title 'Poly approx'\n");
    _pclose(gnu);

    // 2. ø�s Y ���q (y vs t)
    gnu = _popen("gnuplot -persist", "w");
    fprintf(gnu, "set terminal gif animate delay 50\n");
    fprintf(gnu, "set output 'gauss y.gif'\n");
    fprintf(gnu, "set xrange [0:2*pi]\n");
    fprintf(gnu, "set yrange [-12:12]\n");
    fprintf(gnu, "set xlabel 't'\n");
    fprintf(gnu, "set ylabel 'y'\n");
    // using 1:3 �N���ϥβ�1�C(t)����b�A��3�C(y)���a�b
    fprintf(gnu, "plot 10*sin(x) lc rgb 'blue' title 'Target: 10*sin(t)', \
                  '50p_gaussian.txt' index 2 using 1:3 with lines lw 2 lc rgb 'red' title 'Poly approx'\n");
    _pclose(gnu);

    // 3. ø�s �ѼƦ���� (y vs x)
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
    // using 2:3 �N���ϥβ�2�C(x)����b�A��3�C(y)���a�b
    fprintf(gnu, "plot 10*cos(t), 10*sin(t) lc rgb 'blue' title 'Target Circle', \
                  '50p_gaussian.txt' index 2 using 2:3 with lines lw 2 lc rgb 'red' title 'Poly approx'\n");
                  
    _pclose(gnu);
}
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'A-conjugate_heatmap.png'

set title "A-Conjugate Heatmap"
set xlabel "d_i"
set ylabel "d_j"
set cblabel "Log10( |Value| )"
set xtics 1
set ytics 1
set palette defined (0 "dark-blue", 1 "blue", 2 "cyan", 3 "green", 4 "yellow", 5 "red")

plot '../src/conjugate_relation.txt' matrix using 1:2:(log10(abs($3) + 1e-16)) with image
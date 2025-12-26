# 畫迭代次數比較圖
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'iteration_comparison.png'

set title "Iteration Comparison: CG vs SOR vs SSOR+CG" font ",14"
set xlabel "Matrix Size (N)"
set ylabel "Number of Iterations"

set grid

# 設定圖例位置
set key left top

set logscale y
# 設定 X 軸刻度 (顯示 20, 30, 40...)
set xtics 10

plot "../src/iteration.txt" index 0 using 1:2 with linespoints linewidth 2 pointtype 7 title "Conjugate Gradient", \
     "../src/iteration.txt" index 1 using 1:2 with linespoints linewidth 2 pointtype 5 title "SOR", \
     "../src/iteration.txt" index 2 using 1:2 with linespoints linewidth 2 pointtype 9 title "SSOR + CG"


# 畫時間比較圖
set output 'time_comparison.png'
set title "Execution Time Comparison (Seconds)" font ",14"
set ylabel "Time (sec)"

unset logscale y 

plot "../src/time.txt" index 0 using 1:2 with linespoints linewidth 2 pointtype 7 title "Conjugate Gradient", \
     "../src/time.txt" index 1 using 1:2 with linespoints linewidth 2 pointtype 5 title "SOR", \
     "../src/time.txt" index 2 using 1:2 with linespoints linewidth 2 pointtype 9 title "SSOR + CG"

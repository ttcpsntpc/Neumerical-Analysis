set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'solution_heatmap.png'

set title "Solutions Heatmap"
set xlabel "Dimension Index (N = 20)"
set ylabel "Iteration Index"
set cblabel "Log10( |Value| )"
set xtics 1
set palette defined (0 "dark-blue", 1 "blue", 2 "cyan", 3 "green", 4 "yellow", 5 "red")

# 1. abs($3): 取絕對值，因為 log 不能吃負數。
# 2. + 1e-16: 加一個極小值，防止原本是 0 的時候 log(0) 報錯。
plot '../src/solution.txt' matrix rowheaders using 1:2:(log10(abs($3) + 1e-16)) with image
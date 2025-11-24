# 設定輸出
set terminal wxt 0 size 900,600 enhanced font 'Verdana,10' persist
set title "Double Precision"
set xlabel "Grid Size (n x n)"
set ylabel "Absolute Error"

# 設定座標軸
set logscale y 10
set format y "10^{%L}"  # 讓 Y 軸顯示成 10 的次方格式，比較好看

# 設定 Y 軸範圍 (因為數據很小，固定範圍比較容易看)
set yrange [1e-16:1e-11]
set grid

# 圖例放在圖表外面，避免擋住數據
set key outside right center

# 繪圖
plot "double.txt" index 0 w lp pt 7 ps 1.5 t "N=2", \
     "double.txt" index 2 w lp pt 5 ps 1.5 t "N=3", \
     "double.txt" index 4 w lp pt 9 ps 1.5 t "N=4", \
     "double.txt" index 6 w lp pt 13 ps 1.5 t "N=5", \
     "double.txt" index 8 w lp pt 11 ps 1.5 t "N=6", \
     "double.txt" index 10 w lp pt 6 ps 1.5 t "N=7"

# --- 設定第二個視窗 (Window 1) ---
set terminal wxt 1 size 900,600 enhanced font 'Verdana,10' title "Window 2: Linear Scale" persist
set title "Double Precision"
set xlabel "Grid Size (n x n)"
set ylabel "Relative Error"

set yrange [1e-16:1e-13]

# 畫第二張圖
plot "double.txt" index 1 w lp pt 7 ps 1.5 t "N=2", \
     "double.txt" index 3 w lp pt 5 ps 1.5 t "N=3", \
     "double.txt" index 5 w lp pt 9 ps 1.5 t "N=4", \
     "double.txt" index 7 w lp pt 13 ps 1.5 t "N=5", \
     "double.txt" index 9 w lp pt 11 ps 1.5 t "N=6", \
     "double.txt" index 11 w lp pt 6 ps 1.5 t "N=7"
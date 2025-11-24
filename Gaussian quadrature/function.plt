set terminal wxt size 800,600 enhanced font 'Verdana,10' persist

set title "3D Surface: f(x,y) = 3*sin(8πx)*sin(8πy) + x + y + 1"
set xlabel "X axis"
set ylabel "Y axis"
set zlabel "Z value"

f(x,y) = 3 * sin(8.0 * pi * x) * sin(8.0 * pi * y) + x + y + 1

# 取樣數
set isosamples 150, 150

set xrange [2:6]
set yrange [2:6]

set hidden3d      # 隱藏被遮擋的線條 (讓它看起來像實體)
set pm3d          # 加上色彩地圖 (Palette Mapping)，依照高度上色
set view 60, 30   # 設定視角 (角度)

splot f(x,y)
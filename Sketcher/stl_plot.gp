set terminal qt size 800,600
set xlabel 'X'
set ylabel 'Y'
set zlabel 'Z'
set view equal xyz
splot 'stl_plot.dat' with linespoints pointtype 0 linecolor 'blue' title 'STL Visualization'
pause -1

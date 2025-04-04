set terminal qt size 800,600
set xlabel 'X'
set ylabel 'Y'
set zlabel 'Z'
set view equal xyz
splot 'data/line3D.dat' using 1:2:3 with linespoints pointtype 7 linecolor 'blue' title 'Line3D'
pause -1

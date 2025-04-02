set terminal qt size 800,600
set xlabel 'X'
set ylabel 'Y'
set zlabel 'Z'
set view equal xyz
splot 'data/cylinder.dat' using 1:2:3 with linespoints pointtype 7 linecolor 'black' title 'Cylinder'
pause -1

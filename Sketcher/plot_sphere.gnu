set terminal wxt
set parametric
set hidden3d
set mouse
set urange [0:2*pi]
set vrange [0:pi]
set xlabel 'X'
set ylabel 'Y'
set zlabel 'Z'
set view equal xyz
r = 13
splot r*sin(v)*cos(u), r*sin(v)*sin(u), r*cos(v) with lines

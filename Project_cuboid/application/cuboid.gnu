set terminal png
set output 'cuboid.png'
set parametric
set hidden3d
splot '-' with lines
0 0 0
10 0 0
10 20 0
0 20 0
0 0 0
e

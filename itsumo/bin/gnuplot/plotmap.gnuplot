plot 'links.txt' using 1:2:3:4 with vectors head filled size 0.05,30,1 lw 1.5  lt 1;
replot 'shortest_path.txt' using 1:2:3:4 with vectors head filled size 0.05,30,1 lw 1.5  lt 2;
replot 'nodes.txt' u 1:2:3 with labels;
set terminal png size 4000, 3000;
 set size 1, 1
set terminal svg;
 set size 1, 1
set output 'graph.svg';
replot

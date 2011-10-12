#!/bin/sh
#script muito burro pra criar os graficos de velocidade media, densidade media e no de carros parados 
#incoming lanes X outgoing lanes

#velocidade media
touch temporary.param
echo set terminal postscript landscape noenhanced monochrome dashed defaultplex \"Helvetica\" 14 >> temporary.param
echo set output \'$1_speed.ps\' >> temporary.param
echo set tics out >> temporary.param
echo set title \"Avg Speed - TL $1\" >> temporary.param
echo set xlabel \"Steps\" >> temporary.param
echo set ylabel \"Speed\" >> temporary.param
echo plot \"$1\" using 1:4 title \"Incoming Lanes\" w l, \\ >> temporary.param
echo \"$1\" using 1:7 title \"Outgoing Lanes\" w l >> temporary.param

gnuplot temporary.param


#densidade media
echo set terminal postscript landscape noenhanced monochrome dashed defaultplex \"Helvetica\" 14 > temporary.param
echo set output \'$1_density.ps\' >> temporary.param
echo set tics out >> temporary.param
echo set title \"Avg Density - TL $1\" >> temporary.param
echo set xlabel \"Steps\" >> temporary.param
echo set ylabel \"Density\" >> temporary.param
echo plot \"$1\" using 1:5 title \"Incoming Lanes\" w l, \\ >> temporary.param
echo \"$1\" using 1:8 title \"Outgoing Lanes\" w l >> temporary.param

gnuplot temporary.param


#carros parados
touch temporary.param

echo set terminal postscript landscape noenhanced monochrome dashed defaultplex \"Helvetica\" 14 > temporary.param
echo set output \'$1_stopped.ps\' >> temporary.param
echo set tics out >> temporary.param
echo set title \"Avg Stopped Cars - TL $1\" >> temporary.param
echo set xlabel \"Steps\" >> temporary.param
echo set ylabel \"Stopped Cars\" >> temporary.param
echo plot \"$1\" using 1:6 title \"Incoming Lanes\" w l, \\ >> temporary.param
echo \"$1\" using 1:9 title \"Outgoing Lanes\" w l >> temporary.param

gnuplot temporary.param

rm -f temporary.param
#!/usr/bin/awk -f
#File: AvgStdDev_totalStopped.sh
# Written 08-2007  by Denise de Oliveira
# This is an AWK script to calculate the average and standard deviation of the total stoped cars.
# Usage: ./AvgStdDev_totalStopped.sh total_stopped_cars_in_network.log

BEGIN { AVG = 0.0 }
{ AVG += $2; 
  keep[NR] = $2;}
END { 
  AVG /= NR;
  SUM = 0.0;
  for (i = 1; i <= NR; i++)
    SUM += (keep[i] - AVG) * (keep[i] - AVG);
  print "#avg\t std.dev";
  printf ("%.2f\t %.2f\t \n", AVG,sqrt(SUM / (NR-1)));  
} 

#!/usr/bin/awk -f
#File: MobAvg_totalStopped.sh 
# Written 08-2007  by Denise de Oliveira 
# This is an AWK script to calculate the mobile average of total stoped cars. 
# The default interval  of time steps is set to 30, change the variable 
# 'interval' for a different time window be used in the average computation.   
# Usage: ./MobAvg_totalStopped.sh total_stopped_cars_in_network.log
  
BEGIN { AVG = 0.0 ; print "#step\t mob. avg"; interval= 60;}
{ AVG += $2; 
  count+=1;
  if(count==interval){
	printf("%d \t %.2f \n",$1,AVG/count);\
	AVG = 0.0;
	count =0;
  } 
} 

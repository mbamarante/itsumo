#!/bin/bash
#clp=./classes
clp=./classes:./libs/jgraph.jar:./libs/jgraphaddons.jar
#cmd="java -Xmx120000K -classpath $clp testbed.sensorCSP \
#cmd="java -classpath $clp testbed.ocop.cycleCutset.CSPSolver \
cmd="java -classpath $clp frodo.OCOP.DPOP.graphics.OCOPViewer \
$*"
echo $cmd
$cmd

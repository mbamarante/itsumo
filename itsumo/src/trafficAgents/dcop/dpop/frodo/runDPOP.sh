#!/bin/bash

#java -Xmx512000K -classpath ./classes:./libs/jgraph.jar:./libs/jgraphaddons.jar frodo.OCOP.DPOP.TesterDPOP tests/10.txt.1 result.txt 0
java -Xmx512000K -classpath ./classes:./libs/jgraph.jar:./libs/jgraphaddons.jar frodo.OCOP.DPOP.TesterDPOP $1 $2 $3


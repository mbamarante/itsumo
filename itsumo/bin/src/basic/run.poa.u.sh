#!/bin/bash

tabela="travels"

for ((  i = 1 ;  i <= 1;  i++  ))
do
  echo "*** RUNNING ROUND $i ***"
	./itsumo ../../input/config.poa.u_20k.gnr.xml $i $tabela
	./itsumo ../../input/config.poa.u_20k.gr.xml $i $tabela
	./itsumo ../../input/config.poa.u_20k.l.xml $i $tabela
	./itsumo ../../input/config.poa.u_20k.lc.xml $i $tabela
	./itsumo ../../input/config.poa.u_20k.lr.xml $i $tabela
	./itsumo ../../input/config.poa.u_20k.lcr.xml $i $tabela

	./itsumo ../../input/config.poa.u_25k.gnr.xml $i $tabela
	./itsumo ../../input/config.poa.u_25k.gr.xml $i $tabela
	./itsumo ../../input/config.poa.u_25k.l.xml $i $tabela
	./itsumo ../../input/config.poa.u_25k.lc.xml $i $tabela
	./itsumo ../../input/config.poa.u_25k.lr.xml $i $tabela
	./itsumo ../../input/config.poa.u_25k.lcr.xml $i $tabela

	./itsumo ../../input/config.poa.u_40k.gnr.xml $i $tabela
	./itsumo ../../input/config.poa.u_40k.gr.xml $i $tabela
	./itsumo ../../input/config.poa.u_40k.l.xml $i $tabela
	./itsumo ../../input/config.poa.u_40k.lc.xml $i $tabela
	./itsumo ../../input/config.poa.u_40k.lr.xml $i $tabela
	./itsumo ../../input/config.poa.u_40k.lcr.xml $i $tabela
done

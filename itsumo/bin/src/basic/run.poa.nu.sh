#!/bin/bash

tabela="travels"

for ((  i = 1 ;  i <= 1;  i++  ))
do
  echo "*** RUNNING ROUND $i ***"
	./itsumo ../../input/config.poa.nu_20k.gnr.xml $i $tabela
	./itsumo ../../input/config.poa.nu_20k.gr.xml $i $tabela
	./itsumo ../../input/config.poa.nu_20k.l.xml $i $tabela
	./itsumo ../../input/config.poa.nu_20k.lc.xml $i $tabela
	./itsumo ../../input/config.poa.nu_20k.lr.xml $i $tabela
	./itsumo ../../input/config.poa.nu_20k.lcr.xml $i $tabela

	./itsumo ../../input/config.poa.nu_25k.gnr.xml $i $tabela
	./itsumo ../../input/config.poa.nu_25k.gr.xml $i $tabela
	./itsumo ../../input/config.poa.nu_25k.l.xml $i $tabela
	./itsumo ../../input/config.poa.nu_25k.lc.xml $i $tabela
	./itsumo ../../input/config.poa.nu_25k.lr.xml $i $tabela
	./itsumo ../../input/config.poa.nu_25k.lcr.xml $i $tabela

	./itsumo ../../input/config.poa.nu_40k.gnr.xml $i $tabela
	./itsumo ../../input/config.poa.nu_40k.gr.xml $i $tabela
	./itsumo ../../input/config.poa.nu_40k.l.xml $i $tabela
	./itsumo ../../input/config.poa.nu_40k.lc.xml $i $tabela
	./itsumo ../../input/config.poa.nu_40k.lr.xml $i $tabela
	./itsumo ../../input/config.poa.nu_40k.lcr.xml $i $tabela
done

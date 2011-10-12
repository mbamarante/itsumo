#!/bin/bash

tabela="travels_delay_grid"

for ((  i = 1 ;  i <= 5;  i++  ))
do
  echo "*** RUNNING ROUND $i ***"
	./itsumo ../../input/config.6x6.nu_4200.gnr.xml $i $tabela
	./itsumo ../../input/config.6x6.nu_4200.gr.xml $i $tabela
	./itsumo ../../input/config.6x6.nu_4200.l.xml $i $tabela
	./itsumo ../../input/config.6x6.nu_4200.lc.xml $i $tabela

	./itsumo ../../input/config.6x6.nu_6k.gnr.xml $i $tabela
	./itsumo ../../input/config.6x6.nu_6k.gr.xml $i $tabela
	./itsumo ../../input/config.6x6.nu_6k.l.xml $i $tabela
	./itsumo ../../input/config.6x6.nu_6k.lc.xml $i $tabela

	./itsumo ../../input/config.6x6.nu_8k.gnr.xml $i $tabela
	./itsumo ../../input/config.6x6.nu_8k.gr.xml $i $tabela
	./itsumo ../../input/config.6x6.nu_8k.l.xml $i $tabela
	./itsumo ../../input/config.6x6.nu_8k.lc.xml $i $tabela 
done

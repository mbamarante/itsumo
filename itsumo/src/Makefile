#Itsumo Makefile

all: 
	@echo == Building all==
	@make -C tiny
	@make -C drivers
	@make -C drivers/routerlibs
	@make -C geradorPS
	@make -C socket
	@make -C basic
	@make -C interface -f Fixmake
	@make -C trafficAgents
	@mv interface/witsumo ../

	@make -C visual3D	

	@echo =================
	@echo

clean:
	@echo == Cleaning all==
	@rm -f */*.o */*/*.o
	@make clean -C tiny
	@make clean -C drivers/routerlibs
	@make clean -C drivers
	@make clean -C geradorPS
	@make clean -C socket
	@make clean -C basic
	@make clean -C interface
	@make clean -C trafficAgents
	@make clean -C visual3D

	@rm -f ../witsumo
	@echo =================

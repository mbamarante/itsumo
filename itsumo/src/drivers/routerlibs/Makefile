CC=g++
W_FLAGS= -Wall
C_FLAGS= -O2 $(W_FLAGS) -g
DRIVERS = ItsumoGraph.o geradorOD.o
	
all: $(DRIVERS)
	@echo Drivers/RouterLibs: OK
.cc.o:
	@echo Compiling $@
	@$(CC) -c $< $(C_FLAGS) -o $@ 

clean:
	@rm -f *.o 
	@echo Drivers/RouterLibs: CLEAN

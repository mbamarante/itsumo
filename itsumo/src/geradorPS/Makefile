CC=g++
W_FLAGS= -Wall
C_FLAGS= -O2 $(W_FLAGS) -g
DRIVERS = leNet.o	Main.o
	
all: $(DRIVERS)
	@echo geradorPS: OK
	@$(CC) ../tiny/*.o *.o -o geradorPS
.cc.o:
	@echo Compiling $@
	@$(CC) -c $< $(C_FLAGS) -o $@ 

clean:
	@rm -f *.o 
	@rm geradorPS
	@echo geradorPS: CLEAN

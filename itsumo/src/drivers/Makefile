CC=g++
W_FLAGS= -Wall
C_FLAGS= -O2 $(W_FLAGS) -g
DRIVERS = nasch.o braess.o fc.o irc.o routed.o

all: $(DRIVERS)
	@echo Drivers: OK
.cc.o:
	@echo Compiling $@
	@$(CC) -c $< $(C_FLAGS) -o $@ 

clean:
	@rm -f *.o 
	@echo Drivers: CLEAN

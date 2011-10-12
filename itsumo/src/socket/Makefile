OBJS = ServerSocket.o ClientSocket.o Socket.o
	
all: $(OBJS)
	@echo Socket: OK
.cpp.o:
	@echo Compiling $@
	@$(CC) -c $< $(C_FLAGS) -o $@ 

clean:
	@rm -f *.o 
	@echo Socket: CLEAN 

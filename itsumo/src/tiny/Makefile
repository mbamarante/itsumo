OBJS = tinystr.o tinyxmlerror.o tinyxmlparser.o tinyxml.o
	
all: $(OBJS)
	@echo TinyXML: OK
.cpp.o:
	@echo Compiling $@
	@$(CC) -c $< $(C_FLAGS) -o $@ 

clean:
	@rm -f *.o 
	@echo TinyXML: CLEAN


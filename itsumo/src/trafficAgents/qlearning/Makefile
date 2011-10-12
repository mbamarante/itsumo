# Requires the installation of the gsl library package:
# on Ubuntu: libgsl0-dev
# on OpenSUSE: gsl-devel 

CC=g++
W_FLAGS= -Wall
C_FLAGS= -O2 $(W_FLAGS) -g -lpthread -lgsl -lgslcblas 
NAME = start_agents

OBJS = tlagent.o local_agent.o topologyReader.o main.o 

EXTRA = ../../socket/*.o ../../tiny/*.o ../basic/common.o ../../basic/utils.o 


all: $(NAME)
	@echo q-learning: OK

$(NAME): $(OBJS)
	@$(CC) $(C_FLAGS) -o $@ $^ $(EXTRA)


.cc.o:
	@echo Compiling $@
	@$(CC) -c $< $(C_FLAGS) -o $@ 


clean:
	@rm -f *.o $(NAME)
	@rm -f *~
	@echo q-learning: CLEAN

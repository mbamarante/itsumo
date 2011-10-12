CC=g++
W_FLAGS= -Wall
C_FLAGS= $(W_FLAGS) -g -lpthread -lgsl -lgslcblas
NAME = start_agents

OBJS = ../basic/common.o ../../basic/utils.o ../qlearning/tlagent.o local_agent.o ../qlearning/topologyReader.o main.o 

EXTRA = ../../socket/*.o ../../tiny/*.o


all: $(NAME)
	@echo WLU learning: OK

$(NAME): $(OBJS)
	@$(CC) $(C_FLAGS) -o $@ $^ $(EXTRA)


.cc.o:
	@echo Compiling $@
	@$(CC) -c $< $(C_FLAGS) -o $@ 


clean:
	@rm -f *.o $(NAME)
	@rm -f *~
	@echo WLU: CLEAN

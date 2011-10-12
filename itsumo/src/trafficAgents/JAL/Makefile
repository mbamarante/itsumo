CC=g++
W_FLAGS= -Wall
C_FLAGS= -O2 $(W_FLAGS) -g -lpthread 
NAME = start_agents

OBJS = local_agent.o main.o 

EXTRA = ../../socket/*.o ../../tiny/*.o ../qlearning/tlagent.o ../qlearning/topologyReader.o ../basic/common.o ../../basic/utils.o 


all: $(NAME)
	@echo JAL: OK

$(NAME): $(OBJS)
	@$(CC) $(C_FLAGS) -o $@ $^ $(EXTRA)


.cc.o:
	@echo Compiling $@
	@$(CC) -c $< $(C_FLAGS) -o $@ 


clean:
	@rm -f *.o $(NAME)
	@rm -f *~
	@echo JAL: CLEAN

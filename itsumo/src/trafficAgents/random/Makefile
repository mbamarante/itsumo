CC=g++
W_FLAGS= -Wall
C_FLAGS= -O2 $(W_FLAGS) -g -lpthread
NAME = start_agents

OBJS = local_agent.o randomTLAgent.o  main.o


EXTRA = ../../socket/*.o ../../tiny/*.o ../basic/common.o ../../basic/utils.o ../basic/tlagent.o 


all: $(NAME)
	@echo random: OK

$(NAME): $(OBJS)
	@$(CC) $(C_FLAGS) -o $@ $^ $(EXTRA)


.cc.o:
	@echo Compiling $@
	@$(CC) -c $< -o $@ 


clean:
	@rm -f *.o $(NAME)
	@echo random: CLEAN

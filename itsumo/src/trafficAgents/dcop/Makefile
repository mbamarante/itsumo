CC=g++
W_FLAGS= -Wall
C_FLAGS= -O2 $(W_FLAGS) -g -lpthread
NAME = start_agents

OBJS = common.o tlagent.o local_agent.o main.o

EXTRA = ../../socket/*.o ../../tiny/*.o ../../basic/utils.o


all: $(NAME)
	@echo dcop: OK

$(NAME): $(OBJS)
	@$(CC) $(C_FLAGS) -o $@ $^ $(EXTRA)


.cc.o:
	@echo Compiling $@
	@$(CC) -c $< -o $@ 


clean:
	@rm -f *.o $(NAME)
	@echo dcop: CLEAN

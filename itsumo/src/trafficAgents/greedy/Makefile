CC=g++
W_FLAGS= -Wall
C_FLAGS= -O2 $(W_FLAGS) -g -lpthread
NAME = start_agents

OBJS = tlagent.o local_agent.o main.o

EXTRA = ../../socket/*.o ../../tiny/*.o ../../basic/utils.o ../basic/common.o


all: $(NAME)
	@echo greedy: OK

$(NAME): $(OBJS)
	@$(CC) $(C_FLAGS) -o $@ $^ $(EXTRA)


.cc.o:
	@echo Compiling $@
	@$(CC) -c $< $(C_FLAGS) -o $@ 


clean:
	@rm -f *.o $(NAME)
	@rm -f *~
	@echo greedy: CLEAN

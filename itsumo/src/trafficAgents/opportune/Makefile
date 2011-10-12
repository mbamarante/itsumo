# Requires the installation of the gsl library package:
# on Ubuntu: libgsl0-dev
# on OpenSUSE: gsl-devel 

CC=g++
#W_FLAGS= -Wall
C_FLAGS= -g -lpthread -lgsl -lgslcblas -lm -O2
NAME = start_agents

OBJS = util.o Message.o local_agent.o OpLearner.o CQValue.o stateRecord.o DQTable.o rewardHistory.o rewardRecord.o SAComponent.o StringTokenizer.o main.o 


EXTRA = ../../socket/*.o ../../tiny/*.o ../../basic/utils.o ../qlearning/tlagent.o ../qlearning/topologyReader.o ../basic/common.o


 
all: $(NAME)
	@echo OPPORTUNE: OK

$(NAME): $(OBJS)
	@$(CC) $(C_FLAGS) -o $@ $^ $(EXTRA)


.cc.o:
	@echo Compiling $@
	@$(CC) -c $< $(C_FLAGS) -o $@ 


clean:
	@rm -f *.o $(NAME)
	@rm -f *~
	@echo OPPORTUNE: CLEAN

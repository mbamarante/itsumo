CC=g++
W_FLAGS= -Wall
C_FLAGS= -O2 $(W_FLAGS) -g
NAME = itsumo

OBJS=itsumo.o utils.o topology.o node.o topologyelement.o \
lane.o section.o laneset.o  calibrator.o\
car.o trafficlight.o network.o driver.o simulation.o parse_network.o\
driver-loader.o 

EXTRA = ../tiny/*.o ../drivers/*.o ../drivers/routerlibs/*.o ../socket/*.o


all: $(NAME)
	@echo ITSUMO: OK


$(NAME): $(OBJS)
	@$(CC) $(C_FLAGS) -o $@ $^ $(EXTRA) -lsqlite3 -I/usr/include/mysql -L/usr/lib/mysql -lmysqlclient


.cc.o:
	@echo Compiling $@
	@$(CC) -c $< $(C_FLAGS) -o $@ 

clean:
	@rm -f *.o $(NAME)
	@echo ITSUMO: CLEAN

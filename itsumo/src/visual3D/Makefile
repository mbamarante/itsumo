INCLUDE = -I/usr/X11R6/include/
LIBDIR  = -L/usr/X11R6/lib -L.


COMPILERFLAGS = -Wall -O3
#-g
CC = g++
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)
LIBRARIES =  -lglut -lGLU -lm -lX11 -lglpng
SOURCE = visual.cpp nodo.cpp aux.cpp zpr.c section.cpp laneset.cpp fontes.cpp

visual: $(SOURCE)
	$(CC) $(CFLAGS) -o visual $(SOURCE) $(LIBDIR) $(LIBRARIES)

visual.o:	definitions.h
section.cpp:	definitions.h section.h
laneset.cpp:	definitions.h laneset.h
nodo.cpp:	definitions.h nodo.h
aux.cpp:	definitions.h aux.h
fontes.cpp:	definitions.h fontes.h

clean:
	rm -f visual 

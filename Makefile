OPTIONS_DEBUG=-ggdb -g3 -Wall
OPTIONS_RELEASE=-O3 
OPTIONS=$(OPTIONS_RELEASE)
INCPATH=/home/bayashi/Coding/Include
LIBPATH=/home/bayashi/Coding/Include

all : main

main: main.o bcurve.o $(LIBPATH)/pbmath.o $(LIBPATH)/shapoid.o $(LIBPATH)/gset.o Makefile 
	gcc $(OPTIONS) main.o bcurve.o $(LIBPATH)/pbmath.o $(LIBPATH)/shapoid.o $(LIBPATH)/gset.o -o main -lm

main.o : main.c bcurve.h Makefile
	gcc $(OPTIONS) -I$(INCPATH) -c main.c

bcurve.o : bcurve.c bcurve.h $(INCPATH)/pbmath.h $(INCPATH)/shapoid.h $(INCPATH)/gset.h Makefile
	gcc $(OPTIONS) -I$(INCPATH) -c bcurve.c

clean : 
	rm -rf *.o main

valgrind :
	valgrind -v --track-origins=yes --leak-check=full --gen-suppressions=yes --show-leak-kinds=all ./main

install :
	cp bcurve.h ../Include; cp bcurve.o ../Include
	

#Makefile
##############################################################
# author: Hannaneh Barahouei Pasandi 
# MAC layer Simulator
#
# binary: simulator 
##############################################################
CC=g++
simulator: updated.o
	$(CC) -o simulator updated.o -lm

updated.o: updated.cpp updated.h

clean:
	rm -f *.o simulator

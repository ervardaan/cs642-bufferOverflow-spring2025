#
# compsci642 - F24 Project Bufferoverflow
# Makefile - makefile for the assignment
#

# Environment Setup
INCLUDES=-I.
CC=./642cc
CFLAGS=-I. -c -g -Wall $(INCLUDES)
LINKARGS=-g
LIBS=-lcompsci642

# Suffix rules
.SUFFIXES: .c .o

.c.o:
		$(CC) $(CFLAGS)  -o $@ $<

# Files

TARGETS=task1-attack task2-attack task3-attack task4-attack

# Productions

all : $(TARGETS)

task1-attack: cs642-task1.o 
	$(CC) $(LINKARGS) cs642-task1.o -o $@ $(LIBS)
task2-attack: cs642-task2.o 
	$(CC) $(LINKARGS) cs642-task2.o -o $@ $(LIBS)
task3-attack: cs642-task3.o 
	$(CC) $(LINKARGS) cs642-task3.o -o $@ $(LIBS)
task4-attack: cs642-task4.o 
	$(CC) $(LINKARGS) cs642-task4.o -o $@ $(LIBS)

clean:
	rm -f *.o *~ $(TARGETS)
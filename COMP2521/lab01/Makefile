# COMP2521 ... Lab 01 Makefile

CC = gcc
CFLAGS = -Wall -g -Werror

.PHONY: all
all: useIntList randList

useIntList: useIntList.o IntList.o
useIntList.o: useIntList.c IntList.h
IntList.o: IntList.c IntList.h

randList: randList.o

.PHONY: clean
clean:
	rm -f useIntList useIntList.o IntList.o
	rm -f randList randList.o


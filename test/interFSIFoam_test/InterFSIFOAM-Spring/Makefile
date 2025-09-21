#!/bin/bash

CC	= mpic++
#CC      = icx
CFLAGS	= -std=c++14 -O3 -I../../../../../coupling/MUI/src

SCR = $(wildcard *.cpp)
EXE = $(SCR:.cpp=)

default: $(EXE)

% : %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXE)

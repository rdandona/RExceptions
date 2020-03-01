SHELL = /bin/sh

OBJS =  exce2.o exce3.o exce4.o
SOURCES = exec3.cxx 
CXXFLAG = -Wall -g
CXX = g++
INCLUDE =
LIBS = -lm

all: hello

hello: $(OBJS)
   ${CXX} ${CXXFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

.PHONY: clean
clean:
   rm -f *.o core *.core

.cxx.o: $(wildcard *.cxx)
   ${CXX} ${CXXFLAGS} ${INCLUDES} -c $<

LDLIBS = -lglut -lGLU -lGL -lX11 -lpthread -lXrandr -lXi -lpng -lm

CPPFLAGS= $(INCDIRS) -O3

TARGETS = main

SRCS = main.cpp

OBJS =  $(SRCS:.cpp=.o)

CXX = g++

default: $(TARGETS)

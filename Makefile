ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell uname -s)
endif

CXX := g++
CXXFLAGS := -std=c++14 -O2 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
INCLUDES := -I/usr/local/include
LIBS := -L/usr/local/lib
OBJS :=  main.o algorithm.o couplings.o utils.o
INSTALLDIR := ../
TARGET := main

ifeq ($(detected_OS),Windows)
    CXXFLAGS += -D WIN32
endif
ifeq ($(detected_OS),Darwin)  # Mac OS X
    CXXFLAGS += -D OSX
	  LIBS += -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif
ifeq ($(detected_OS),Linux)
    CXXFLAGS += -D LINUX
	  LIBS += -Wl,-rpath,/usr/local/lib -lGL -lglfw
endif

$(TARGET): $(OBJS)
	$(CXX) $(INCLUDES) $(OBJS) -o main $(LIBS)
main.o: main.cpp state.hpp algorithm.hpp couplings.hpp utils.hpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c main.cpp  -o main.o
install:
	install $(TARGET) $(INSTALLDIR)
clean:
	-rm *.o

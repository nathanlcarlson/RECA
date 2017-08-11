ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell uname -s)
endif

CXX := g++
CXXFLAGS := -std=c++14 -O2 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
INCLUDES := -I.
LIBS := -L/usr/local/lib -lglfw


ifeq ($(detected_OS),Windows)
    CXXFLAGS += -D WIN32
endif
ifeq ($(detected_OS),Darwin)  # Mac OS X
    CXXFLAGS += -D OSX
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif
ifeq ($(detected_OS),Linux)
    CXXFLAGS += -D LINUX
	  LIBS += -Wl,-rpath,/usr/local/lib -lGL
endif

test: main.o
	$(CXX) main.o -o test $(LIBS)
main.o: main.cpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c main.cpp  -o main.o

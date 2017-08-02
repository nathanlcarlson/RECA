ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell uname -s)
endif

CXX := g++
CXXFLAGS := -std=c++11
INCLUDES := -I.
LIBS := -lglfw
OBJECTS := main.o

ifeq ($(detected_OS),Windows)
    CXXFLAGS += -D WIN32
endif
ifeq ($(detected_OS),Darwin)  # Mac OS X
    CXXFLAGS += -D OSX
	LIBS += -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
endif
ifeq ($(detected_OS),Linux)
    CXXFLAGS += -D LINUX
	LIBS += -lGL	
endif

test: $(OBJECTS)
	$(CXX) $<  -o $@ $(LIBS)
.cpp.o:
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $<  -o $@

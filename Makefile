CXX := g++
CXXFLAGS := -std=c++11 -O2 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
INCLUDES := -I/usr/local/include -I.
LIBS := -L/usr/local/lib
OBJS :=  couplings.o utils.o

detected_OS := $(shell uname -s)
ifeq ($(detected_OS),Darwin)  # Mac OS X
    CXXFLAGS += -D OSX
		GLIBS := $(LIBS) -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif
ifeq ($(detected_OS),Linux)
    CXXFLAGS += -D LINUX
	  GLIBS += $(LIBS) -lGL -lglfw -Wl,-rpath,/usr/local/lib
endif

graphics: $(OBJS) graphics.o
	$(CXX) $(INCLUDES) $(OBJS) graphics.o -o graphics $(GLIBS)
graphics.o: graphics.cpp state.hpp algorithm.hpp couplings.hpp utils.hpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c graphics.cpp  -o graphics.o

reca: $(OBJS) reca.o
	$(CXX) $(INCLUDES) $(OBJS) reca.o -o reca $(LIBS)
reca.o: reca.cpp state.hpp algorithm.hpp couplings.hpp utils.hpp
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c reca.cpp  -o reca.o

experiment:
	$(CXX) $(INCLUDES) $(CXXFLAGS) experiment.cpp -o experiment

clean:
	-rm *.o

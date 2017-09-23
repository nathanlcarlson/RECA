CXX := g++
CXXFLAGS := -std=c++11 -O2 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
INCLUDES := -I/usr/local/include -I.
LIBS := -L/usr/local/lib
OBJS :=  obj/couplings.o obj/utils.o
HPP := $(wildcard src/*.hpp)

detected_OS := $(shell uname -s)
ifeq ($(detected_OS),Darwin)  # Mac OS X
    CXXFLAGS += -D OSX
		GLIBS := $(LIBS) -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif
ifeq ($(detected_OS),Linux)
    CXXFLAGS += -D LINUX
	  GLIBS += $(LIBS) -lGL -lglfw -Wl,-rpath,/usr/local/lib
endif

graphics: $(OBJS) obj/graphics.o
	$(CXX) $(INCLUDES) -o $@ $^ $(GLIBS)
reca: $(OBJS) obj/reca.o
	$(CXX) $(INCLUDES) -o $@ $^ $(LIBS)
obj/%.o: src/%.cpp $(HPP)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c -o $@ $<

experiment:
	$(CXX) $(INCLUDES) $(CXXFLAGS) src/experiment.cpp -o experiment

clean:
	-rm -f obj/*.o
	-rm -f graphics
	-rm -f reca
	-rm -f experiment

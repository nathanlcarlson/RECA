CXX := c++
CXXFLAGS := -std=c++14 -O3 #-Wall -Wextra -Wnon-virtual-dtor -pedantic
INCLUDES := -I/usr/local/include -I. -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/libbson-1.0
LIBS := -L/usr/local/lib -lfftw3
OBJDIR := obj
OBJS :=  $(addprefix $(OBJDIR)/,couplings.o utils.o)
HPP := $(wildcard src/*.hpp)

detected_OS := $(shell uname -s)
ifeq ($(detected_OS),Darwin)  # Mac OS X
    CXXFLAGS += -D OSX
		GLIBS := $(LIBS)  -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -Wl,-rpath,/usr/local/lib/
endif
ifeq ($(detected_OS),Linux)
    CXXFLAGS += -D LINUX
	  GLIBS += $(LIBS) -lGL -lglfw -Wl,-rpath=/usr/local/lib/
endif

graphics: $(OBJS) obj/graphics.o
	$(CXX) $(INCLUDES) -o $@ $^ $(GLIBS)
reca: $(OBJS) obj/reca.o
	$(CXX) $(INCLUDES) -o $@ $^ $(LIBS)
obj/%.o: src/%.cpp $(HPP) | $(OBJDIR)
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c -o $@ $< -Wl,-rpath=/usr/local/lib/
$(OBJDIR):
	mkdir -p $(OBJDIR)
experiment:
	$(CXX) $(INCLUDES) $(CXXFLAGS) src/experiment.cpp -o experiment

clean:
	-rm -f obj/*.o
	-rm -f graphics
	-rm -f reca
	-rm -f experiment

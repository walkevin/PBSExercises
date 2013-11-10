

EXE=viewer
SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

#SOURCES = bla.cpp
#OBJECTS = bla.o

CXX        = g++-4.8
LDFLAGS    = -lGL -lGLEW -lGLU -lglut -lrt -L/usr/lib/x86_64-linux-gnu
CFLAGS     = -c -O2 -std=c++11


default: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) -o $(EXE) $(LDFLAGS) $(OBJECTS)

%.o : %.cpp
	$(CXX) $(CFLAGS) -o $@ $<

clean:
	 rm -f *.o $(EXE)


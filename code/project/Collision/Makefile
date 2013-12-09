EXE=collisionTest

EXAMPLE_SRC = main.cpp CollisionHandler.cpp
EXAMPLE_OBJ = main.o CollisionHandler.o

SOURCES = $(EXAMPLE_SRC)
OBJECTS = $(EXAMPLE_OBJ)

CXX        = g++-4.8
LDFLAGS    = 
INC        = -I/usr/include/eigen3
CFLAGS     = -c -std=c++11 -Wno-enum-compare


default: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) -o $(EXE) $(OBJECTS) $(LDFLAGS)

%.o : %.cpp
	$(CXX) $(CFLAGS) $(INC) -o $@ $<

clean:
	 rm -f *.o $(EXE)

run:
	./$(EXE)

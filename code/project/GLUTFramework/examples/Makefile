EXE=viewer

EXAMPLE_SRC = main.cpp ExampleBall.cpp
EXAMPLE_OBJ = main.o ExampleBall.o

#EXAMPLE_SRC = triangles.cpp
#EXAMPLE_OBJ = triangles.o

FP = ../src/
FRAMEWORK_SRC = $(FP)GlutFramework.cpp $(FP)PerformanceTimer.cpp $(FP)Keyboard.cpp $(FP)ShaderLoader.cpp $(FP)RotatingView.cpp $(FP)Ball.cpp
FRAMEWORK_OBJ = $(FP)GlutFramework.o $(FP)PerformanceTimer.o $(FP)Keyboard.o $(FP)ShaderLoader.o $(FP)RotatingView.o $(FP)Ball.o

#SOURCES = $(wildcard *.cpp)
#OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

SOURCES = $(EXAMPLE_SRC) $(FRAMEWORK_SRC)
OBJECTS = $(EXAMPLE_OBJ) $(FRAMEWORK_OBJ)

CXX        = g++-4.8
LDFLAGS    = -lrt -lGL -lGLEW -lGLU -lglut -L/usr/lib/x86_64-linux-gnu
INC        = -I/usr/include/eigen3
CFLAGS     = -c -std=c++11


default: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) -o $(EXE) $(OBJECTS) $(LDFLAGS)

%.o : %.cpp
	$(CXX) $(CFLAGS) $(INC) -o $@ $<

clean:
	 rm -f *.o $(EXE)

run:
	./$(EXE)

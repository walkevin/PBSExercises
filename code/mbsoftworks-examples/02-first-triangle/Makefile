EXE=viewer
#EXAMPLE_SRC = main.cpp Example1.cpp
#EXAMPLE_OBJ = main.o Example1.o

#EXAMPLE_SRC = triangles.cpp
#EXAMPLE_OBJ = triangles.o

EXAMPLE_SRC = OpenGLControl.cpp RendeScene.cpp
EXAMPLE_OBJ = OpenGLControl.o RendeScene.o
#EXAMPLE_OBJ = triangles.o

FP = ../src/
FRAMEWORK_SRC = $(FP)GlutFramework.cpp $(FP)PerformanceTimer.cpp $(FP)Keyboard.cpp $(FP)ShaderLoader.cpp
FRAMEWORK_OBJ = $(FP)GlutFramework.o $(FP)PerformanceTimer.o $(FP)Keyboard.o $(FP)ShaderLoader.o

#SOURCES = $(wildcard *.cpp)
#OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

SOURCES = $(EXAMPLE_SRC) $(FRAMEWORK_SRC)
OBJECTS = $(EXAMPLE_OBJ) $(FRAMEWORK_OBJ)

CXX        = g++
LDFLAGS    = -lGL -lGLEW -lGLU -lglut -lrt -L/usr/lib/x86_64-linux-gnu
INC        = -I/usr/include/eigen3
CFLAGS     = -c -std=c++11


default: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) -o $(EXE) $(LDFLAGS) $(OBJECTS)

%.o : %.cpp
	$(CXX) $(CFLAGS) $(INC) -o $@ $<

clean:
	 rm -f *.o $(EXE)


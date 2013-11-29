EXE=viewer
EXAMPLE_SRC = main.cpp 03-shaders-are-coming.cpp
EXAMPLE_OBJ = main.o 03-shaders-are-coming.o

#EXAMPLE_SRC = main.cpp 02-firstTriangle.cpp
#EXAMPLE_OBJ = main.o 02-firstTriangle.o

#EXAMPLE_SRC = main.cpp 03-shaders-are-coming.cpp
#EXAMPLE_OBJ = main.o 03-shaders-are-coming.o

FP = ../GLUTFramework/src/
FRAMEWORK_SRC = $(FP)GlutFramework.cpp $(FP)PerformanceTimer.cpp $(FP)Keyboard.cpp $(FP)ShaderLoader.cpp
FRAMEWORK_OBJ = $(FP)GlutFramework.o $(FP)PerformanceTimer.o $(FP)Keyboard.o $(FP)ShaderLoader.o

#SOURCES = $(wildcard *.cpp)
#OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

SOURCES = $(EXAMPLE_SRC) $(FRAMEWORK_SRC)
OBJECTS = $(EXAMPLE_OBJ) $(FRAMEWORK_OBJ)

CXX        = g++-4.8
LDFLAGS    = -lGL -lGLEW -lGLU -lglut -lrt -L/usr/lib/x86_64-linux-gnu -L/usr/local/cuda-5.0/samples/common/lib/linux/x86_64/
INC        = -I/usr/include/eigen3
CFLAGS     = -c -std=c++11


default: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) -o $(EXE) $(OBJECTS) $(LDFLAGS)

%.o : %.cpp
	$(CXX) $(CFLAGS) $(INC) -o $@ $<

clean:
	 rm -f *.o $(EXE)


EXE=viewer
EXAMPLE_SRC = main.cpp Paintball.cpp
EXAMPLE_OBJ = main.o Paintball.o

FP = ../GLUTFramework/src/
FRAMEWORK_SRC = $(FP)GlutFramework.cpp $(FP)PerformanceTimer.cpp $(FP)ShaderLoader.cpp $(FP)RotatingView.cpp 
FRAMEWORK_OBJ = $(FP)GlutFramework.o $(FP)PerformanceTimer.o $(FP)ShaderLoader.o $(FP)RotatingView.o

FPS = ../SPH/
SOLVER_SRC = $(FPS)SphSolver.cpp $(FPS)SphBase.cpp $(FPS)SphCell.cpp $(FPS)SphLiquid.cpp
SOLVER_OBJ = $(FPS)SphSolver.o $(FPS)SphBase.o $(FPS)SphCell.o $(FPS)SphLiquid.o

FPC = ../Collision/
COLLISION_SRC = $(FPC)CollisionHandler.cpp
COLLISION_OBJ = $(FPC)CollisionHandler.o

FPM = ../Models/
MODEL_SRC = $(FPM)Ball.cpp $(FPM)Pyramid.cpp $(FPM)Cuboid.cpp $(FPM)GeometricObject.cpp $(FPM)Ellipse.cpp $(FPM)OBJModel.cpp
MODEL_OBJ = $(FPM)Ball.o $(FPM)Pyramid.o $(FPM)Cuboid.o $(FPM)GeometricObject.o $(FPM)Ellipse.o $(FPM)OBJModel.o
#SOURCES = $(wildcard *.cpp)
#OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

SOURCES = $(EXAMPLE_SRC) $(FRAMEWORK_SRC) $(SOLVER_SRC) $(COLLISION_SRC) $(MODEL_SRC)
OBJECTS = $(EXAMPLE_OBJ) $(FRAMEWORK_OBJ) $(SOLVER_OBJ) $(COLLISION_OBJ) $(MODEL_OBJ)


CXX        = g++-4.8
#LDFLAGS    = -g -pg -lGL -lGLEW -lGLU -lglut -lrt -L/usr/lib/x86_64-linux-gnu
LDFLAGS    = -lGL -lGLEW -lGLU -lglut -lrt -L/usr/lib/x86_64-linux-gnu
INC        = -I/usr/include/eigen3
#CFLAGS     = -g -pg -c -std=c++11 -Og -Wno-enum-compare
CFLAGS     = -c -std=c++11 -O3 -Wno-enum-compare

DEFINES = -DNDEBUG



default: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) -o $(EXE) $(OBJECTS) $(LDFLAGS)

%.o : %.cpp
	$(CXX) $(CFLAGS) $(INC) -o $@ $< $(DEFINES)

clean:
	 rm -f $(FP)*.o $(FPS)*.o $(FPC)*.o $(FPM)*.o *.o $(EXE)

run:
	./$(EXE)
	
.PHONY: clean

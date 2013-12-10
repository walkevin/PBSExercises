EXE=viewer
EXAMPLE_SRC = main.cpp Paintball.cpp
EXAMPLE_OBJ = main.o Paintball.o

#EXAMPLE_SRC = triangles.cpp
#EXAMPLE_OBJ = triangles.o

FP = ../GLUTFramework/src/
FRAMEWORK_SRC = $(FP)GlutFramework.cpp $(FP)PerformanceTimer.cpp $(FP)ShaderLoader.cpp $(FP)RotatingView.cpp $(FP)Ball.cpp $(FP)Pyramid.cpp $(FP)Cuboid.cpp $(FP)GeometricObject.cpp
FRAMEWORK_OBJ = $(FP)GlutFramework.o $(FP)PerformanceTimer.o $(FP)ShaderLoader.o $(FP)RotatingView.o $(FP)Ball.o $(FP)Pyramid.o $(FP)Cuboid.o $(FP)GeometricObject.o

FPS = ../SPH/
SOLVER_SRC = $(FPS)SphSolver.cpp $(FPS)SphBase.cpp $(FPS)SphCell.cpp $(FPS)SphLiquid.cpp
SOLVER_OBJ = $(FPS)SphSolver.o $(FPS)SphBase.o $(FPS)SphCell.o $(FPS)SphLiquid.o

FPC = ../Collision/
COLLISION_SRC = $(FPC)CollisionHandler.cpp
COLLISION_OBJ = $(FPC)CollisionHandler.o

#SOURCES = $(wildcard *.cpp)
#OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

SOURCES = $(EXAMPLE_SRC) $(FRAMEWORK_SRC) $(SOLVER_SRC) $(COLLISION_SRC)
OBJECTS = $(EXAMPLE_OBJ) $(FRAMEWORK_OBJ) $(SOLVER_OBJ) $(COLLISION_OBJ)

CXX        = g++-4.8
LDFLAGS    = -g -pg -lGL -lGLEW -lGLU -lglut -lrt -L/usr/lib/x86_64-linux-gnu
INC        = -I/usr/include/eigen3
CFLAGS     = -g -pg -c -std=c++11 -O3 -Wno-enum-compare
DEFINES    = -DNDEBUG


default: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) -o $(EXE) $(OBJECTS) $(LDFLAGS)

%.o : %.cpp
	$(CXX) $(CFLAGS) $(INC) -o $@ $< $(DEFINES)

clean:
	 rm -f $(FP)*.o $(FPS)*.o $(FPC)*.o *.o $(EXE)

run:
	./$(EXE)
	
.PHONY: clean

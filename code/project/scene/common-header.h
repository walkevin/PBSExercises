////////////////////
//
// common-header.h
//
////////////////////
#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H
#include <iostream>
using namespace std;

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "../GLUTFramework/src/ShaderLoader.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))

enum VAO_IDs { Triangles, NumVAOs};
enum Buffer_IDs { ArrayBuffer, NumBuffers};
enum Attrib_IDs { vPosition = 0 };

#endif

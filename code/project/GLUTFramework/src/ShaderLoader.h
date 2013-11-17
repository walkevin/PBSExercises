#ifndef __SHADERLOADER_H__
#define __SHADERLOADER_H__

#include <GL/gl.h>
#include <string>
//----------------------------------------------------------------------------
//
//  LoadShaders() takes an array of ShaderFile structures, each of which
//    contains the type of the shader, and a pointer a C-style character
//    string (i.e., a NULL-terminated array of characters) containing the
//    entire shader source.
//
//  The array of structures is terminated by a final Shader with the
//    "type" field set to GL_NONE.
//
//  LoadShaders() returns the shader program value (as returned by
//    glCreateProgram()) on success, or zero on failure.
//

//typedef struct {
//    GLenum       type;
//    std::string  filename;
//    GLuint       shader;
//} ShaderInfo;

class ShaderLoader
{
public:
	GLuint loadAndUse( const std::string &vertex_file, const std::string &fragment_file );
	void destroy();

	GLuint getProgramId();
private:
	std::string readTextfile(const std::string& filename);
	GLuint vertexShaderId;
	GLuint fragmentShaderId;
	GLuint programId;

};

#endif // __LOAD_SHADERS_H__

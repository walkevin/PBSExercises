#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include "ShaderLoader.h"

//=======================================================================
// Reads a text file
//=======================================================================
// IN:  Name of the file
// OUT: Contents of the file as a string
//=======================================================================
std::string ShaderLoader::readTextfile(const std::string &fileName)
{
	// open file
	std::ifstream ifs(fileName.c_str(), std::ios::binary);
	if(!ifs)
	{
		std::cerr << "File not found" << std::endl;
		return "";
	}

	// Determine length of file
	ifs.seekg(0, std::ios::end);
	size_t fileLen = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	// Allocate string buffer and read string from file
	char *pStr = new char[fileLen + 1];
	ifs.read(pStr, fileLen);
	pStr[fileLen] = 0;

	// Convert to STL string
	std::string s = pStr;
	delete[] pStr;

	return s;
}


//=======================================================================
// Creates two shader objects, fills them with the shader source,
// compiles them, creates a new Program object, attaches both shaders
// to this object, links the program object, returns its identifier.
//=======================================================================
// IN:  path to a GLSL vertex shader file, path to a GLSL fragment shader file
// OUT: GLuint identifier of the (Shader)Program object
//=======================================================================
GLuint ShaderLoader::load(const std::string &vertex_file, const std::string &fragment_file)
{
	GLint status;

	// create both shader objects
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// load source code from file an add to shader objects
	std::string sourceCode = readTextfile(vertex_file);
	if(sourceCode.length() == 0)
	{
		std::cerr << "create Shader Object failed";
		return 0;
	}
	const char *pS = sourceCode.c_str();
	glShaderSource(vertexShaderId, 1, &pS, NULL);

	sourceCode = readTextfile(fragment_file);
	if(sourceCode.length() == 0)
	{
		std::cerr << "create Shader Object failed";
		return 0;
	}
	pS = sourceCode.c_str();
	glShaderSource(fragmentShaderId, 1, &pS, NULL);

	// compile both shaders
	glCompileShader(vertexShaderId);
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &status);
	if(status == 0)
	{
		GLint length;
		glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &length);
		char * errorstr = new char[length];
		glGetShaderInfoLog(vertexShaderId, length, NULL, errorstr);
		std::cerr << vertex_file << " compile error(s):" << std::endl << errorstr;
		delete [] errorstr;
		return 0;
	}

	glCompileShader(fragmentShaderId);
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &status);
	if(status == 0)
	{
		GLint length;
		glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &length);
		char * errorstr = new char[length];
		glGetShaderInfoLog(fragmentShaderId, length, NULL, errorstr);
		std::cerr << fragment_file << " compile error(s):" << std::endl << errorstr;
		delete[] errorstr;
		return 0;
	}

	// create Program object
	programId = glCreateProgram();

	// attach both shaders
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	// Link Program
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &status);
	if(status == 0)
	{
		GLint length;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
		char * errorstr = new char[length];
		glGetProgramInfoLog(programId, length, NULL, errorstr);
		std::cerr << vertex_file << " " << fragment_file << " link error(s):" << std::endl << errorstr;
		delete[] errorstr;
		return 0;
	}


	return programId;
}

void ShaderLoader::use(){
	if(glIsProgram(programId)){
		glUseProgram(programId);
	}
	else{
		std::cerr << "ERROR: Could not use program. Is no program" << std::endl;
	}
}

GLuint ShaderLoader::loadAndUse(const std::string &vertex_file, const std::string &fragment_file){
	GLuint tmp = load(vertex_file, fragment_file);
	use();
	return tmp;
}

void ShaderLoader::destroy(){
	GLenum ErrorCheckValue = glGetError();

	glUseProgram(0);

	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);

	glDeleteShader(fragmentShaderId);
	glDeleteShader(vertexShaderId);

	glDeleteProgram(programId);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not destroy the shaders: %s \n",
			gluErrorString(ErrorCheckValue)
		);

		exit(-1);
	}
}


GLuint ShaderLoader::getProgramId(){
	return programId;
}

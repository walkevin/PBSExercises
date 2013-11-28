#include "ViewerPoints.h"
#include <iostream>
#include <fstream>
	ViewerPoints::ViewerPoints(){
		//Fill array with arbitrary points
		pos.reserve(50);
		for(int i = 0; i < 50; i++){
			std::cout << i << std::endl;
 			pos[i] << i, i*i, std::sqrt(i), 1.;
		}
	}
	ViewerPoints::~ViewerPoints(){}

	void ViewerPoints::load(){
		createShaders();
		createVBO();
	}
	void ViewerPoints::display(float dTime){
		glClearColor(1.0, 1.0, 1.0, 1.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPointSize(5.0f);
		glDrawArrays(GL_POINTS, 0, pos.size());

		glutSwapBuffers();
		glutPostRedisplay();
	}

	void ViewerPoints::close(){
		std::cout << "ViewerPoints::close" << std::endl;
		destroyShaders();
		destroyVBO();
	}
	void ViewerPoints::createVBO(){
		GLenum ErrorCheckValue = glGetError();
		const size_t BufferSize = pos.size() * sizeof(pos[0]);
		const size_t VertexSize = sizeof(pos[0]);

		glGenVertexArrays(1, &VaoId);
		glBindVertexArray(VaoId);

		glGenBuffers(1, &BufferId);
		glBindBuffer(GL_ARRAY_BUFFER, BufferId);
		glBufferData(GL_ARRAY_BUFFER, BufferSize, &pos[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);

		glEnableVertexAttribArray(0);

		ErrorCheckValue = glGetError();
		if (ErrorCheckValue != GL_NO_ERROR)
		{
			fprintf(
				stderr,
				"ERROR: Could not create a VBO: %s \n",
				gluErrorString(ErrorCheckValue)
			);

			exit(-1);
		}
	}
	void ViewerPoints::destroyVBO(){
		GLenum ErrorCheckValue = glGetError();

		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &BufferId);

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &VaoId);

		ErrorCheckValue = glGetError();
		if (ErrorCheckValue != GL_NO_ERROR)
		{
			fprintf(
				stderr,
				"ERROR: Could not destroy the VBO: %s \n",
				gluErrorString(ErrorCheckValue)
			);

			exit(-1);
		}
	}
	void ViewerPoints::createShaders(){
		const GLchar* VertexShader =
		{
			"#version 400\n"\

			"layout(location=0) in vec4 in_Position;\n"\
//			"out vec4 ex_Color;\n"\

			"void main(void)\n"\
			"{\n"\
			"	gl_Position = in_Position;\n"\
			"}\n"
		};

//		const GLchar* FragmentShader =
//		{
//			"#version 400\n"\
//
//			"in vec4 ex_Color;\n"\
//			"out vec4 out_Color;\n"\
//
//			"void main(void)\n"\
//			"{\n"\
//			"	out_Color = ex_Color;\n"\
//			"}\n"
//		};


		GLenum ErrorCheckValue = glGetError();

		VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
		glCompileShader(VertexShaderId);

//		FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
//		glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
//		glCompileShader(FragmentShaderId);

		ProgramId = glCreateProgram();
			glAttachShader(ProgramId, VertexShaderId);
//			glAttachShader(ProgramId, FragmentShaderId);
		glLinkProgram(ProgramId);
		glUseProgram(ProgramId);

		ErrorCheckValue = glGetError();
		if (ErrorCheckValue != GL_NO_ERROR)
		{
			fprintf(
				stderr,
				"ERROR: Could not create the shaders: %s \n",
				gluErrorString(ErrorCheckValue)
			);

			exit(-1);
		}
	}
	void ViewerPoints::destroyShaders(){
		GLenum ErrorCheckValue = glGetError();

		glUseProgram(0);

		glDetachShader(ProgramId, VertexShaderId);
//		glDetachShader(ProgramId, FragmentShaderId);

//		glDeleteShader(FragmentShaderId);
		glDeleteShader(VertexShaderId);

		glDeleteProgram(ProgramId);

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

	//=======================================================================
	// Reads a text file
	//=======================================================================
	// IN:  Name of the file
	// OUT: Contents of the file as a string
	//=======================================================================
	std::string readTextFile(const std::string &fileName)
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

	GLuint createShaderObject(const std::string &vertex_file, const std::string &fragment_file)
	{
		GLint status;

		// create both shader objects
		GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

		// load source code from file an add to shader objects
		std::string sourceCode = readTextFile(vertex_file);
		if(sourceCode.length() == 0)
		{
			std::cerr << "create Shader Object failed";
			return 0;
		}
		const char *pS = sourceCode.c_str();
		glShaderSource(vshader, 1, &pS, NULL);

		sourceCode = readTextFile(fragment_file);
		if(sourceCode.length() == 0)
		{
			std::cerr << "create Shader Object failed";
			return 0;
		}
		pS = sourceCode.c_str();
		glShaderSource(fshader, 1, &pS, NULL);

		// compile both shaders
		glCompileShader(vshader);
		glGetShaderiv(vshader, GL_COMPILE_STATUS, &status);
		if(status == 0)
		{
			GLint length;
			glGetShaderiv(vshader, GL_INFO_LOG_LENGTH, &length);
			char * errorstr = new char[length];
			glGetShaderInfoLog(vshader, length, NULL, errorstr);
			std::cerr << vertex_file << " compile error(s):" << std::endl << errorstr;
			delete [] errorstr;
			return 0;
		}

		glCompileShader(fshader);
		glGetShaderiv(fshader, GL_COMPILE_STATUS, &status);
		if(status == 0)
		{
			GLint length;
			glGetShaderiv(fshader, GL_INFO_LOG_LENGTH, &length);
			char * errorstr = new char[length];
			glGetShaderInfoLog(fshader, length, NULL, errorstr);
			std::cerr << fragment_file << " compile error(s):" << std::endl << errorstr;
			delete[] errorstr;
			return 0;
		}

		// create Program object
		GLuint program = glCreateProgram();

		// attach both shaders
		glAttachShader(program, vshader);
		glAttachShader(program, fshader);

		// Link Program
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if(status == 0)
		{
			GLint length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			char * errorstr = new char[length];
			glGetProgramInfoLog(program, length, NULL, errorstr);
			std::cerr << vertex_file << " " << fragment_file << " link error(s):" << std::endl << errorstr;
			delete[] errorstr;
			return 0;
		}

		// Flag the shader objects for deletion so they get deleted
		// automatically when the program object is deleted.
		glDeleteShader(vshader);
		glDeleteShader(fshader);

		return program;
	}

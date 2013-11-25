#include "ExampleAnim.h"
#include <iostream>
#include "vmath.h"
	ExampleAnim::ExampleAnim() : viewDistance(3.), viewAzimuthAngle(M_PI *1.), viewPolarAngle(0.), angleChangeSpeed(M_PI * 0.1) {
		pos.reserve(n_points);
		for(int i = 0; i < n_points; i++){
 			pos[i] << (float)i/n_points, (float)(i)/n_points, (float)(i)/n_points, 1.0f;
		}
	}
	ExampleAnim::~ExampleAnim(){}

	void ExampleAnim::updatePositions(){
		for(int i = 0; i < n_points; i++){
			//pos[i][0] -= 0.01f;
		}
	}

	void ExampleAnim::load(){
		sh.load("shaders/exampleAnim.glslv","shaders/exampleAnim.glslf");
		sh.use();

		// Set model matrix
		GLfloat model_mat[16] = {
			 1.000000f,  0.000000f,  0.000000f, 0.000000f,
			 0.000000f,  1.000000f,  0.000000f, 0.000000f,
			 0.000000f,  0.000000f,  1.000000f, 0.000000f,
			 0.000000f,  0.000000f,  0.000000f, 1.0000000f
		};

		GLint location;
		location = glGetUniformLocation(sh.getProgramId(), "Model_mat");
		glUniformMatrix4fv(location, 1, false, model_mat);


		// Set projection*view Matrix
		vmath::mat4 projview_mat = vmath::perspective(60.0f, float(WINDOW_WIDTH/WINDOW_HEIGHT), 1.0f, 500.0f) *
				                   vmath::lookat(vmath::vec3(std::cos(viewAzimuthAngle) * viewDistance, 4.0f, std::sin(viewAzimuthAngle) * viewDistance), vmath::vec3(0.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 1.0f, 0.0f));

		location = glGetUniformLocation(sh.getProgramId(), "ProjectView_mat");
		glUniformMatrix4fv(location, 1, false, (float*)(projview_mat));

		//Create Vertex Buffer Objects
		createVBO();
	}
	void ExampleAnim::display(float dTime){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		updatePositions();
		//VertexSize and BufferSize for std::vector<Eigen::Array<float,4> >
		const size_t VertexSize = sizeof(pos[0]);//ok for std::array (size known at compile time), otherwise 4 * sizeof(pos[0][0])
		const size_t BufferSize = n_points * VertexSize;
		void* dataPointer = pos.data();


		glBindVertexArray(VaoId);

		glBindBuffer(GL_ARRAY_BUFFER, BufferId);
		glBufferData(GL_ARRAY_BUFFER, BufferSize, dataPointer, GL_STATIC_DRAW);

		GLint posLoc = glGetAttribLocation(sh.getProgramId(), "in_Position");
		glVertexAttribPointer(posLoc, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
		glEnableVertexAttribArray(posLoc);

		GLenum ErrorCheckValue = glGetError();
		if (ErrorCheckValue != GL_NO_ERROR)
		{
			fprintf(
				stderr,
				"ERROR: Could not create a VBO: %s \n",
				gluErrorString(ErrorCheckValue)
			);

			//exit(-1);
		}


		glPointSize(5.0f);
		glDrawArrays(GL_POINTS, 0, n_points);
		glutSwapBuffers();
		glutPostRedisplay();
	}

	void ExampleAnim::close(){
		std::cout << "ExampleAnim::close" << std::endl;
		sh.destroy();
		destroyVBO();
	}
	void ExampleAnim::createVBO(){
		glGenVertexArrays(1, &VaoId);
		glGenBuffers(1, &BufferId);


		float coordsC[][4] = {
			{ -0.2f, 0.8f, 0.0f, 1.0f },  // 1
			{ 0.075, 0.85f, 0.05f, 1.0f },  // 2
			{ 0.0f, 0.8f, 0.0f, 1.0f },  //3
			{ 0.0f, 1.0f, 0.0f, 1.0f }
		};
//		const size_t VertexSize = sizeof(coordsC[0]);
//		const size_t BufferSize = 4 * VertexSize;
//		void* dataPointer = coordsC;

		std::vector<float> coordsStd;
		for(int i = 0; i < n_points; i++){
			coordsStd.push_back(float(i)/n_points);
			coordsStd.push_back(0.0f);
			coordsStd.push_back(0.0f);
			coordsStd.push_back(1.0f);
		}
//		const size_t VertexSize = 4 * sizeof(coordsStd[0]);
//		const size_t BufferSize = n_points*VertexSize;
//		void* dataPointer = coordsStd.data();
	}
	void ExampleAnim::destroyVBO(){
		GLenum ErrorCheckValue = glGetError();

		//TODO: Make locations of attributes, uniforms etc. class variables, in order to properly destroy them here.
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &BufferId);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &IndexBufferId);

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

	void ExampleAnim::specialKeyboardDown(int key, int x, int y )
	{
		// Subclass and override this method
		//printf( "KeyboardDown: %c = %d\n", key, (int)key );

		if(key == 100 || key == 102){
			if(key == 100)
				viewAzimuthAngle -= angleChangeSpeed;
			else
				viewAzimuthAngle += angleChangeSpeed;

			sh.use();
			vmath::mat4 projview_mat = vmath::perspective(60.0f, float(WINDOW_WIDTH/WINDOW_HEIGHT), 1.0f, 500.0f) *
	                   vmath::lookat(vmath::vec3(std::cos(viewAzimuthAngle) * viewDistance, 4.0f, std::sin(viewAzimuthAngle) * viewDistance), vmath::vec3(0.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 1.0f, 0.0f));

			GLint location = glGetUniformLocation(sh.getProgramId(), "ProjectView_mat");
			glUniformMatrix4fv(location, 1, false, (float*)(projview_mat));
			glutPostRedisplay();
		}

		if(key == 27)
			glutLeaveMainLoop();


		keyStates.keyDown( (int)key );
	}

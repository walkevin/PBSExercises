/*
 * The MIT License
 *
 * Copyright (c) 2010 Paul Solt, PaulSolt@gmail.com 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "GlutFramework.h"

namespace glutFramework {
	
	// Set constants
	const double GlutFramework::FRAME_TIME = 1.0 / GlutFramework::FPS * 1000.0; // Milliseconds

	
	GlutFramework *GlutFramework::instance = NULL;
	
	GlutFramework::GlutFramework() { 
		elapsedTimeInSeconds = 0;
		frameTimeElapsed = 0;
		title = "Paintball Shot 2013";
	}
	
	GlutFramework::~GlutFramework() {
	}
	
	void GlutFramework::startFramework(int argc, char *argv[]) {
		setInstance();	// Sets the instance to self, used in the callback wrapper functions
		
		// Initialize GLUT
		glutInit(&argc, argv);
//		glutInitContextVersion(3, 0);
//		glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
//		glutInitContextProfile(GLUT_CORE_PROFILE);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutInitWindowPosition(WINDOW_X_POSITION, WINDOW_Y_POSITION);
		glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		glutCreateWindow(title.c_str()); 
		
		// Function callbacks with wrapper functions
		glutReshapeFunc(reshapeWrapper);
		glutMouseFunc(mouseButtonPressWrapper);
		glutMotionFunc(mouseMoveWrapper);
		glutDisplayFunc(displayWrapper);
		glutKeyboardFunc(keyboardDownWrapper);
		glutKeyboardUpFunc(keyboardUpWrapper);
		glutSpecialFunc(specialKeyboardDownWrapper);
		glutSpecialUpFunc(specialKeyboardUpWrapper);
		glutCloseFunc(closeWrapper);
		

		//Initialize GLEW, after GLUT
		GLenum GlewInitResult;
		glewExperimental = GL_TRUE;
		GlewInitResult = glewInit();

		if (GLEW_OK != GlewInitResult) {
			std::cerr << "ERROR: " << glewGetErrorString(GlewInitResult) << std::endl;
			exit(EXIT_FAILURE);
		}

		init();						// Initialize
		glutIdleFunc(runWrapper); 	// The program run loop
		glutMainLoop();				// Start the main GLUT thread
	}
	
	void GlutFramework::load() {
		// Subclass and override this method
	}
	
	void GlutFramework::display(float dTime) {
		// Subclass and override this method
		
		static int frame = 0;
		std::cout << "GlutFramework Display: Frame: " << frame << ", dt(sec): " << dTime << std::endl;
		++frame;
		
		// DEMO: Create a teapot
		glutSolidTeapot(2.5); 
	}
	
	void GlutFramework::reshape(int width, int height) {
		glViewport(0,0,(GLsizei)width,(GLsizei)height);
	}
	
	void GlutFramework::mouseButtonPress(int button, int state, int x, int y) {
		std::cout << "MouseButtonPress: x: " << x << "y: " << y << std::endl;
		
	}
	
	void GlutFramework::mouseMove(int x, int y) {
		std::cout << "MouseMove: x: " << x << "y: " << y << std::endl;
	}
	
	void GlutFramework::keyboardDown( unsigned char key, int x, int y ) 
	{
		// Subclass and override this method
		std::cout << "KeyboardDown: " << key << " = " << (int)key << std::endl;
		if (key==27) { //27 =- ESC key
			glutLeaveMainLoop();
//			exit (0);
		}
		
		keyStates.keyDown( (int)key );
	}
	
	void GlutFramework::keyboardUp( unsigned char key, int x, int y ) 
	{
		// Subclass and override this method
		std::cout << "KeyboardUp" << key << std::endl;
		keyStates.keyUp( (int)key );
	}
	
	void GlutFramework::specialKeyboardDown( int key, int x, int y ) 
	{
		// Subclass and override this method
		std::cout << "SpecialKeyboardDown: " << key << std::endl;
	}
	
	void GlutFramework::specialKeyboardUp( int key, int x, int y ) 
	{
		// Subclass and override this method	
		std::cout << "SpecialKeyboardUp: " << key << std::endl;
	}

	void GlutFramework::close()
	{
		//Subclass and override this method
		std::cout << "close" << std::endl;
	}
	
	void GlutFramework::setTitle(std::string theTitle) {
		title = theTitle;
	}
	
	// **************************
	// ** GLUT Setup functions **
	// **************************
	void GlutFramework::init() {
		std::cout << "init" << std::endl;
		glClearColor(1.0, 1.0, 1.0, 1.0);
		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		
		load();
	}
	
	void GlutFramework::setInstance() {
		std::cout << "GlutFramework::setInstance()" << std::endl;
		instance = this;
	}
	
	void GlutFramework::run() {
		if(frameRateTimer.isStopped()) {	// The initial frame has the timer stopped, start it once
			frameRateTimer.start();
		}	
		
		frameRateTimer.stop();			// stop the timer and calculate time since last frame
		double milliseconds = frameRateTimer.getElapsedMilliseconds();
		frameTimeElapsed += milliseconds;
		
		if( frameTimeElapsed >= FRAME_TIME ) {	// If the time exceeds a certain "frame rate" then show the next frame
			glutPostRedisplay();
			frameTimeElapsed -= FRAME_TIME;		// remove a "frame" and start counting up again
		}
		frameRateTimer.start();			// start the timer
	}
	
	void GlutFramework::displayFramework() {
		if(displayTimer.isStopped()) {			// Start the timer on the initial frame
			displayTimer.start();
		}
		
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Clear once
		
		displayTimer.stop();		// Stop the timer and get the elapsed time in seconds
		elapsedTimeInSeconds = displayTimer.getElapsedSeconds(); // seconds
		
		display(elapsedTimeInSeconds);
		
		glutSwapBuffers();
		displayTimer.start();		// reset the timer to calculate the time for the next frame
	}
	
	// ******************************************************************
	// ** Static functions which are passed to Glut function callbacks **
	// ******************************************************************
	
	void GlutFramework::displayWrapper() {
		instance->displayFramework(); 
	}
	
	void GlutFramework::reshapeWrapper(int width, int height) {
		instance->reshape(width, height);
	}
	
	void GlutFramework::runWrapper() {
		instance->run();
	}
	
	void GlutFramework::mouseButtonPressWrapper(int button, int state, int x, int y) {
		instance->mouseButtonPress(button, state, x, y);
	}
	
	void GlutFramework::mouseMoveWrapper(int x, int y) {
		instance->mouseMove(x, y);
	}
										 
	void GlutFramework::keyboardDownWrapper(unsigned char key, int x, int y) {
		instance->keyboardDown(key,x,y);
	}
	
	void GlutFramework::keyboardUpWrapper(unsigned char key, int x, int y) {
		instance->keyboardUp(key,x,y);
	}
	
	void GlutFramework::specialKeyboardDownWrapper(int key, int x, int y) {
		instance->specialKeyboardDown(key,x,y);
	}
	
	void GlutFramework::specialKeyboardUpWrapper(int key, int x, int y) {
		instance->specialKeyboardUp(key,x,y);
	}
	
	void GlutFramework::closeWrapper(){
		instance->close();
	}

	//Compatibility, to be deleted
	void GlutFramework::setLookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ){
	 }
} // namespace

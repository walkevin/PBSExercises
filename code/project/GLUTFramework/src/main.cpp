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

//#include "GlutFramework.h"
#include "ConcreteViewer.h"
using namespace glutFramework;

/**
 * A sample program start that uses the base class GlutFramework to create a
 * graphics window that displays a teapot moving side to side.
 * Create a subclass of the GlutFramework and override the virtual methods.
 * 
 * @author Paul Solt 8-22-10
 */
int main(int argc, char *argv[]) {
	
	GlutFramework* framework = new ConcreteViewer();
	framework->setLookAt(0.0, 2.0, 10.0, 0.0, 2.0, 0.0, 0.0, 1.0, 0.0);
	framework->startFramework(argc, argv);
	// **Note** No code below startFramework() will get executed 
	
	return 0;
}

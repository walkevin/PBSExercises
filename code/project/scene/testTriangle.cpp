//EXAMPLE_SRC = main.cpp LustigiSzene.cpp Ball.cpp
//EXAMPLE_OBJ = main.o LustigiSzene.o Ball.o

#include "testTriangle.h"
#include <iostream>

//OpenGL Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../GLUTFramework/src/RotatingView.h"
#include "../GLUTFramework/src/Ball.h"

using namespace std;

GLuint car;
float carrot;
std::vector<float> objVertices;
std::vector<float> objFaces;

	LustigiSzene::LustigiSzene(){}
	LustigiSzene::~LustigiSzene(){}

void LustigiSzene::sf_loadObj(char *fname) {
	FILE *fp;
	int read;
	int read2;
  GLfloat x,y,z;
  GLfloat xx,xy,yx,yy,zx,zy;
  char ch, cha, chb, chc, chd;
	car=glGenLists(1);
	fp=fopen(fname,"r");
	if (!fp)
	{
		printf("can't open file %s\n", fname);
		exit(1);
	}
	glPointSize(2.0);
	glNewList(car, GL_COMPILE);
	{
		glPushMatrix();
		glBegin(GL_POINTS);
		while(!(feof(fp)))
		{
			read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
			if(read==4&&ch=='v')
			{
				objVertices.push_back(x);
				objVertices.push_back(y);
				objVertices.push_back(z);
				glVertex3f(x,y,z);
			}
			read2=fscanf(fp,"%c %f %[/] %f %f %[/] %f %f %[/] %f",&cha,&xx,&chb,&xy,&yx,&chc,&yy,&zx,&chd,&zy);
//			if(read2==10&&ch=='f')
			std::cout << cha;
			if(read2==10&&cha=='f')
			{
				objFaces.push_back(xx);
				objFaces.push_back(yx);
				objFaces.push_back(zx);
			}
		}
		glEnd();
	}
//	for ( int i = 0; i < objVertices.size(); i++ )
//	{
//		std::cout << objVertices[i];
//	}
//	for ( int i = 0; i < objFaces.size(); i++ )
//	{
//		std::cout << objFaces[i] << std::endl;
//	}
//	std::cout << std::endl;
	glPopMatrix();
	glEndList();
	fclose(fp);
}
void LustigiSzene::wiki_load_obj(const char* filename, std::vector<glm::vec4> &vertices, std::vector<glm::vec3> &normals, std::vector<GLushort> &elements) {
	glm::vec4 * mesh; 
  ifstream in(filename, ios::in);
  if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }
 
  string line;
  while (getline(in, line)) {
    if (line.substr(0,2) == "v ") {
      istringstream s(line.substr(2));
      glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
      vertices.push_back(v);
    }  else if (line.substr(0,2) == "f ") {
      istringstream s(line.substr(2));
      GLushort a,b,c;
      s >> a; s >> b; s >> c;
      a--; b--; c--;
      elements.push_back(a); elements.push_back(b); elements.push_back(c);
    }
    else if (line[0] == '#') { /* ignoring this line */ }
    else { /* ignoring this line */ }
  }
 
  normals.resize(mesh->vertices.size(), glm::vec3(0.0, 0.0, 0.0));
  for (int i = 0; i < elements.size(); i+=3) {
    GLushort ia = elements[i];
    GLushort ib = elements[i+1];
    GLushort ic = elements[i+2];
    glm::vec3 normal = glm::normalize(glm::cross(
      glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
      glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
    normals[ia] = normals[ib] = normals[ic] = normal;
  }
}

void LustigiSzene::drawCar() {
 glPushMatrix();
 glTranslatef(0,-40.00,-105);
 glColor3f(1.0,0.23,0.27);
 glScalef(0.1,0.1,0.1);
 glRotatef(carrot,0,1,0);
 glCallList(car);
 glPopMatrix();
 carrot=carrot+0.6;
 if(carrot>360)carrot=carrot-360;
}

void LustigiSzene::load(){
	sh.load("shaders/lustigiSzene.vert","shaders/lustigiSzene.frag");
	sh.use();

	//Create RotatingView Object AFTER shader have been loaded
	rv = new RotatingView(sh, "ProjectView_mat");
	//Send default projection view matrix to GPU
	rv->updateView();

	createVBO();
}
void LustigiSzene::display(float dTime){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_BYTE, NULL);
//////////////////////////////////////
// Draw Triangle and Quad 
//////////////////////////////////////   
		glBindVertexArray(VaoId[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
//		glBindVertexArray(VaoId[1]);
//		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//////////////////////////////////////   
//		glutSolidTeapot(1);
//////////////////////////////////////   
//
//////////////////////////////////////   
// wikibooks opengl-obj-reader
//////////////////////////////////////   
vector<glm::vec4> suzanne_vertices;
vector<glm::vec3> suzanne_normals;
vector<GLushort> suzanne_elements;
//////////////////////////////////////   



		glClearColor (0.0,1.0,0.0,1.0);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		sf_loadObj("data/woman.obj");//replace porsche.obj with radar.obj or any other .obj to display it
		drawCar();

		glutSwapBuffers();
		glutPostRedisplay();
	}

	void LustigiSzene::close(){
		std::cout << "LustigiSzene::close" << std::endl;
		sh.destroy();
		destroyVBO();
	}
	void LustigiSzene::createVBO(){
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		float fTriangle[9]; // Data to render triangle (3 vertices, each has 3 floats)
		float fQuad[12]; // Data to render quad using triangle strips (4 vertices, each has 3 floats)
		float fTriangleColor[9];
		float fQuadColor[12];


		// Setup triangle vertices
		fTriangle[0] = -0.4f; fTriangle[1] = 0.1f; fTriangle[2] = 0.0f;
		fTriangle[3] = 0.4f; fTriangle[4] = 0.1f; fTriangle[5] = 0.0f;
		fTriangle[6] = 0.0f; fTriangle[7] = 0.7f; fTriangle[8] = 0.0f;

		// Setup triangle color

		fTriangleColor[0] = 1.0f; fTriangleColor[1] = 0.0f; fTriangleColor[2] = 0.0f;
		fTriangleColor[3] = 0.0f; fTriangleColor[4] = 1.0f; fTriangleColor[5] = 0.0f;
		fTriangleColor[6] = 0.0f; fTriangleColor[7] = 0.0f; fTriangleColor[8] = 1.0f;

		// Setup quad vertices

		fQuad[0] = -0.2f; fQuad[1] = -0.1f; fQuad[2] = 0.0f;
		fQuad[3] = -0.2f; fQuad[4] = -0.6f; fQuad[5] = 0.0f;
		fQuad[6] = 0.2f; fQuad[7] = -0.1f; fQuad[8] = 0.0f;
		fQuad[9] = 0.2f; fQuad[10] = -0.6f; fQuad[11] = 0.0f;

		// Setup quad color

		fQuadColor[0] = 1.0f; fQuadColor[1] = 0.0f; fQuadColor[2] = 0.0f;
		fQuadColor[3] = 0.0f; fQuadColor[4] = 1.0f; fQuadColor[8] = 0.0f;
		fQuadColor[6] = 0.0f; fQuadColor[7] = 0.0f; fQuadColor[5] = 1.0f;
		fQuadColor[9] = 1.0f; fQuadColor[10] = 1.0f; fQuadColor[11] = 0.0f;

		glGenVertexArrays(nVao, VaoId); // Generate two VAOs, one for triangle and one for quad
		glGenBuffers(nBuffer, BufferId); // And four VBOs

		// Setup whole triangle
		glBindVertexArray(VaoId[0]);

		glBindBuffer(GL_ARRAY_BUFFER, BufferId[0]);
		glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangle, GL_STATIC_DRAW);
		locs["inPosition"] = glGetAttribLocation(sh.getProgramId(),"inPosition");
		glVertexAttribPointer(locs["inPosition"], 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(locs["inPosition"]);

		glBindBuffer(GL_ARRAY_BUFFER, BufferId[1]);
		glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangleColor, GL_STATIC_DRAW);
		locs["inColor"] = glGetAttribLocation(sh.getProgramId(),"inColor");
		glVertexAttribPointer(locs["inColor"], 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(locs["inColor"]);


		// Setup whole quad
		glBindVertexArray(VaoId[1]);

		glBindBuffer(GL_ARRAY_BUFFER, BufferId[2]);
		glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuad, GL_STATIC_DRAW);
		glVertexAttribPointer(locs["inPosition"], 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(locs["inPosition"]);

		glBindBuffer(GL_ARRAY_BUFFER, BufferId[3]);
		glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuadColor, GL_STATIC_DRAW);
		glVertexAttribPointer(locs["inColor"], 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(locs["inColor"]);

		//warning, possible double declaration
		GLenum	ErrorCheckValue = glGetError();
			if (ErrorCheckValue != GL_NO_ERROR){
				std::cerr << "ERROR: Could not create a VBO: " << gluErrorString(ErrorCheckValue) << "\n";
				exit(-1);
			}

	}
	void LustigiSzene::destroyVBO(){
		GLenum ErrorCheckValue = glGetError();

		std::map<std::string, GLint>::iterator it;
		for(it = locs.begin(); it != locs.end(); ++it){
			glDisableVertexAttribArray((*it).second);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(nBuffer, &BufferId[0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(nIndexBuffer, &IndexBufferId[0]);

		glBindVertexArray(0);
		glDeleteVertexArrays(nVao, &VaoId[0]);

		ErrorCheckValue = glGetError();
		if (ErrorCheckValue != GL_NO_ERROR){
			std::cerr << "ERROR: Could not destroy the VBO: " << gluErrorString(ErrorCheckValue) << "\n";
			exit(-1);
		}
	}

	void LustigiSzene::specialKeyboardDown(int key, int x, int y ){
		rv->keyboardEvent(key);
	}

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GLU.h>
#include <GL/GL.h>
#include <time.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include "tinyxml2.h"
#include "Draw.h"
#include <sstream>
#include <algorithm>
#include <valarray>
#include <iostream>

#pragma comment(lib,"glew32.lib")

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult);}
#endif

using namespace std;

GLuint* buffers; vector<int> sizes;
float angle = 0, angle1 = 0;
float alfa = 0, beta = 0, raio = 5, step = 0.02;
float posx = 0, posy = 0, posz = 0;
GLfloat size = 1;
std::valarray<GLfloat> vertices;

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void prepareBuffers(vector<const char*> nomes){
	int i;
	vector<GLfloat> aux;
	glEnableClientState(GL_VERTEX_ARRAY);
	buffers = new GLuint[nomes.size()];
	glGenBuffers(nomes.size(), buffers);
	for (i = 0; i < nomes.size(); i++){
		aux = readVertices(nomes[i]);
		sizes.push_back(aux.size());
		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glBufferData(GL_ARRAY_BUFFER, aux.size() * sizeof(GLfloat), &aux[0], GL_STATIC_DRAW);
	}
}

void drawModels(){
	for (int i = 0; i < sizes.size(); i++){
		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, sizes[i]);
	}
}

void renderScene(void) {
	// clear buffers
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glColor3f(1, 0.2, 0.3);
	// set the camera
	glLoadIdentity();

	gluLookAt(raio*cos(beta)*sin(alfa), raio*sin(beta), raio*cos(beta)*cos(alfa),
		0, 0, 0,
		0.0f, 1.0f, 0.0f);

	/* SISTEMA SOLAR (sort of)
	glPushMatrix();
	glScalef(0.75, 0.75, 0.75);
	drawVertices(vertices);
	glPopMatrix();


	glPushMatrix();
	glRotatef(angle+=0.5, 0.0, 0.0, 1.0); // 2. Rotate the object.
	glTranslatef(3, 0, 0); // 1. Translate to the origin.
	glRotatef(angle1 += 1, 0, 0, 1);
	glScalef(0.25, 0.25, 0.25);
	drawVertices(vertices);
	glPopMatrix();

	if (angle == 360){
	angle = 0;
	}
	if (angle1 == 360){
	angle1 = 0;
	}*/

	drawModels();

	// fim do frame
	glutSwapBuffers();
}

// escrever função de processamento do teclado
void keyboardSpecial(int key, int x, int y){
	switch (key){
	case GLUT_KEY_UP:
		if (beta < (M_PI / 2 - step))
			beta += step;
		break;
	case GLUT_KEY_DOWN:
		if (beta > -(M_PI / 2 - step))
			beta -= step;
		break;
	case GLUT_KEY_LEFT:
		alfa -= step;
		break;
	case GLUT_KEY_RIGHT:
		alfa += step;
		break;
	}
}

void keyboard(unsigned char key, int x, int y){
	float step = 0.05;
	switch (key){
	case '+':
		raio -= 0.05;
		break;
	case '-':
		raio += 0.05;
		break;
	}
}

void readScene(char *filename){
	using namespace tinyxml2;
	//Carregar o ficheiro xml
	XMLDocument xmlDoc;
	XMLError eResult = xmlDoc.LoadFile(filename);

	XMLCheckResult(eResult);

	printf("Loaded %s\n", filename);

	XMLNode * pRoot = xmlDoc.FirstChild();
	if (pRoot == NULL)
		throw 21;

	XMLElement * pListElement = pRoot->FirstChildElement("modelo");
	vector<const char*> nomes;
	while (pListElement != NULL) {
		const char * nome;
		nome = pListElement->Attribute("ficheiro");
		if (nome != NULL) {
			nomes.push_back(nome);
		}
		pListElement = pListElement->NextSiblingElement("modelo");
	}
	prepareBuffers(nomes);
}

int main() {
	srand(time(NULL));

	// init de cenas
	glutInit(&__argc, __argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Projeto-CG");

	// registo de funcs
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutSpecialFunc(keyboardSpecial);
	glutKeyboardFunc(keyboard);
	glewInit();
	// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	try{ readScene("cena.xml"); }
	catch (int e){
		if (e == 21){
			puts("Erro na leitura da cena, formato do ficheiro invalido");
		}
		else{
			puts("Erro na leitura dos triangulos, formato do ficheiro XML invalido!");
		}
		exit(-1);
	}
	// entrar no loop do glut
	glutMainLoop();

	return 1;
}

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
#include <GL/GLU.h>
#include <GL/GL.h>
#include <time.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include "tinyxml2.h"
#include "Modelador.h"
#include "Draw.h"
#include <sstream>
#include <algorithm>
#include <valarray>

using namespace std;

float GLOBAL_height = 1, GLOBAL_angle = 0, GLOBAL_barrelroll = 0;
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

void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(0, 3, 5, 0, 0, 0, 0, 1, 0);

	glTranslatef(posx, posy, posz);
	glRotatef(GLOBAL_angle, 0, 1, 0);
	glRotatef(GLOBAL_barrelroll, 1, 0, 0);
	glShadeModel(GL_SMOOTH);
	glColor3f(1, 0.2, 0.3);

	//glutSolidSphere(1,20,20);
	drawVertices(vertices*size);

	// fim do frame
	glutSwapBuffers();
}

// escrever fun��o de processamento do teclado
void keyboardSpecial(int key, int x, int y){
	float step = 0.05;
	switch (key){
	case GLUT_KEY_UP:
		posz -= step;
		break;
	case GLUT_KEY_DOWN:
		posz += step;
		break;
	case GLUT_KEY_LEFT:
		posx -= step;
		break;
	case GLUT_KEY_RIGHT:
		posx += step;
		break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
	float step = 0.05;
	switch (key){
	case '+':
		size += 0.05;
		break;
	case '-':
		size -= 0.05;
		break;
	case 'd':
		GLOBAL_angle += 1;
		break;
	case 'a':
		GLOBAL_angle -= 1;
		break;
	case 'w':
		GLOBAL_barrelroll += 1;
		break;
	case 's':
		GLOBAL_barrelroll -= 1;
		break;
	}
	glutPostRedisplay();
}

// awkward main func do windows, just copy paste
int main() {
	srand(time(NULL));

	try{ vertices = readVertices("esfera.3d"); }
	catch (int e){ 
		puts("Erro na leitura dos triangulos, formato do ficheiro XML invalido!"); 
		exit(-1); 
	}

	// init de cenas
	glutInit(&__argc, __argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Projeto-CG");

	// registo de funcs
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutSpecialFunc(keyboardSpecial);
	glutKeyboardFunc(keyboard);

	// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	// entrar no loop do glut
	glutMainLoop();

	return 1;
}

#include <GL\glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <IL\il.h>
#include <GL/glut.h>
#include <time.h>
#include <vector>
#include <map>
#define _USE_MATH_DEFINES
#include <math.h>
#include "tinyxml2.h"
#include "Utils.h"
#include <windows.system.h>

using namespace std;

#pragma comment(lib, "glew32.lib")

//modelos e indices do vbo
static map<string, int> models;

//nome da cena que está a ser utilizada
static char *sceneName;

//controlo da camara
static float alfa = 0, beta = 0, raio = 10, step = 0.02;


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
	glShadeModel(GL_SMOOTH);
	glColor3f(1, 0.2, 0.3);
	// set the camera
	glLoadIdentity();

	gluLookAt(raio*cos(beta)*sin(alfa), raio*sin(beta), raio*cos(beta)*cos(alfa),
		0, 0, 0,
		0.0f, 1.0f, 0.0f);

	//TESTE
	GLfloat amb[3] = { 0.2, 0.2, 0.2 };
	GLfloat diff[3] = { 1.0, 1.0, 1.0 };
	// light colors
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	try{ drawScene(sceneName); }
	catch (int e){
		exceptionHandler(e);
		exit(-1);
	}
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
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
	float step = 0.05;
	switch (key){
	case '+':
		raio -= 0.5;
		break;
	case '-':
		raio += 0.5;
		break;
	}
	glutPostRedisplay();
}

void menu(int op){
	switch (op)
	{
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 2:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 3:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	default:
		break;
	}
	glutPostRedisplay();
}

void createMenu(){
	glutCreateMenu(menu);
	glutAddMenuEntry("Linha", 1);
	glutAddMenuEntry("Preencher", 2);
	glutAddMenuEntry("Ponto", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {
	srand(time(NULL));

	if (argc != 2){
		puts("Erro nos argumentos! Especifique apenas o nome da cena a desenhar.\nCertifique-se tambem que o ficheiro esta na diretoria do executavel!");
		return -1;
	}

	sceneName = argv[1];

	//inits opengl
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Projeto-CG");
	glewInit();
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	// registo de funcs
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutMouseFunc(NULL);
	glutReshapeFunc(changeSize);
	glutSpecialFunc(keyboardSpecial);
	glutKeyboardFunc(keyboard);

	// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CCW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	createMenu();

	try{ prepareModels(sceneName); }
	catch (int e){
		exceptionHandler(e);
		return -1;
	}

	// entrar no loop do glut
	glutMainLoop();
	return 1;
}

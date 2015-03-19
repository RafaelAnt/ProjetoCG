#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include "tinyxml2.h"
#include "Draw.h"

//#pragma comment(lib,"glew32.lib")

using namespace std;

/* Esta variável irá conter todos os modelos a desenhar*/
static vector<vector<GLfloat>> models;

/* Variáveis da camara, começa a 5 unidades de distância da origem */
float alfa = 0, beta = 0, raio = 5, step = 0.02;

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

/* Preencher os vetores com os modelos */
void prepareModels(vector<const char*> nomes){
	vector<GLfloat> aux;
	for (unsigned int i = 0; i < nomes.size(); i++){
		aux = readVertices(nomes[i]);
		models.push_back(aux);
	}
}

/* Desenhar os modelos carregados */
void drawModels(){
	for (unsigned int i = 0; i < models.size(); i++){
		drawVertices(models[i]);
	}
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
	glutPostRedisplay();
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

/* 
	Função que analisa o ficheiro XML e determina os modelos a desenhar
	Em caso de problemas, irão ser atiradas as respetivas excepções (ficheiro XML inválido, modelo inexistente, etc)
	A função preenche um vetor com os nomes dos modelos referenciados pelo XML, e irá preparar os vetores que irão
	guardar os seus vértices.
*/
void readScene(char *filename){
	using namespace tinyxml2;
	//Carregar o ficheiro xml
	XMLDocument xmlDoc;
	XMLError eResult = xmlDoc.LoadFile(filename);

	if (eResult != XML_SUCCESS){
		printf("Erro!! %s", xmlDoc.ErrorName());

	}

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
	prepareModels(nomes);
}

void createMenu(){
	glutCreateMenu(menu);
	glutAddMenuEntry("Linha", 1);
	glutAddMenuEntry("Preencher", 2);
	glutAddMenuEntry("Ponto", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main() {
	srand(time(NULL));

	try{ readScene(__argv[1]); }
	catch (int e){
		if (e == 21){
			puts("Erro na leitura da cena, XML parsing error!");
		} if (e == 22){
			puts("Erro na leitura de um dos modelos!");
		}
		else{
			puts("Erro na leitura dos triangulos, excepcao não tratada!");
		}
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
	//glutIdleFunc(renderScene);
	glutMouseFunc(NULL);
	glutReshapeFunc(changeSize);
	glutSpecialFunc(keyboardSpecial);
	glutKeyboardFunc(keyboard);


	//glewInit();

	// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	createMenu();

	// entrar no loop do glut
	glutMainLoop();
	return 1;
}

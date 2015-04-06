#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <vector>
#include <map>
#define _USE_MATH_DEFINES
#include <math.h>
#include "tinyxml2.h"
#include "Draw.h"

using namespace std;

/* Esta variável irá conter todos os modelos a desenhar*/
static map<string, vector<GLfloat>> models;
static char* sceneName;

/* Variáveis da camara, começa a 5 unidades de distância da origem */
static float alfa = 0, beta = 0, raio = 5, step = 0.02;

static void drawNode(tinyxml2::XMLNode *pRoot){
	if (pRoot == NULL)
		return;
	using namespace tinyxml2;
	vector<const char*> modelos;
	XMLNode *modelosGroup; XMLElement *modelo, *aux;
	//obter nodos
	aux = pRoot->FirstChildElement();
	while (aux){
		//obter o node modelos
		modelosGroup = pRoot->FirstChildElement("modelos");
		//percorrer os modelos
		if (modelosGroup){
			modelo = modelosGroup->FirstChildElement("modelo");
			while (modelo) {
				const char * nome;
				nome = modelo->Attribute("ficheiro");
				if (nome) {
					puts(nome);
					modelos.push_back(nome);
				}
				modelo = modelo->NextSiblingElement("modelo");
			}
		}

		//obter translacao
		if (strcmp(aux->Name(), "translacao") == 0){
			Translate t; t.x = 0; t.y = 0; t.z = 0;
			aux->QueryFloatAttribute("X", &t.x);
			aux->QueryFloatAttribute("Y", &t.y);
			aux->QueryFloatAttribute("Z", &t.z);
			glTranslatef(t.x, t.y, t.z);
		}

		//obter escalas
		else if (strcmp(aux->Name(), "escala") == 0){
			Scale s; s.x = 0; s.y = 0; s.z = 0;
			aux->QueryFloatAttribute("X", &s.x);
			aux->QueryFloatAttribute("Y", &s.y);
			aux->QueryFloatAttribute("Z", &s.z);
			glScalef(s.x, s.y, s.z);
		}
		//obter rotacoes
		else if (strcmp(aux->Name(), "rotacao") == 0){
			Rotation r; r.angle = 0; r.x = 0; r.y = 0; r.z = 0;
			aux->QueryFloatAttribute("angulo", &r.angle);
			aux->QueryFloatAttribute("X", &r.x);
			aux->QueryFloatAttribute("Y", &r.y);
			aux->QueryFloatAttribute("Z", &r.z);
			glRotatef(r.angle, r.x, r.y, r.z);
		}
		aux = aux->NextSiblingElement();
	}
	for (int i = 0; i < modelos.size(); i++){
		drawVertices(models.find(string(modelos[i]))->second);
	}
	modelos.clear();
	
	drawNode(pRoot->FirstChildElement("grupo"));
}

void makeTransforms(char *filename){
	//Carregar o ficheiro xml
	using namespace tinyxml2;
	XMLDocument xmlDoc; XMLNode *modelosGroup; XMLElement *modelo, *aux;
	XMLError eResult = xmlDoc.LoadFile(filename);
	if (eResult != XML_SUCCESS){
		printf("Erro!! %s \n", xmlDoc.ErrorName());
		throw 20;
	}
	XMLNode * pRoot = xmlDoc.FirstChild(), *temp;
	if (pRoot == NULL)
		throw 21;

	pRoot = pRoot->FirstChildElement("grupo");
	if (pRoot == NULL){
		puts("Erro");
		throw 19; //ficheiro inválido
	}

	while (pRoot){
		puts("cenas");
		glPushMatrix();
		drawNode(pRoot);
		glPopMatrix();
		pRoot = pRoot->NextSiblingElement("grupo");
	}
}

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

void drawModels(){
	map<string, vector<GLfloat >>::iterator it = models.begin();
	for (it; it != models.end(); it++){
		drawVertices(it->second);
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

	makeTransforms(sceneName);

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



void createMenu(){
	glutCreateMenu(menu);
	glutAddMenuEntry("Linha", 1);
	glutAddMenuEntry("Preencher", 2);
	glutAddMenuEntry("Ponto", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main() {
	srand(time(NULL));

	if (__argc != 2){
		puts("Erro nos argumentos! Especifique apenas o nome da cena a desenhar.\nCertifique-se tambem que o ficheiro esta na diretoria do executavel!");
		return -1;
	}

	sceneName = __argv[1];

	try{ models = prepareModels(sceneName); }
	catch (int e){
		if (e == 21){
			puts("Erro na leitura da cena, XML parsing error!");
		}
		else if (e == 22){
			puts("Erro na leitura de um dos modelos!");
		}
		else if (e == 20){
			puts("Erro!! ");
		}
		else if (e == 1){
			puts("Erro! Triângulos malformados!!");
		}
		else{
			puts("Erro na leitura dos triangulos, excepcao não tratada!");
		}
		return -1;
	}

	printf("%d\n", models.find("esfera.3d")->second.size());

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

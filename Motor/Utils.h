#ifndef UTILS_H

#define UTILS_H

#include <vector>
#include <map>
#include <valarray>
#include <GL/GLUT.h>

using namespace std;

typedef struct point{
	GLfloat x;
	GLfloat y;
	GLfloat z;
}Point;

typedef struct translate{
	GLfloat x;
	GLfloat y;
	GLfloat z;
}Translate;

typedef struct rotation{
	GLfloat angle;
	GLfloat time;
	GLfloat x;
	GLfloat y;
	GLfloat z;
}Rotation;

typedef struct scale{
	GLfloat x;
	GLfloat y;
	GLfloat z;
}Scale;

typedef struct model{
	vector<GLfloat> vertices;
	vector<GLfloat> normais;
	vector<GLfloat> texcoords;
}Model;

#define CG_REPEATED_TRANSFORM 99
#define CG_CURVE_INVALID_TIME 98
#define CG_ROTATION_INVALID_TIME 97
#define CG_REPEATED_MODELS 96
#define CG_INVALID_MODELS 95
#define CG_NO_XML_NODES 94
#define CG_DRAW_WITHOUT_LOAD 93
#define CG_XML_PARSE_ERROR 20
#define CG_INCOMPLETE_TRIANGLE 90
#define CG_INCORRECT_NORMALS_OR_TEX 89

/*
	Desenha uma cena (filename), aplicando as transformações necessárias, utilizando o map models
	para desenhar os modelos com eficiencia.
	*/
void drawScene(char *filename);

/*
	Desenha os vértices armazenados no buffer correspondente.
	*/
void drawVertices(int vboIndex);

/*
	Função que analisa o ficheiro XML e determina os modelos a desenhar
	Em caso de problemas, irão ser atiradas as respetivas excepções (ficheiro XML inválido, modelo inexistente, etc)
	A função guarda todos os modelos em memória inicializando os VBOs.
	*/
void prepareModels(char *filename);

void exceptionHandler(int e);

#endif

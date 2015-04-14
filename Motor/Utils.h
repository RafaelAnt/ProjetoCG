#include <vector>
#include <map>
#include <valarray>
#include <GL/GLUT.h>

using namespace std;

typedef struct translate{
	GLfloat x;
	GLfloat y;
	GLfloat z;
}Translate;

typedef struct rotation{
	GLfloat angle;
	GLfloat x;
	GLfloat y;
	GLfloat z;
}Rotation;

typedef struct scale{
	GLfloat x;
	GLfloat y;
	GLfloat z;
}Scale ;

/*
	Desenha uma cena (filename), aplicando as transformações necessárias, utilizando o map models
	para desenhar os modelos com eficiencia.
*/
void drawScene(char *filename, map<string, int> models);

/*
	Desenha um conjunto de vértices em modo imeadiato. De notar o vetor deve ter um cojunto de triângulos.
	3 pontos de cada vez.
*/
void drawVertices(int vboIndex);

/*
	Função que analisa o ficheiro XML e determina os modelos a desenhar
	Em caso de problemas, irão ser atiradas as respetivas excepções (ficheiro XML inválido, modelo inexistente, etc)
	A função preenche um vetor com os nomes dos modelos referenciados pelo XML, e irá preparar os vetores que irão
	guardar os seus vértices.
	Os modelos serão retornados num vetor de vetores de vértices. Cada vetor de vértices é um modelo.
*/
map<string, int> prepareModels(char *filename);


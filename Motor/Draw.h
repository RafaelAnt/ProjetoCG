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
	Desenha um conjunto de v�rtices em modo imeadiato. De notar o vetor deve ter um cojunto de tri�ngulos.
	3 pontos de cada vez.
*/
void drawVertices(vector<GLfloat> vertices);

/*
	Fun��o que analisa o ficheiro XML e determina os modelos a desenhar
	Em caso de problemas, ir�o ser atiradas as respetivas excep��es (ficheiro XML inv�lido, modelo inexistente, etc)
	A fun��o preenche um vetor com os nomes dos modelos referenciados pelo XML, e ir� preparar os vetores que ir�o
	guardar os seus v�rtices.
	Os modelos ser�o retornados num vetor de vetores de v�rtices. Cada vetor de v�rtices � um modelo.
*/
map<string, vector<GLfloat>> prepareModels(char *filename);


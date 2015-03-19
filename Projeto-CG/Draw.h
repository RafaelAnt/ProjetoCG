#include <vector>
#include <valarray>
#include <GL/GLUT.h>

using namespace std;

/*
	L� os v�rtices de um determinado modelo
*/
vector<GLfloat> readVertices(const char *filename);

/*
	Desenha um conjunto de v�rtices em modo imeadiato. De notar o vetor deve ter um cojunto de tri�ngulos.
	3 pontos de cada vez.
*/
void drawVertices(vector<GLfloat> vertices);


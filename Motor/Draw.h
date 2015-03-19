#include <vector>
#include <valarray>
#include <GL/GLUT.h>

using namespace std;

/*
	Lê os vértices de um determinado modelo
*/
vector<GLfloat> readVertices(const char *filename);

/*
	Desenha um conjunto de vértices em modo imeadiato. De notar o vetor deve ter um cojunto de triângulos.
	3 pontos de cada vez.
*/
void drawVertices(vector<GLfloat> vertices);


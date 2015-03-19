#include <vector>
#include <valarray>
#include <GL/GLUT.h>

using namespace std;

/*
	Desenha um conjunto de vértices em modo imeadiato. De notar o vetor deve ter um cojunto de triângulos.
	3 pontos de cada vez.
*/
void drawVertices(vector<GLfloat> vertices);

/*
	Função que analisa o ficheiro XML e determina os modelos a desenhar
	Em caso de problemas, irão ser atiradas as respetivas excepções (ficheiro XML inválido, modelo inexistente, etc)
	A função preenche um vetor com os nomes dos modelos referenciados pelo XML, e irá preparar os vetores que irão
	guardar os seus vértices.
	Os modelos serão retornados num vetor de vetores de vértices. Cada vetor de vértices é um modelo.
*/
vector <vector<GLfloat>> readScene(char *filename);


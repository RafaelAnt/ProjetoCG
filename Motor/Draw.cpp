#include <valarray>
#include <map>
#include <string>
#include <vector>
#include "tinyxml2.h"
#include "Draw.h"
#include <GL/GLUT.h>

using namespace std;

typedef struct point{
	GLfloat x;
	GLfloat y;
	GLfloat z;
} Point;

/*
	Funçao que desenha um vetor de vértices, cada 3 vértices é um triângulo.
	Atenção!!! A função não irá verificar se o vetor está bem formado, ou seja,
	se tem exatamente um conjunto de vários 3 pontos. Caso o vetor não esteja bem formado
	poderá haver acessos a indices do array fora de limites, o que provavelmente irá resultar num crash.
	Nesta aplicação a verificação é feita no momento da leitura dos vértices dos modelos.
	*/
void drawVertices(vector<GLfloat> vertices){
	Point p1, p2, p3;
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0, j = 0; i < vertices.size(); i += 3, j++){
		p1.x = vertices[i]; p1.y = vertices[i + 1]; p1.z = vertices[i + 2];
		glVertex3f(p1.x, p1.y, p1.z);
		i += 3;
		p2.x = vertices[i]; p2.y = vertices[i + 1]; p2.z = vertices[i + 2];
		glVertex3f(p2.x, p2.y, p2.z);
		i += 3;
		p3.x = vertices[i]; p3.y = vertices[i + 1]; p3.z = vertices[i + 2];
		glVertex3f(p3.x, p3.y, p3.z);
	}
	glEnd();
}

/*
	Função auxiliar que obtem um triângulo de um elemento XML.
	Caso este elemento não tenha 3 pontos, é atirada um excepção que sinaliza o evento erróneo.
	A função aceita um vetor, que ja pode ter elementos, coloca os 3 pontos no fim do vetor, e
	retorna o mesmo.
	*/
static vector<GLfloat> readVertices_aux(tinyxml2::XMLElement *pElement, vector<GLfloat> vertices){
	int i = 0;
	float x, y, z;
	const char *aux;
	while (pElement != NULL) {
		aux = pElement->GetText();
		sscanf_s(aux, "X=%f Y=%f Z=%f", &x, &y, &z);
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		pElement = pElement->NextSiblingElement("vertex");
		i++;
	}
	if (i != 3) throw 1;
	return vertices;
}

/*
	Dado um nome de um ficheiro, esta função irá preencher um vetor com 3 pontos de cada vez, cada 3 pontos representam
	um triângulo.
	Caso se trate de um ficheiro XML inválido, serão atiradas as respetivas excepções.
	Caso os elementos do ficheiro XML não contenham exatamente 3 pontos cada um, também irá ser atirada uma excepção.
	A função irá retornar um vetor com todos os pontos, bem definidos e prontos a serem desenhados.
	*/

static vector<GLfloat> readVertices(const char *filename) {
	using namespace tinyxml2;
	vector<GLfloat> vec;
	tinyxml2::XMLDocument xmlDoc;
	XMLError eResult = xmlDoc.LoadFile(filename);
	XMLNode * pRoot = xmlDoc.FirstChild();
	XMLElement *pElement;
	if (pRoot == nullptr){
		throw 22;
	}
	pElement = pRoot->FirstChildElement("triangle");
	while (pElement != NULL){
		vec = readVertices_aux(pElement->FirstChildElement("vertex"), vec);
		pElement = pElement->NextSiblingElement("triangle");
	}
	return vec;
}

map<string, vector<GLfloat>> prepareModels(char *filename){
	using namespace tinyxml2;
	//Vetor que vai armazenar os nomes dos modelos
	map<string, vector<GLfloat>> modelos;
	//Carregar o ficheiro xml
	XMLDocument xmlDoc; XMLNode *modelosGroup; XMLElement *modelo, *aux;
	XMLError eResult = xmlDoc.LoadFile(filename);
	if (eResult != XML_SUCCESS){
		printf("Erro!! %s \n", xmlDoc.ErrorName());
		throw 20;
	}
	printf("Loaded %s\n", filename);
	XMLNode * pRoot = xmlDoc.FirstChild();
	if (pRoot == NULL)
		throw 21;

	pRoot = pRoot->FirstChildElement("grupo");
	if (pRoot == NULL){
		puts("Erro");
		throw 19; //ficheiro inválido
	}
	while (pRoot != NULL){
		//obter o node modelos
		modelosGroup = pRoot->FirstChildElement("modelos");
		//percorrer os modelos
		if (modelosGroup){
			modelo = modelosGroup->FirstChildElement("modelo");
			while (modelo) {
				const char * nome;
				nome = modelo->Attribute("ficheiro");
				if (nome) {
					if (modelos.count(nome) == 0){
						modelos[string(nome)] = readVertices(nome);
					}
				}
				modelo = modelo->NextSiblingElement("modelo");
			}
		}
		//ir para o próximo grupo
		pRoot = pRoot->FirstChildElement("grupo");
	}
	printf("%d\n", modelos.size());
	return modelos;
}


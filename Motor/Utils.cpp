#include <GL/glew.h>
#include <valarray>
#include <map>
#include <string>
#include <vector>
#include "tinyxml2.h"
#include "Utils.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/GLUT.h>

#pragma comment(lib, "glew32.lib")

using namespace std;

tinyxml2::XMLDocument xmlDoc;

bool loaded = false;
GLuint *vbo;
vector<int> sizes;

//de casteljau's algorithm!!
Point bezierCurve(vector<Point> pontos, GLfloat tempo){
	GLfloat useless, t = modf(tempo, &useless);
	vector<Point> q(pontos);
	int k, i;
	for (k = 1; k < pontos.size(); k++){
		for (i = 0; i < pontos.size() - k; i++){
			q[i].x = (1 - t)*q[i].x + t*q[i + 1].x;
			q[i].y = (1 - t)*q[i].y + t*q[i + 1].y;
			q[i].z = (1 - t)*q[i].z + t*q[i + 1].z;
		}
	}
	return q[0];
}

//proot é o grupo a desenhar
static void drawNode(tinyxml2::XMLNode *pRoot, map<string, int> models){
	//se null, fazer pop (chegamos ao fim da hierarquia)
	if (pRoot == NULL){
		glPopMatrix();
		return;
	}
	//push matrix, transformações apenas efetuadas 
	//aos modelos deste grupo e aos filhos
	glPushMatrix();
	using namespace tinyxml2;
	//nome modelos
	vector<const char*> modelos;
	//variaveis de controlo de transforms duplicadas
	bool trans = false, esc = false, rot = false;
	XMLNode *modelosGroup; XMLElement *modelo, *aux;
	//obter nodos
	aux = pRoot->FirstChildElement();

	//obter o node modelos
	modelosGroup = pRoot->FirstChildElement("modelos");
	//percorrer os modelos
	if (modelosGroup){
		modelo = modelosGroup->FirstChildElement("modelo");
		while (modelo) {
			const char * nome;
			nome = modelo->Attribute("ficheiro");
			if (nome) {
				//guardar modelo
				modelos.push_back(nome);
			}
			modelo = modelo->NextSiblingElement("modelo");
		}
		XMLElement *test = modelosGroup->NextSiblingElement();
		//nodos a seguir a modelo = ERR!!
		if (modelosGroup->NextSiblingElement("modelos") != NULL){
			throw CG_REPEATED_MODELS; //REPEATED MODELS
		}
	}

	//enquanto nao se esgotarem os elementos no grupo
	while (aux){
		//obter translacao
		if (strcmp(aux->Name(), "translação") == 0){
			//mais que uma translacao = exception
			if (trans)
				throw CG_REPEATED_TRANSFORM; //REPEATED TRANSFORM
			XMLElement *ponto = aux->FirstChildElement("ponto");
			if (ponto == NULL){
				Translate t; t.x = 0; t.y = 0; t.z = 0;
				aux->QueryFloatAttribute("X", &t.x);
				aux->QueryFloatAttribute("Y", &t.y);
				aux->QueryFloatAttribute("Z", &t.z);
				glTranslatef(t.x, t.y, t.z);
			}
			else{
				GLfloat tempo = -1;
				aux->QueryFloatAttribute("tempo", &tempo);
				if (tempo == -1){
					//tempo inválido ou não especificado!
					throw CG_CURVE_INVALID_TIME;
				}
				//falta verificaçao de erros
				vector<Point> pontos;
				while (ponto != NULL){
					Point t; t.x = 0; t.y = 0; t.z = 0;
					ponto->QueryFloatAttribute("X", &t.x);
					ponto->QueryFloatAttribute("Y", &t.y);
					ponto->QueryFloatAttribute("Z", &t.z);
					pontos.push_back(t);
					ponto = ponto->NextSiblingElement();
				}
				//colocar o tempo decorrido na timescale desejada
				Point objetivo = bezierCurve(pontos, glutGet(GLUT_ELAPSED_TIME) / (tempo * 1000));
				glTranslatef(objetivo.x, objetivo.y, objetivo.z);
			}
			trans = true;
		}

		//obter escalas
		else if (strcmp(aux->Name(), "escala") == 0){
			//mais que uma escala = exception
			if (esc)
				throw CG_REPEATED_TRANSFORM;
			Scale s; s.x = 1; s.y = 1; s.z = 1;
			aux->QueryFloatAttribute("X", &s.x);
			aux->QueryFloatAttribute("Y", &s.y);
			aux->QueryFloatAttribute("Z", &s.z);
			glScalef(s.x, s.y, s.z);
			esc = true;
		}
		//obter rotacoes
		else if (strcmp(aux->Name(), "rotação") == 0){
			//mais que uma rotacao = exception
			if (rot)
				throw CG_REPEATED_TRANSFORM;
			Rotation r; r.angle = -1; r.time = -1; r.x = 0; r.y = 0; r.z = 0;
			aux->QueryFloatAttribute("angulo", &r.angle);
			aux->QueryFloatAttribute("tempo", &r.time);
			aux->QueryFloatAttribute("eixoX", &r.x);
			aux->QueryFloatAttribute("eixoY", &r.y);
			aux->QueryFloatAttribute("eixoZ", &r.z);
			if (r.angle > 0)
				glRotatef(r.angle, r.x, r.y, r.z);
			else if (r.time > 0)
				glRotatef((360)*(glutGet(GLUT_ELAPSED_TIME) / (r.time * 1000)), r.x, r.y, r.z);
			rot = true;
		}
		//avaliar proximo elemento
		aux = aux->NextSiblingElement();
	}
	for (unsigned int i = 0; i < modelos.size(); i++){
		//desenhar modelos
		drawVertices(models[modelos[i]]);
	}
	modelos.clear();

	//desenhar os filhos
	drawNode(pRoot->FirstChildElement("grupo"), models);
	//depois desenhar os irmaos
	drawNode(pRoot->NextSiblingElement("grupo"), models);
}

void drawScene(char *filename, map<string, int> models){
	//Carregar o ficheiro xml
	using namespace tinyxml2;
	XMLNode *pRoot;
	//se a cena ainda não foi carregada, atirar exception
	if (!loaded){
		throw CG_DRAW_WITHOUT_LOAD;
	}
	else{
		pRoot = xmlDoc.FirstChild()->FirstChildElement("grupo");
	}
	drawNode(pRoot, models);
}

/*
	Funçao que desenha o array de vértices armazenados num dado buffer (vboIndex)
	*/
void drawVertices(int vboIndex){
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[vboIndex]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, sizes[vboIndex] / 3);
	glDisableClientState(GL_VERTEX_ARRAY);
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
		throw CG_INVALID_MODELS;
	}
	pElement = pRoot->FirstChildElement("triangle");
	while (pElement != NULL){
		vec = readVertices_aux(pElement->FirstChildElement("vertex"), vec);
		pElement = pElement->NextSiblingElement("triangle");
	}
	return vec;
}

//passar map por referencia
static void auxPrepare(map<string, vector<GLfloat>> *modelos, tinyxml2::XMLNode *pRoot){
	using namespace tinyxml2;
	if (pRoot == NULL)
		return;
	XMLDocument xmlDoc; XMLNode *modelosGroup; XMLElement *modelo;
	//obter o grupo modelos
	modelosGroup = pRoot->FirstChildElement("modelos");
	//percorrer os modelos
	if (modelosGroup){
		modelo = modelosGroup->FirstChildElement("modelo");
		while (modelo) {
			const char * nome;
			nome = modelo->Attribute("ficheiro");
			if (nome) {
				if (modelos->count(nome) == 0){
					//guardar modelos no map
					(*modelos)[string(nome)] = readVertices(nome);
				}
			}
			modelo = modelo->NextSiblingElement("modelo");
		}
	}
	auxPrepare(&(*modelos), pRoot->FirstChildElement("grupo"));
	auxPrepare(&(*modelos), pRoot->NextSiblingElement("grupo"));
}

/*
	Função que lê uma cena XML e armazena todos os modelos a desenhar nos respetivos buffers, e devolve um map com o nome
	do modelo e do respetivo identificador do buffer
	*/
map<string, int> prepareModels(char *filename){
	using namespace tinyxml2;
	//Map que vai armazenar os modelos
	map<string, vector<GLfloat>> modelos;
	//Carregar o ficheiro xml
	XMLError eResult = xmlDoc.LoadFile(filename);
	loaded = true;
	//test erros
	if (eResult != XML_SUCCESS){
		printf("Erro!! %s \n", xmlDoc.ErrorName());
		throw CG_XML_PARSE_ERROR;
	}
	//confirm load
	printf("Loaded %s\n", filename);
	//obter node inicial
	XMLNode * pRoot = xmlDoc.FirstChild();
	//erro de empty xml
	if (pRoot == NULL)
		throw CG_NO_XML_NODES;

	pRoot = pRoot->FirstChildElement("grupo");
	//ficheiro sem grupos
	if (pRoot == NULL){
		throw CG_NO_XML_NODES; //ficheiro inválido
	}
	//obter os modelos
	auxPrepare(&modelos, pRoot);

	vbo = new GLuint[modelos.size()];
	glGenBuffers(modelos.size(), vbo);
	map<string, int> vboIndex;
	typedef map<string, vector<GLfloat>>::iterator it_type;
	int index = 0;
	for (it_type iterator = modelos.begin(); iterator != modelos.end(); iterator++) {
		vector<GLfloat> vec = iterator->second;
		vboIndex[iterator->first] = index;
		sizes.push_back(vec.size());
		glBindBuffer(GL_ARRAY_BUFFER, vbo[index++]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vec.size(), &vec[0], GL_STATIC_DRAW);
	}
	return vboIndex;
}


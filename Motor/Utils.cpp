#include <GL/glew.h>
#include <valarray>
#include <map>
#include <string>
#include <vector>
#include "tinyxml2.h"
#include "Utils.h"
#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/GLUT.h>
#include <IL/il.h>

#pragma comment(lib, "glew32.lib")

using namespace std;

tinyxml2::XMLDocument xmlDoc;

vector<Model> modelos_GLOBAL;
bool loaded = false;
GLuint *vbo;
vector<int> sizes;

void exceptionHandler(int e){
	if (e == CG_CURVE_INVALID_TIME){
		puts("Tempo de movimento da curva inválido! Inteiros positivos apenas!");
	}
	else if (e == CG_INVALID_MODELS){
		puts("Erro na leitura de um dos modelos!");
	}
	else if (e == CG_DRAW_WITHOUT_LOAD){
		puts("Erro!! A aplicação tentou desenhar modelos sem os carregar!!");
		puts("ISTO N E SUPOSTO ACONTECER.");
	}
	else if (e == CG_NO_XML_NODES){
		puts("A CENA NÃO POSSUI NODOS XML!!");
	}
	else if (e == CG_REPEATED_MODELS){
		puts("Erro!! Apenas pode existir um zona de modelos por grupo!!");
	}
	else if (e == CG_REPEATED_TRANSFORM){
		puts("Erro!! Apenas pode existir uma transformação de cada tipo por grupo!!");
	}
	else if (e == CG_ROTATION_INVALID_TIME){
		puts("Tempo de rotação inválido! Inteiros positivos apenas!");
	}
	else if (e == CG_XML_PARSE_ERROR){
		puts("Erro de parsing do XML!");
	}
	else{
		puts("UNHANDLED EXCEPTION! ABORT");
	}
}


/*
Funçao que desenha o array de vértices armazenados num dado buffer (vboIndex)
*/
void drawVertices(Model model){
	float aux[4];
	for (int i = 0; i < model.material.size(); i++){
		Material m = model.material[i];
		aux[0] = m.red; aux[1] = m.green; aux[2] = m.blue; aux[3] = 0;
		glMaterialfv(GL_FRONT, model.material[i].type, aux);
	}
	glBindTexture(GL_TEXTURE_2D, model.texID);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[model.index]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, model.vertices.size() / 3);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//proot é o grupo a desenhar, n é o próximo modelo a desenhar
//retorna o índice do modelo a desenhar para utilizar na chamada recursiva
static int drawNode(tinyxml2::XMLNode *pRoot, int n){
	//se null, fazer pop (chegamos ao fim da hierarquia)
	if (pRoot == NULL){
		glPopMatrix();
		//esatmos num grupo "vazio" retornar o indice atual
		return n;
	}
	//push matrix, transformações apenas efetuadas 
	//aos modelos deste grupo e aos filhos
	glPushMatrix();
	using namespace tinyxml2;
	//indices dos modelos a desenhar no fim
	vector<int> modelos;
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
				modelos.push_back(n++);
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
				catmullRomCurveMovement(glutGet(GLUT_ELAPSED_TIME) / (tempo * 1000), pontos);
				//glPopMatrix();
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
			else if (r.time > 0){
				//calcular velocidade angular em graus/milisegundo
				GLfloat velAngular = 360.0 / (r.time * 1000);
				//angulo a percorrer = velAngular * t
				glRotatef(velAngular * glutGet(GLUT_ELAPSED_TIME), r.x, r.y, r.z);
			}
			rot = true;
		}
		//avaliar proximo elemento
		aux = aux->NextSiblingElement();
	}
	for (unsigned int i = 0; i < modelos.size(); i++){
		drawVertices(modelos_GLOBAL[modelos[i]]);
	}
	modelos.clear();

	//desenhar os filhos, tomar nota do último modelo desenhado neste grupo
	n = drawNode(pRoot->FirstChildElement("grupo"), n);
	//depois desenhar os irmaos
	drawNode(pRoot->NextSiblingElement("grupo"), n);
}

void drawScene(char *filename){
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
	drawNode(pRoot, 0);
}

/*
	Função auxiliar que obtem um triângulo de um elemento XML.
	Caso este elemento não tenha 3 vértices e 3 normais, é atirada um excepção que sinaliza o evento erróneo.
	A função aceita uma estrutura do tipo Model, e atualiza com a informação necessária.
	*/
static void readVertices_aux(tinyxml2::XMLElement *pElement, Model *model){
	int i = 0;
	// EM CASO DE OMISSÃO DE VALORES, ASSUMIMOS HIPOTETICO VALOR COMO 0
	float x = 0, y = 0, z = 0;
	float nx = 0, ny = 0, nz = 0;
	float u = 0, v = 0;
	pElement = pElement->FirstChildElement("vertex");
	while (pElement != NULL) {
		sscanf_s(pElement->GetText(), "X=%f Y=%f Z=%f", &x, &y, &z);
		model->vertices.push_back(x);
		model->vertices.push_back(y);
		model->vertices.push_back(z);
		pElement = pElement->NextSiblingElement("vertex");
		i++;
	}
	if (i != 3) throw CG_INCOMPLETE_TRIANGLE;
	/* pARA JA NAO TEMOS NORMAIS NEM TEXTURAS
	i = 0;
	pElement = pElement->FirstChildElement("normal");
	while (pElement != NULL) {
	pElement->QueryFloatAttribute("X", &x);
	pElement->QueryFloatAttribute("Y", &y);
	pElement->QueryFloatAttribute("Z", &z);
	model->normais.push_back(x);
	model->normais.push_back(y);
	model->normais.push_back(z);
	pElement = pElement->NextSiblingElement("normal");
	i++;
	}
	if (i != 3) throw CG_INCOMPLETE_TRIANGLE;
	i = 0;
	pElement = pElement->FirstChildElement("texcoord");
	while (pElement != NULL) {
	pElement->QueryFloatAttribute("X", &x);
	pElement->QueryFloatAttribute("Y", &y);
	pElement->QueryFloatAttribute("Z", &z);
	model->texcoords.push_back(x);
	model->texcoords.push_back(y);
	model->texcoords.push_back(z);
	pElement = pElement->NextSiblingElement("texcoord");
	i++;
	}
	if (i>0 && i!=3) throw CG_INCOMPLETE_TRIANGLE;
	*/
}

/*
	Dado um nome de um ficheiro, esta função irá preencher um vetor com 3 pontos de cada vez, cada 3 pontos representam
	um triângulo.
	Caso se trate de um ficheiro XML inválido, serão atiradas as respetivas excepções.
	Caso os elementos do ficheiro XML não contenham exatamente 3 pontos cada um, também irá ser atirada uma excepção.
	A função irá retornar um vetor com todos os pontos, bem definidos e prontos a serem desenhados.
	*/

static Model readVertices(const char *filename) {
	using namespace tinyxml2;
	Model model;
	tinyxml2::XMLDocument xmlDoc;
	XMLError eResult = xmlDoc.LoadFile(filename);
	XMLNode * pRoot = xmlDoc.FirstChild();
	XMLElement *pElement;
	if (pRoot == nullptr){
		throw CG_INVALID_MODELS;
	}
	pElement = pRoot->FirstChildElement("triangle");
	while (pElement != NULL){
		readVertices_aux(pElement, &model);
		pElement = pElement->NextSiblingElement("triangle");
	}
	return model;
}

static GLuint loadTexture(const char *texture){
	unsigned int t, tw, th;
	unsigned char *texData;
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)texture);
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();
	GLuint texID;
	glGenTextures(1, &texID); // unsigned int texID - variavel global;
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texID;
}

//auxiliar que preenche obtem os modelos
static void auxPrepare(vector<Model> *modelos, tinyxml2::XMLNode *pRoot){
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
			Model model;
			const char * nome, *texture;
			nome = modelo->Attribute("ficheiro");
			if (nome) {
				//guardar modelos no map
				model=readVertices(nome);
			}
			texture = modelo->Attribute("textura");
			if (texture) {
				model.texID = loadTexture(texture);
				if (model.texcoords.size() == 0)
					throw CG_NO_TEXTURE_COORDINATES;
			}
			else{
				model.texID = 0;
			}
			//ler cores e componentes (TO DO)
			Material diffuse; diffuse.type = GL_DIFFUSE;
			diffuse.red = -1; diffuse.green = -1; diffuse.blue = -1;
			modelo->QueryFloatAttribute("diffR", &diffuse.red);
			modelo->QueryFloatAttribute("diffG", &diffuse.green);
			modelo->QueryFloatAttribute("diffB", &diffuse.blue);
			if (!(diffuse.red == -1 || diffuse.green == -1 || diffuse.blue == -1))
				model.material.push_back(diffuse);

			Material ambient; ambient.type = GL_AMBIENT;
			ambient.red = -1; ambient.green = -1; ambient.blue = -1;
			modelo->QueryFloatAttribute("ambR", &ambient.red);
			modelo->QueryFloatAttribute("ambG", &ambient.green);
			modelo->QueryFloatAttribute("ambB", &ambient.blue);
			if (!(ambient.red == -1 || ambient.green == -1 || ambient.blue == -1))
				model.material.push_back(ambient);

			Material specular; specular.type = GL_SPECULAR;
			specular.red = 0; specular.green = 0; specular.blue = 0;
			modelo->QueryFloatAttribute("specR", &specular.red);
			modelo->QueryFloatAttribute("specG", &specular.green);
			modelo->QueryFloatAttribute("specB", &specular.blue);
			if (!(specular.red == -1 || specular.green == -1 || specular.blue == -1))
				model.material.push_back(specular);

			model.index = modelos->size();
			printf("%d\n", model.index);
			modelos->push_back(model);
			modelo = modelo->NextSiblingElement("modelo");
		}
	}
	auxPrepare(&(*modelos), pRoot->FirstChildElement("grupo"));
	auxPrepare(&(*modelos), pRoot->NextSiblingElement("grupo"));
}

/*
	Função que lê uma cena XML e armazena todos os modelos a desenhar nos respetivos buffers
	*/
void prepareModels(char *filename){
	using namespace tinyxml2;
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
	//Map que vai armazenar os modelos
	vector<Model> modelos;
	//obter os modelos, auxPrepare analisa a cena XML
	//e preenche o map com todos os modelos (sem repetições!)
	auxPrepare(&modelos, pRoot);
	modelos_GLOBAL = modelos;
	//inicializar buffers
	vbo = new GLuint[modelos.size()];
	glGenBuffers(modelos.size(), vbo);
	//map que irá associar nome de modelo ao número do buffer
	map<string, int> vboIndex;
	//tipo do iterador
	typedef vector<Model>::iterator it_type;
	//inicializar indice
	int index = 0;
	//para todos os modelos obtidos criar um buffer
	for (it_type iterator = modelos.begin(); iterator != modelos.end(); iterator++) {
		//atualizar vetor de tamanhos, para que na altura do 
		//desenho se saiba o numero de triangulos a desenhar
		sizes.push_back(iterator->vertices.size());
		//fazer bind do buffer
		glBindBuffer(GL_ARRAY_BUFFER, vbo[index]);
		//guardar vértices do modelo no buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*iterator->vertices.size(), &iterator->vertices[0], GL_STATIC_DRAW);
		//guardar normais
		//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*iterator->normais.size(), &iterator->normais[0], GL_STATIC_DRAW);
		//guardar texturas SE EXISTIREM
		//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*iterator->texcoords.size(), &iterator->texcoords[0], GL_STATIC_DRAW);

		//atualizar indice do buffer para a proxima iteração
		index++;
	}
}


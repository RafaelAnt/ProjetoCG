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
using namespace tinyxml2;

tinyxml2::XMLDocument xmlDoc;

int nluzes = 0;

vector<Model> modelos_GLOBAL;
bool loaded = false;
GLuint *vertexBuffer;
GLuint *normalBuffer;
GLuint *texBuffer;


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
	else if (e == CG_NO_TEXTURE_COORDINATES){
		puts("Um dos modelos não possui coordenadas de textura!");
	}
	else if (e == CG_INCORRECT_NORMALS_OR_TEX){
		puts("Um dos modelos não possui as normais ou as coordenadas de textura necessárias!");
	}
	else if (e == CG_INCOMPLETE_TRIANGLE){
		puts("Um dos triangulos dos modelos não possui a informaçãos toda!");
	}
	else if (e == CG_LIGHT_WITHOUT_TYPE){
		puts("Uma das luzes não tem tipo!");
	}
	else{
		puts("UNHANDLED EXCEPTION! ABORT");
	}
}


/*
Funçao que desenha o array de vértices armazenados num dado buffer (vboIndex)
*/
void drawVertices(Model model){
	for (unsigned int i = 0; i < model.material.size(); i++){
		Material m = model.material[i];
		float aux[4] = { 0, 0, 0, 1 };
		aux[0] = m.red; aux[1] = m.green; aux[2] = m.blue;
		printf("%d %f %f %f \n", model.index, aux[0], aux[1], aux[2], aux[3]);
		glMaterialfv(GL_FRONT_AND_BACK, m.type, aux);
	}
	//selecionar textura do modelo
	glBindTexture(GL_TEXTURE_2D, model.texID);
	//enables dos VBO
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//selecionar os vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[model.index]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	//selecionar as normais
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer[model.index]);
	glNormalPointer(GL_FLOAT, 0, 0);
	//selecionar as coordenadas de textura
	glBindBuffer(GL_ARRAY_BUFFER, texBuffer[model.index]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
	//desenhar
	glDrawArrays(GL_TRIANGLES, 0, model.vertices.size() / 3);
	//disable dos VBO
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//selecionar textura "nula"
	glBindTexture(GL_TEXTURE_2D, 0);
	
}

//proot é o grupo a desenhar, n é o próximo modelo a desenhar
//retorna o índice do modelo a desenhar para utilizar na chamada recursiva
static int drawNode(XMLNode *pRoot, int n){
	//se null, fazer pop (chegamos ao fim da hierarquia)
	if (pRoot == NULL){
		glPopMatrix();
		//esatmos num grupo "vazio" retornar o indice atual
		return n;
	}
	//push matrix, transformações apenas efetuadas 
	//aos modelos deste grupo e aos filhos
	glPushMatrix();
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
	n = drawNode(pRoot->NextSiblingElement("grupo"), n);
	return n;
}

//node deverá ser a raiz do ficheiro XML
static void prepareLights(XMLNode *node){
	XMLNode *lightsGroup = node->FirstChildElement("luzes");
	XMLElement *aux = lightsGroup->FirstChildElement("luz");
	while (aux != NULL){
		const char *tipo = aux->Attribute("tipo");
		if (!tipo)
			throw CG_LIGHT_WITHOUT_TYPE;
		float x = 0, y = 0, z = 0, R = 0, G = 0, B = 0;
		//tipo ponto utiliza array com a localização no espaço
		if (strcmp(tipo, "PONTO") == 0){
			GLfloat arr[4] = { 0.0, 0.0, 0.0, 1 };
			aux->QueryFloatAttribute("posX", &x);
			aux->QueryFloatAttribute("posY", &y);
			aux->QueryFloatAttribute("posZ", &z);
			arr[0] = x; arr[1] = y; arr[2] = z;
			glLightfv(GL_LIGHT0, GL_POSITION, arr);
		}
		//os outros tipos utilizam uma cor
		else{
			int tipoID = -1;
			if (strcmp(tipo, "AMBIENTE") == 0)
				tipoID = GL_AMBIENT;
			else if (strcmp(tipo, "ESPECULAR") == 0)
				tipoID = GL_SPECULAR;
			else if (strcmp(tipo, "DIFUSA") == 0)
				tipoID = GL_DIFFUSE;
			
			if (tipoID != -1){
				GLfloat arr[4] = { 0.0, 0.0, 0.0, 0.0 };
				aux->QueryFloatAttribute("R", &R);
				aux->QueryFloatAttribute("G", &G);
				aux->QueryFloatAttribute("B", &B);
				arr[0] = R; arr[1] = G; arr[2] = B;
				glLightfv(GL_LIGHT0, tipoID, arr);
			}
		}
		//ver os outros elementos do grupo
		aux = aux->NextSiblingElement("luz");
	}
}


void drawScene(char *filename){
	//Carregar o ficheiro xml
	XMLNode *pRoot;
	//se a cena ainda não foi carregada, atirar exception
	if (!loaded){
		throw CG_DRAW_WITHOUT_LOAD;
	}
	else{
		pRoot = xmlDoc.FirstChild();
	}
	prepareLights(pRoot);
	drawNode(pRoot->FirstChildElement("grupo"), 0);
}

/*
	Função auxiliar que obtem um triângulo de um elemento XML.
	Caso este elemento não tenha 3 vértices ,3 normais e 3 coordenadas de textura, 
	é atirada um excepção que sinaliza o evento erróneo.
	A função aceita uma estrutura do tipo Model, e atualiza a com a informação necessária.
	*/
static void readVertices_aux(XMLElement *pElement, Model *model){
	int i = 0;
	// EM CASO DE OMISSÃO DE VALORES, ASSUMIMOS HIPOTETICO VALOR COMO 0
	float x = 0, y = 0, z = 0;
	float nx = 0, ny = 0, nz = 0;
	float u = 0, v = 0;
	pElement = pElement->FirstChildElement("vertex");
	XMLElement *head = pElement;
	while (pElement != NULL) {
		sscanf_s(pElement->GetText(), "X=%f Y=%f Z=%f", &x, &y, &z);
		model->vertices.push_back(x);
		model->vertices.push_back(y);
		model->vertices.push_back(z);
		pElement = pElement->NextSiblingElement("vertex");
		i++;
	}
	if (i != 3) throw CG_INCOMPLETE_TRIANGLE;
	i = 0;
	pElement = head->NextSiblingElement("normal");
	while (pElement != NULL) {
		sscanf_s(pElement->GetText(), "X=%f Y=%f Z=%f", &x, &y, &z);
		model->normais.push_back(x);
		model->normais.push_back(y);
		model->normais.push_back(z);
		pElement = pElement->NextSiblingElement("normal");
		i++;
	}
	if (i != 3) throw CG_INCOMPLETE_TRIANGLE;
	i = 0;
	pElement = head->NextSiblingElement("texcoord");
	while (pElement != NULL) {
		sscanf_s(pElement->GetText(), "U=%f V=%f", &x, &y);
		model->texcoords.push_back(x);
		model->texcoords.push_back(y);
		pElement = pElement->NextSiblingElement("texcoord");
		i++;
	}
	if (i != 3) throw CG_INCOMPLETE_TRIANGLE;
}

/*
	Dado um nome de um ficheiro, uma estrutura do tipo Model com as informações necessárias (normais, texturas, vertices, etc)
	Caso se trate de um ficheiro XML inválido, serão atiradas as respetivas excepções.
	A função deteta os erros de mà formação dos modelos (falta de normais ou texturas, triangulos com um número de pontos errado)
	A convenção é a seguinte: um triangulo contém 3 vértices, 3 normais e 3 coordenadas textura.
	A função irá retornar a estrutura do tipo Model.
	*/

static Model readVertices(const char *filename) {
	using namespace tinyxml2;
	Model model;
	XMLDocument xmlDoc;
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
	printf("Loaded %s texture ID=%d\n", texture, texID);
	return texID;
}

//auxiliar que obtem os modelos
static void auxPrepare(vector<Model> *modelos, tinyxml2::XMLNode *pRoot){
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
				//guardar model na estrutura
				model = readVertices(nome);
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
			//ler cores e componentes
			Material diffuse; diffuse.type = GL_DIFFUSE;
			diffuse.red = -1; diffuse.green = -1; diffuse.blue = -1;
			modelo->QueryFloatAttribute("diffR", &diffuse.red);
			modelo->QueryFloatAttribute("diffG", &diffuse.green);
			modelo->QueryFloatAttribute("diffB", &diffuse.blue);
			if (diffuse.red !=-1 && 
				diffuse.green != -1 && 
				diffuse.blue !=-1)
				
				model.material.push_back(diffuse);

			Material ambient; ambient.type = GL_AMBIENT;
			ambient.red = -1; ambient.green = -1; ambient.blue = -1;
			modelo->QueryFloatAttribute("ambR", &ambient.red);
			modelo->QueryFloatAttribute("ambG", &ambient.green);
			modelo->QueryFloatAttribute("ambB", &ambient.blue);
			if (ambient.red != -1 && 
				ambient.green != -1 &&
				ambient.blue != -1)
				
				model.material.push_back(ambient);

			Material specular; specular.type = GL_SPECULAR;
			specular.red = -1; specular.green = -1; specular.blue = -1;
			modelo->QueryFloatAttribute("specR", &specular.red);
			modelo->QueryFloatAttribute("specG", &specular.green);
			modelo->QueryFloatAttribute("specB", &specular.blue);
			if (specular.red != -1 && 
				specular.green != -1 && 
				specular.blue != -1)

				model.material.push_back(specular);

			Material emission; emission.type = GL_EMISSION;
			emission.red = -1; emission.green = -1; emission.blue = -1;
			modelo->QueryFloatAttribute("emiR", &emission.red);
			modelo->QueryFloatAttribute("emiG", &emission.green);
			modelo->QueryFloatAttribute("emiB", &emission.blue);
			if (emission.red != -1 &&
				emission.green != -1 &&
				emission.blue != -1)

				model.material.push_back(emission);

			model.index = modelos->size();
			modelos->push_back(model);
			modelo = modelo->NextSiblingElement("modelo");
		}
	}
	auxPrepare(&(*modelos), pRoot->FirstChildElement("grupo"));
	auxPrepare(&(*modelos), pRoot->NextSiblingElement("grupo"));
}

/*
	Função que lê uma cena XML e armazena todos os modelos a desenhar nos respetivos buffers.
	Além disso, prepare e inicializa a iluminação, caso exista.
	*/
void prepareModels(char *filename){
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
	//vetor que armazena os modelos
	vector<Model> modelos;
	//obter os modelos, auxPrepare analisa a cena XML
	//e preenche o map com todos os modelos (sem repetições!)
	auxPrepare(&modelos, pRoot);
	//variável global que armazena os modelos
	modelos_GLOBAL = modelos;
	//inicializar buffers
	vertexBuffer = new GLuint[modelos.size()];
	normalBuffer = new GLuint[modelos.size()];
	texBuffer = new GLuint[modelos.size()];
	glGenBuffers(modelos.size(), vertexBuffer);
	glGenBuffers(modelos.size(), normalBuffer);
	glGenBuffers(modelos.size(), texBuffer);
	//map que irá associar nome de modelo ao número do buffer
	map<string, int> vboIndex;
	//tipo do iterador
	typedef vector<Model>::iterator it_type;
	//inicializar indice
	int index = 0;
	//para todos os modelos obtidos criar um buffer
	for (it_type iterator = modelos.begin(); iterator != modelos.end(); iterator++) {
		//fazer bind do buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[index]);
		//guardar vértices do modelo no buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*iterator->vertices.size(), &iterator->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer[index]);
		//guardar normais
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*iterator->normais.size(), &iterator->normais[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, texBuffer[index]);
		//guardar texturas SE EXISTIREM
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*iterator->texcoords.size(), &iterator->texcoords[0], GL_STATIC_DRAW);

		//atualizar indice do buffer para a proxima iteração
		index++;
	}
}


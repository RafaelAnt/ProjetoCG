#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include "tinyxml2.h"
#include <sstream>

typedef struct ponto{
	double x;
	double y;
	double z;
}Ponto;

tinyxml2::XMLDocument xmlDoc;

static void writeVertexToXML(tinyxml2::XMLNode * pRoot, Ponto v){
	using namespace tinyxml2;
	char text[1024];
	XMLElement *elem = xmlDoc.NewElement("vertex");
	sprintf_s(text, "X=%g Y=%g Z=%g", v.x, v.y, v.z);
	elem->SetText(text);
	pRoot->InsertEndChild(elem);
}

static void writeVertexToXML(tinyxml2::XMLNode * pRoot, double x, double y, double z){
	using namespace tinyxml2;
	char text[1024];
	XMLElement *elem = xmlDoc.NewElement("vertex");
	sprintf_s(text, "X=%g Y=%g Z=%g", x, y, z);
	elem->SetText(text);
	pRoot->InsertEndChild(elem);
}

static void writeTriangleToXML(tinyxml2::XMLNode * pRoot, Ponto v1, Ponto v2, Ponto v3){
	using namespace tinyxml2;
	XMLNode *triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, v1);
	writeVertexToXML(triangle, v2);
	writeVertexToXML(triangle, v3);
	pRoot->InsertEndChild(triangle);
}

void drawParallelpipedXML(float width, float height, float lenght, char* filename){
	using namespace tinyxml2;
	XMLNode * pRoot = xmlDoc.NewElement("paralelipipedo");
	xmlDoc.InsertFirstChild(pRoot);

	XMLNode *triangle = xmlDoc.NewElement("triangle");
	//face frente
	writeVertexToXML(triangle, width / 2, height / 2, lenght / 2);
	writeVertexToXML(triangle, width / 2, -height / 2, lenght / 2);
	writeVertexToXML(triangle, -width / 2, height / 2, lenght / 2);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, -width / 2, height / 2, lenght / 2);
	writeVertexToXML(triangle, width / 2, -height / 2, lenght / 2);
	writeVertexToXML(triangle, -width / 2, -height / 2, lenght / 2);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	//face direita
	writeVertexToXML(triangle, width / 2, height / 2, lenght / 2);
	writeVertexToXML(triangle, width / 2, height / 2, -lenght / 2);
	writeVertexToXML(triangle, width / 2, -height / 2, -lenght / 2);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, width / 2, height / 2, lenght / 2);
	writeVertexToXML(triangle, width / 2, -height / 2, -lenght / 2);
	writeVertexToXML(triangle, width / 2, -height / 2, lenght / 2);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	//face  cima
	writeVertexToXML(triangle, width / 2, height / 2, lenght / 2);
	writeVertexToXML(triangle, -width / 2, height / 2, lenght / 2);
	writeVertexToXML(triangle, width / 2, height / 2, -lenght / 2);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, -width / 2, height / 2, lenght / 2);
	writeVertexToXML(triangle, -width / 2, height / 2, -lenght / 2);
	writeVertexToXML(triangle, width / 2, height / 2, -lenght / 2);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	//face esquerda
	writeVertexToXML(triangle, -width / 2, height / 2, lenght / 2);
	writeVertexToXML(triangle, -width / 2, -height / 2, -lenght / 2);
	writeVertexToXML(triangle, -width / 2, height / 2, -lenght / 2);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, -width / 2, height / 2, lenght / 2);
	writeVertexToXML(triangle, -width / 2, -height / 2, lenght / 2);
	writeVertexToXML(triangle, -width / 2, -height / 2, -lenght / 2);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	//face baixo
	writeVertexToXML(triangle, -width / 2, -height / 2, lenght / 2);
	writeVertexToXML(triangle, width / 2, -height / 2, -lenght / 2);
	writeVertexToXML(triangle, -width / 2, -height / 2, -lenght / 2);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, width / 2, -height / 2, lenght / 2);
	writeVertexToXML(triangle, width / 2, -height / 2, -lenght / 2);
	writeVertexToXML(triangle, -width / 2, -height / 2, lenght / 2);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	//face traz
	writeVertexToXML(triangle, -width / 2, -height / 2, -lenght / 2);
	writeVertexToXML(triangle, width / 2, height / 2, -lenght / 2);
	writeVertexToXML(triangle, -width / 2, height / 2, -lenght / 2);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, width / 2, -height / 2, -lenght / 2);
	writeVertexToXML(triangle, width / 2, height / 2, -lenght / 2);
	writeVertexToXML(triangle, -width / 2, -height / 2, -lenght / 2);
	pRoot->InsertEndChild(triangle);


	xmlDoc.SaveFile(filename);
}

void drawPyramidXML(float base, float height, char *filename){
	using namespace tinyxml2;
	Ponto v1, v2, v3;
	XMLNode * pRoot = xmlDoc.NewElement("piramide");
	xmlDoc.InsertFirstChild(pRoot);

	v3.x = 0; v3.y = height; v3.z = 0;

	v1.x = base; v1.y = 0; v1.z = -base;
	v2.x = base; v2.y = 0; v2.z = base;
	writeTriangleToXML(pRoot, v1, v2, v3);

	v1.x = -base; v1.y = 0; v1.z = base;
	v2.x = -base; v2.y = 0; v2.z = -base;
	writeTriangleToXML(pRoot, v1, v2, v3);

	v1.x = base; v1.y = 0; v1.z = base;
	v2.x = -base; v2.y = 0; v2.z = base;
	writeTriangleToXML(pRoot, v1, v2, v3);

	v1.x = -base; v1.y = 0; v1.z = -base;
	v2.x = base; v2.y = 0; v2.z = -base;
	writeTriangleToXML(pRoot, v1, v2, v3);

	v1.x = base; v1.y = 0; v1.z = base;
	v2.x = base; v2.y = 0; v2.z = -base;
	v3.x = -base; v3.y = 0; v3.z = base;
	writeTriangleToXML(pRoot, v1, v2, v3);

	v1.x = -base; v1.y = 0; v1.z = -base;
	v2.x = -base; v2.y = 0; v2.z = base;
	v3.x = base; v3.y = 0; v3.z = -base;
	writeTriangleToXML(pRoot, v1, v2, v3);

	xmlDoc.SaveFile(filename);
}

void drawSphereXML(float r, int stacks, int slices, char *filename){
	using namespace tinyxml2;
	XMLNode * pRoot = xmlDoc.NewElement("esfera");
	xmlDoc.InsertFirstChild(pRoot);
	int t, p;
	for (t = 0; t < stacks; t++){ // stacks -> theta (theta é PI porque é na vertical (180º)
		float theta1 = ((float)(t) / stacks)*M_PI;
		float theta2 = ((float)(t + 1) / stacks)*M_PI;

		for (p = 0; p < slices; p++){ // slices -> phi
			double phi1 = ((float)(p) / slices) * 2 * M_PI; // (phi é 2PI porque é na horizontal, tem que ser à volta da esfera
			double phi2 = ((float)(p + 1) / slices) * 2 * M_PI; // toda, ou seja, 360º = 2PI

			Ponto vertex1;
			vertex1.x = r*sin(theta1)*cos(phi1);
			vertex1.y = r*sin(theta1)*sin(phi1);
			vertex1.z = r*cos(theta1);

			Ponto vertex2;
			vertex2.x = r*sin(theta1)*cos(phi2);
			vertex2.y = r*sin(theta1)*sin(phi2);
			vertex2.z = r*cos(theta1);

			Ponto vertex3;
			vertex3.x = r*sin(theta2)*cos(phi2);
			vertex3.y = r*sin(theta2)*sin(phi2);
			vertex3.z = r*cos(theta2);

			Ponto vertex4;
			vertex4.x = r*sin(theta2)*cos(phi1);
			vertex4.y = r*sin(theta2)*sin(phi1);
			vertex4.z = r*cos(theta2);

			if (t == 0){//camada inicial
				writeTriangleToXML(pRoot, vertex1, vertex3, vertex4);
			}
			else if (t + 1 == stacks){ //camada final 
				writeTriangleToXML(pRoot, vertex3, vertex1, vertex2);
			}
			else{
				writeTriangleToXML(pRoot, vertex1, vertex2, vertex4);
				writeTriangleToXML(pRoot, vertex2, vertex3, vertex4);
			}
			//os dois "if" iniciais servem para desenhar o "inicio" e o "fim" da esfera
			//caso nao estejamos na ultima ou primeira camada, desenharemos os "quads" para formar a esfera (2triangles=1quad)
		}
	}
	xmlDoc.SaveFile(filename);
}

void drawCylinderXML(float height, float radius, int stacks, int slices, char *filename) {
	using namespace tinyxml2;
	XMLNode * pRoot = xmlDoc.NewElement("cilindro");
	xmlDoc.InsertFirstChild(pRoot);

	float pi = 3.1415f, h1, h2 = 0;

	//Ciclo que gera base superior
	for (float i = 0; i < 2 * pi; i += 2 * pi / slices) {
		Ponto vertex1;
		vertex1.x = radius*sin(i);
		vertex1.y = height;
		vertex1.z = radius*cos(i);

		Ponto vertex2;
		vertex2.x = 0;
		vertex2.y = height;
		vertex2.z = 0;

		Ponto vertex3;
		vertex3.x = radius*sin(i + 2 * pi / slices);	//(i + 2 * pi / slices) = angulo do ponto da proxima fatia
		vertex3.y = height;
		vertex3.z = radius*cos(i + 2 * pi / slices);

		writeTriangleToXML(pRoot, vertex1, vertex2, vertex3);
	}

	//Ciclo que gera base inferior
	for (float i = 0; i < 2 * pi; i += 2 * pi / slices) {

		Ponto vertex1;
		vertex1.x = radius*sin(i);
		vertex1.y = 0;
		vertex1.z = radius*cos(i);

		Ponto vertex2;
		vertex2.x = 0;
		vertex2.y = 0;
		vertex2.z = 0;

		Ponto vertex3;
		vertex3.x = radius*sin(i + 2 * pi / slices);
		vertex3.y = 0;
		vertex3.z = radius*cos(i + 2 * pi / slices);

		writeTriangleToXML(pRoot, vertex1, vertex3, vertex2);
	}

	//face curva
	//ciclo que gera as camadas da face 
	for (float l = height / stacks, h1 = 0.0f, h2 = l; h2<height; h1 = h2, h2 = h2 + l) { 
		// l  - altura de cada camada; ciclo termina antes de gerar ultima camada

		//ciclo que gera os triangulos de cada camada
		for (float i = 0; i < 2 * pi; i += 2 * pi / slices) {

			Ponto vertex1;
			vertex1.x = radius*sin(i);
			vertex1.y = h2;
			vertex1.z = radius*cos(i);

			Ponto vertex2;
			vertex2.x = radius*sin(i + 2 * pi / slices);
			vertex2.y = h2;
			vertex2.z = radius*cos(i + 2 * pi / slices);

			Ponto vertex3;
			vertex3.x = radius*sin(i);
			vertex3.y = h1;
			vertex3.z = radius*cos(i);

			Ponto vertex4;
			vertex4.x = radius*sin(i + 2 * pi / slices);
			vertex4.y = h1;
			vertex4.z = radius*cos(i + 2 * pi / slices);

			writeTriangleToXML(pRoot, vertex1, vertex2, vertex3);
			writeTriangleToXML(pRoot, vertex2, vertex4, vertex3);
		}

	}
	//ciclo que gera a ultima camada( solução para problema de arredondamento que por vezes levava a que a ultima camada nao fosse gerada)
	for (float i = 0; i < 2 * pi; i += 2 * pi / slices) {	

		Ponto vertex1;
		vertex1.x = radius*sin(i);
		vertex1.y = height;
		vertex1.z = radius*cos(i);

		Ponto vertex2;
		vertex2.x = radius*sin(i + 2 * pi / slices);
		vertex2.y = height;
		vertex2.z = radius*cos(i + 2 * pi / slices);

		Ponto vertex3;
		vertex3.x = radius*sin(i);
		vertex3.y = height - height / stacks;
		vertex3.z = radius*cos(i);

		Ponto vertex4;
		vertex4.x = radius*sin(i + 2 * pi / slices);
		vertex4.y = height - height / stacks;
		vertex4.z = radius*cos(i + 2 * pi / slices);

		writeTriangleToXML(pRoot, vertex1, vertex2, vertex3);
		writeTriangleToXML(pRoot, vertex2, vertex4, vertex3);
	}



	xmlDoc.SaveFile(filename);
}

void drawConeXML(float height, float radius, int stacks, int slices, char *filename) {
	using namespace tinyxml2;
	XMLNode * pRoot = xmlDoc.NewElement("cone");
	xmlDoc.InsertFirstChild(pRoot);

	float pi = 3.1415f, h1, h2 = 0;

	//Ciclo que gera base inferior
	for (float i = 0; i < 2 * pi; i += 2 * pi / slices) {

		Ponto vertex1;
		vertex1.x = radius*sin(i);
		vertex1.y = 0;
		vertex1.z = radius*cos(i);

		Ponto vertex2;
		vertex2.x = 0;
		vertex2.y = 0;
		vertex2.z = 0;

		Ponto vertex3;
		vertex3.x = radius*sin(i + 2 * pi / slices);
		vertex3.y = 0;
		vertex3.z = radius*cos(i + 2 * pi / slices);

		writeTriangleToXML(pRoot, vertex1, vertex3, vertex2);
	}

	//face curva
	float rAct = radius;	//raio da camada actual
	float r = radius / stacks;	//valor a diminuir ao raio da camada actual em cada camada
	// ciclo que gera as camadas
	for (float l = height / stacks, h1 = 0.0f, h2 = l; h2 < height; h1 = h2, h2 = h2 + l) {
		//ciclo que gera triangulos da camada
		for (float i = 0; i < 2 * pi; i += 2 * pi / slices) {

			Ponto vertex1;
			vertex1.x = (rAct - r)*sin(i);
			vertex1.y = h2;
			vertex1.z = (rAct - r)*cos(i);

			Ponto vertex2;
			vertex2.x = (rAct - r)*sin(i + 2 * pi / slices);
			vertex2.y = h2;
			vertex2.z = (rAct - r)*cos(i + 2 * pi / slices);

			Ponto vertex3;
			vertex3.x = rAct*sin(i);
			vertex3.y = h1;
			vertex3.z = rAct*cos(i);

			Ponto vertex4;
			vertex4.x = rAct*sin(i + 2 * pi / slices);
			vertex4.y = h1;
			vertex4.z = rAct*cos(i + 2 * pi / slices);

			writeTriangleToXML(pRoot, vertex1, vertex2, vertex3);
			writeTriangleToXML(pRoot, vertex2, vertex4, vertex3);
		}
		rAct -= r;

	}

	//ciclo que gera a ultima camada do cone
	for (float i = 0; i < 2 * pi; i += 2 * pi / slices) {

		Ponto vertex1;
		vertex1.x = 0;
		vertex1.y = height;
		vertex1.z = 0;

		Ponto vertex2;
		vertex2.x = r*sin(i);
		vertex2.y = height - height / stacks;
		vertex2.z = r*cos(i);

		Ponto vertex3;
		vertex3.x = r*sin(i + 2 * pi / slices);

		vertex3.y = height - height / stacks;
		vertex3.z = r*cos(i + 2 * pi / slices);

		writeTriangleToXML(pRoot, vertex1, vertex3, vertex2);

	}



	xmlDoc.SaveFile(filename);
}

void drawPlaneXML(float largura, float altura, char *filename){
	using namespace tinyxml2;
	XMLNode * pRoot = xmlDoc.NewElement("cone");
	xmlDoc.InsertEndChild(pRoot);

	Ponto v1, v2, v3, v4;
	
	v1.x = largura / 2;
	v1.y = -altura / 2;
	v1.z = 0;

	v2.x = -largura / 2;
	v2.y = -altura / 2;
	v2.z = 0;

	v3.x = -largura / 2;
	v3.y = altura / 2;
	v3.z = 0;

	v4.x = largura / 2;
	v4.y = altura / 2;
	v4.z = 0;

	
	writeTriangleToXML(pRoot, v2, v3, v1);
	writeTriangleToXML(pRoot, v3, v4, v1);

	xmlDoc.SaveFile(filename);

}

int main(int argc, char **argv){
	int stacks, slices;
	float radius;
	if (argc > 1){
		if (strcmp(argv[1], "esfera") == 0){
			if (argc == 6){
				drawSphereXML((float)atof(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
				printf("Esfera gravada em %s com %Lf de raio, %d camadas e %d fatias.\n", argv[5], (float)atof(argv[2]), atoi(argv[3]), atoi(argv[4]));
			}
			else{
				printf("Erro nos argumentos!\n");
			}
		}
		else if (strcmp(argv[1], "piramide") == 0){
			if (argc == 5){
				drawPyramidXML((float)atof(argv[2]), (float)atof(argv[3]), argv[4]);
				printf("Piramide gravada em %s com %Lf de base e %Lf de altura\n",argv[4], (float)atof(argv[2]), (float)atof(argv[3]));
			}
			else{
				printf("Erro nos argumentos!\n");
			}
		}
		else if (strcmp(argv[1], "cilindro") == 0){
			if (argc == 7){
				drawCylinderXML((float)atof(argv[2]), (float)atof(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
				printf("Cilindro gravado em %s com %Lf de altura, %Lf de raio, %d camadas e %d fatias.\n", argv[6], (float)atof(argv[2]), (float)atof(argv[3]), atoi(argv[4]), atoi(argv[5]));
			}
			else{
				printf("Erro nos argumentos!\n");
			}

		}
		else if (strcmp(argv[1], "paralelepipedo") == 0){
			if (argc == 6){
				drawParallelpipedXML(atof(argv[2]), atof(argv[3]), atof(argv[4]), argv[5]);
				printf("Paralelepipedo gravado em %s com %Lf de largura, %Lf de altura e %Lf de comprimento.\n", argv[5], atof(argv[2]), atof(argv[3]), atof(argv[4]));
			}
			else{
				printf("Erro nos argumentos!\n");
			}
		}
		else if (strcmp(argv[1], "cone") == 0){
			if (argc == 7){
				drawConeXML((float)atof(argv[2]), (float)atof(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
				printf("Cone gravado em %s com %Lf de altura, %Lf de raio, %d camadas e %d fatias.\n", argv[6], (float)atof(argv[2]), (float)atof(argv[3]), atoi(argv[4]), atoi(argv[5]));
			}
			else{
				printf("Erro nos argumentos!\n");
			}
		}
		else if (strcmp(argv[1], "plano") == 0){
			if (argc == 5){
				drawPlaneXML(atof(argv[2]), atof(argv[3]),argv[4]);
				printf("Plano gravado em %s com %Lf de largura e %Lf de altura.\n", argv[4], atof(argv[2]), atof(argv[3]));
			}
			else{
				printf("Erro nos argumentos!\n");
			}
		}
		else{
			printf("Figura invalida!");
		}
	}
}
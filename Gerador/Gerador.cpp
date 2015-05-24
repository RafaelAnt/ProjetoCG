#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>
#include "tinyxml2.h"
#include <sstream>
#include <vector>
#include <fstream>
#include "Bezier.h"

tinyxml2::XMLDocument xmlDoc;

static void writeVertexToXML(tinyxml2::XMLNode * pRoot, Point v){
	using namespace tinyxml2;
	char text[1024];
	XMLElement *elem = xmlDoc.NewElement("vertex");
	sprintf_s(text, "X=%f Y=%f Z=%f", v.x, v.y, v.z);
	elem->SetText(text);
	pRoot->InsertEndChild(elem);
}

static void writeVertexToXML(tinyxml2::XMLNode * pRoot, float x, float y, float z){
	using namespace tinyxml2;
	char text[1024];
	XMLElement *elem = xmlDoc.NewElement("vertex");
	sprintf_s(text, "X=%f Y=%f Z=%f", x, y, z);
	elem->SetText(text);
	pRoot->InsertEndChild(elem);
}

static void writeNormalToXML(tinyxml2::XMLNode * pRoot, Point v){
	using namespace tinyxml2;
	char text[1024];
	XMLElement *elem = xmlDoc.NewElement("normal");
	sprintf_s(text, "X=%f Y=%f Z=%f", v.x, v.y, v.z);
	elem->SetText(text);
	pRoot->InsertEndChild(elem);
}

static void writeNormalToXML(tinyxml2::XMLNode * pRoot, float x, float y, float z){
	using namespace tinyxml2;
	char text[1024];
	XMLElement *elem = xmlDoc.NewElement("normal");
	sprintf_s(text, "X=%f Y=%f Z=%f", x, y, z);
	elem->SetText(text);
	pRoot->InsertEndChild(elem);
}

static void writeTexCoordToXML(tinyxml2::XMLNode * pRoot, Point v){
	using namespace tinyxml2;
	char text[1024];
	XMLElement *elem = xmlDoc.NewElement("texcoord");
	sprintf_s(text, "U=%f V=%f", v.x, v.y, v.z);
	elem->SetText(text);
	pRoot->InsertEndChild(elem);
}

static void writeTexCoordToXML(tinyxml2::XMLNode * pRoot, float u, float v){
	using namespace tinyxml2;
	char text[1024];
	XMLElement *elem = xmlDoc.NewElement("texcoord");
	sprintf_s(text, "U=%f V=%f", u, v);
	elem->SetText(text);
	pRoot->InsertEndChild(elem);
}

static void writeTriangleToXML(tinyxml2::XMLNode * pRoot, Point v1, Point v2, Point v3){
	using namespace tinyxml2;
	XMLNode *triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, v1);
	writeVertexToXML(triangle, v2);
	writeVertexToXML(triangle, v3);
	pRoot->InsertEndChild(triangle);
}
static void writeTriangleToXML(tinyxml2::XMLNode * pRoot, Point v1, Point v2, Point v3, Point n1, Point n2, Point n3, Point t1, Point t2, Point t3){
	using namespace tinyxml2;
	XMLNode *triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, v1);
	writeNormalToXML(triangle, n1);
	writeTexCoordToXML(triangle, t1);
	writeVertexToXML(triangle, v2);
	writeNormalToXML(triangle, n2);
	writeTexCoordToXML(triangle, t2);
	writeVertexToXML(triangle, v3);
	writeNormalToXML(triangle, n3);
	writeTexCoordToXML(triangle, t3);
	pRoot->InsertEndChild(triangle);
}

void drawParallelpipedXML(float width, float height, float lenght, char* filename){
	using namespace tinyxml2;
	XMLNode * pRoot = xmlDoc.NewElement("paralelipipedo");
	xmlDoc.InsertFirstChild(pRoot);

	XMLNode *triangle = xmlDoc.NewElement("triangle");
	//face frente
	writeVertexToXML(triangle, width / 2, height / 2, lenght / 2);
	writeNormalToXML(triangle, 0.0f, 0.0f, 1.0f);
	writeTexCoordToXML(triangle, 1, 1);
	writeVertexToXML(triangle, width / 2, -height / 2, lenght / 2);
	writeNormalToXML(triangle, 0.0f, 0.0f, 1.0f);
	writeTexCoordToXML(triangle, 1, 0);
	writeVertexToXML(triangle, -width / 2, height / 2, lenght / 2);
	writeNormalToXML(triangle, 0.0f, 0.0f, 1.0f);
	writeTexCoordToXML(triangle, 0, 1);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, -width / 2, height / 2, lenght / 2);
	writeNormalToXML(triangle, 0.0f, 0.0f, 1.0f);
	writeTexCoordToXML(triangle, 0, 1);
	writeVertexToXML(triangle, width / 2, -height / 2, lenght / 2);
	writeNormalToXML(triangle, 0.0f, 0.0f, 1.0f);
	writeTexCoordToXML(triangle, 1, 0);
	writeVertexToXML(triangle, -width / 2, -height / 2, lenght / 2);
	writeNormalToXML(triangle, 0.0f, 0.0f, 1.0f);
	writeTexCoordToXML(triangle, 0, 0);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	//face direita
	writeVertexToXML(triangle, width / 2, height / 2, lenght / 2);
	writeNormalToXML(triangle, 1.0f, 0.0f, 0.0f);
	writeTexCoordToXML(triangle, 0, 1);
	writeVertexToXML(triangle, width / 2, height / 2, -lenght / 2);
	writeNormalToXML(triangle, 1.0f, 0.0f, 0.0f);
	writeTexCoordToXML(triangle, 1, 1);
	writeVertexToXML(triangle, width / 2, -height / 2, -lenght / 2);
	writeNormalToXML(triangle, 1.0f, 0.0f, 0.0f);
	writeTexCoordToXML(triangle, 1, 0);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, width / 2, height / 2, lenght / 2);
	writeNormalToXML(triangle, 1.0f, 0.0f, 0.0f);
	writeTexCoordToXML(triangle, 0, 1);
	writeVertexToXML(triangle, width / 2, -height / 2, -lenght / 2);
	writeNormalToXML(triangle, 1.0f, 0.0f, 0.0f);
	writeTexCoordToXML(triangle, 1, 0);
	writeVertexToXML(triangle, width / 2, -height / 2, lenght / 2);
	writeNormalToXML(triangle, 1.0f, 0.0f, 0.0f);
	writeTexCoordToXML(triangle, 0, 0);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	//face  cima
	writeVertexToXML(triangle, width / 2, height / 2, lenght / 2);
	writeNormalToXML(triangle, 0.0f, 1.0f, 0.0f);
	writeTexCoordToXML(triangle, 1, 0);
	writeVertexToXML(triangle, -width / 2, height / 2, lenght / 2);
	writeNormalToXML(triangle, 0.0f, 1.0f, 0.0f);
	writeTexCoordToXML(triangle, 0, 0);
	writeVertexToXML(triangle, width / 2, height / 2, -lenght / 2);
	writeNormalToXML(triangle, 0.0f, 1.0f, 0.0f);
	writeTexCoordToXML(triangle, 1, 1);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, -width / 2, height / 2, lenght / 2);
	writeNormalToXML(triangle, 0.0f, 1.0f, 0.0f);
	writeTexCoordToXML(triangle, 0, 0);
	writeVertexToXML(triangle, -width / 2, height / 2, -lenght / 2);
	writeNormalToXML(triangle, 0.0f, 1.0f, 0.0f);
	writeTexCoordToXML(triangle, 0, 1);
	writeVertexToXML(triangle, width / 2, height / 2, -lenght / 2);
	writeNormalToXML(triangle, 0.0f, 1.0f, 0.0f);
	writeTexCoordToXML(triangle, 1, 1);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	//face esquerda
	writeVertexToXML(triangle, -width / 2, height / 2, lenght / 2);
	writeNormalToXML(triangle, -1.0f, 0.0f, 0.0f);
	writeTexCoordToXML(triangle, 1, 1);
	writeVertexToXML(triangle, -width / 2, -height / 2, -lenght / 2);
	writeNormalToXML(triangle, -1.0f, 0.0f, 0.0f);
	writeTexCoordToXML(triangle, 0, 0);
	writeVertexToXML(triangle, -width / 2, height / 2, -lenght / 2);
	writeNormalToXML(triangle, -1.0f, 0.0f, 0.0f);
	writeTexCoordToXML(triangle, 0, 1);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, -width / 2, height / 2, lenght / 2);
	writeNormalToXML(triangle, -1.0f, 0.0f, 0.0f);
	writeTexCoordToXML(triangle, 1, 1);
	writeVertexToXML(triangle, -width / 2, -height / 2, lenght / 2);
	writeNormalToXML(triangle, -1.0f, 0.0f, 0.0f);
	writeTexCoordToXML(triangle, 1, 0);
	writeVertexToXML(triangle, -width / 2, -height / 2, -lenght / 2);
	writeNormalToXML(triangle, -1.0f, 0.0f, 0.0f);
	writeTexCoordToXML(triangle, 0, 0);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	//face baixo
	writeVertexToXML(triangle, -width / 2, -height / 2, lenght / 2);
	writeNormalToXML(triangle, 0.0f, -1.0f, 0.0f);
	writeTexCoordToXML(triangle, 0, 1);
	writeVertexToXML(triangle, width / 2, -height / 2, -lenght / 2);
	writeNormalToXML(triangle, 0.0f, -1.0f, 0.0f);
	writeTexCoordToXML(triangle, 1, 0);
	writeVertexToXML(triangle, -width / 2, -height / 2, -lenght / 2);
	writeNormalToXML(triangle, 0.0f, -1.0f, 0.0f);
	writeTexCoordToXML(triangle, 0, 0);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, width / 2, -height / 2, lenght / 2);
	writeNormalToXML(triangle, 0.0f, -1.0f, 0.0f);
	writeTexCoordToXML(triangle, 1, 1);
	writeVertexToXML(triangle, width / 2, -height / 2, -lenght / 2);
	writeNormalToXML(triangle, 0.0f, -1.0f, 0.0f);
	writeTexCoordToXML(triangle, 1, 0);
	writeVertexToXML(triangle, -width / 2, -height / 2, lenght / 2);
	writeNormalToXML(triangle, 0.0f, -1.0f, 0.0f);
	writeTexCoordToXML(triangle, 0, 1);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	//face traz
	writeVertexToXML(triangle, -width / 2, -height / 2, -lenght / 2);
	writeNormalToXML(triangle, 0.0f, 0.0f, -1.0f);
	writeTexCoordToXML(triangle, 1, 0);
	writeVertexToXML(triangle, width / 2, height / 2, -lenght / 2);
	writeNormalToXML(triangle, 0.0f, 0.0f, -1.0f);
	writeTexCoordToXML(triangle, 0, 1);
	writeVertexToXML(triangle, -width / 2, height / 2, -lenght / 2);
	writeNormalToXML(triangle, 0.0f, 0.0f, -1.0f);
	writeTexCoordToXML(triangle, 1, 1);
	pRoot->InsertEndChild(triangle);

	triangle = xmlDoc.NewElement("triangle");
	writeVertexToXML(triangle, width / 2, -height / 2, -lenght / 2);
	writeNormalToXML(triangle, 0.0f, 0.0f, -1.0f);
	writeTexCoordToXML(triangle, 0, 0);
	writeVertexToXML(triangle, width / 2, height / 2, -lenght / 2);
	writeNormalToXML(triangle, 0.0f, 0.0f, -1.0f);
	writeTexCoordToXML(triangle, 0, 1);
	writeVertexToXML(triangle, -width / 2, -height / 2, -lenght / 2);
	writeNormalToXML(triangle, 0.0f, 0.0f, -1.0f);
	writeTexCoordToXML(triangle, 1, 0);
	pRoot->InsertEndChild(triangle);


	xmlDoc.SaveFile(filename);
}

Point calculateSurfaceNormal(Point p1, Point p2, Point p3) {
	Point vectorU, vectorV, normal;
	float a;

	vectorU.x = p2.x - p1.x;
	vectorU.y = p2.y - p1.y;
	vectorU.z = p2.z - p1.z;
	vectorV.x = p3.x - p1.x;
	vectorV.y = p3.y - p1.y;
	vectorV.z = p3.z - p1.z;

	normal.x = ((vectorU.y * vectorV.z) - (vectorU.z * vectorV.y));
	normal.y = (vectorU.z * vectorV.x) - (vectorU.x * vectorV.z);
	normal.z = (vectorU.x * vectorV.y) - (vectorU.y * vectorV.x);

	a = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);

	normal.x = normal.x / a;
	normal.y = normal.y / a;
	normal.z = normal.z / a;

	return normal;

}

Point invertNormal(Point p1) {
	

	p1.x = -p1.x;
	p1.y = -p1.y;
	p1.z = -p1.z;

	return p1;

}

void drawPyramidXML(float base, float height, char *filename){
	using namespace tinyxml2;
	Point v1, v2, v3, normal, t1, t2, t3;
	XMLNode * pRoot = xmlDoc.NewElement("piramide");
	xmlDoc.InsertFirstChild(pRoot);

	v3.x = 0; v3.y = height; v3.z = 0;

	v1.x = base; v1.y = 0; v1.z = -base;
	v2.x = base; v2.y = 0; v2.z = base;
	normal = calculateSurfaceNormal(v1, v2, v3);
	normal = invertNormal(normal);
	t1.x = 1.0f; t1.y = 0.0f;
	t2.x = 0.0f; t2.y = 0.0f;
	t3.x = 0.5f; t3.y = 1.0f;	
	writeTriangleToXML(pRoot, v1, v2, v3, normal, normal, normal, t1, t2, t3);


	v1.x = -base; v1.y = 0; v1.z = base;
	v2.x = -base; v2.y = 0; v2.z = -base;
	normal = calculateSurfaceNormal(v1, v2, v3);
	normal = invertNormal(normal);
	t1.x = 1.0f; t1.y = 0.0f;
	t2.x = 0.0f; t2.y = 0.0f;
	t3.x = 0.5f; t3.y = 1.0f;
	writeTriangleToXML(pRoot, v1, v2, v3, normal, normal, normal, t1, t2, t3);

	v1.x = base; v1.y = 0; v1.z = base;
	v2.x = -base; v2.y = 0; v2.z = base;
	normal = calculateSurfaceNormal(v1, v2, v3);
	normal = invertNormal(normal);
	t1.x = 1.0f; t1.y = 0.0f;
	t2.x = 0.0f; t2.y = 0.0f;
	t3.x = 0.5f; t3.y = 1.0f;
	writeTriangleToXML(pRoot, v1, v2, v3, normal, normal, normal, t1, t2, t3);

	v1.x = -base; v1.y = 0; v1.z = -base;
	v2.x = base; v2.y = 0; v2.z = -base;
	normal = calculateSurfaceNormal(v1, v2, v3);
	normal = invertNormal(normal);
	t1.x = 1.0f; t1.y = 0.0f;
	t2.x = 0.0f; t2.y = 0.0f;
	t3.x = 0.5f; t3.y = 1.0f;
	writeTriangleToXML(pRoot, v1, v2, v3, normal, normal, normal, t1, t2, t3);

	v1.x = base; v1.y = 0; v1.z = base;
	v2.x = base; v2.y = 0; v2.z = -base;
	v3.x = -base; v3.y = 0; v3.z = base;
	normal = calculateSurfaceNormal(v1, v2, v3);
	normal = invertNormal(normal);
	t1.x = 1.0f; t1.y = 0.0f;
	t2.x = 0.0f; t2.y = 0.0f;
	t3.x = 0.5f; t3.y = 1.0f;
	writeTriangleToXML(pRoot, v1, v2, v3, normal, normal, normal, t1, t2, t3);

	v1.x = -base; v1.y = 0; v1.z = -base;
	v2.x = -base; v2.y = 0; v2.z = base;
	v3.x = base; v3.y = 0; v3.z = -base;
	normal = calculateSurfaceNormal(v1, v2, v3);
	normal = invertNormal(normal);
	t1.x = 1.0f; t1.y = 0.0f;
	t2.x = 0.0f; t2.y = 0.0f;
	t3.x = 0.5f; t3.y = 1.0f;
	writeTriangleToXML(pRoot, v1, v2, v3, normal, normal, normal, t1, t2, t3);

	xmlDoc.SaveFile(filename);
}

void drawSphereXML(float r, int stacks, int slices, char *filename){
	using namespace tinyxml2;
	XMLNode * pRoot = xmlDoc.NewElement("esfera");
	xmlDoc.InsertFirstChild(pRoot);
	int t, p;
	for (t = 0; t < stacks; t++){ // stacks -> theta (theta é PI porque é na vertical (180º)
		float theta1 = ((float)(t) / stacks)*(float)M_PI;
		float theta2 = ((float)(t + 1) / stacks)*(float)M_PI;

		for (p = 0; p < slices; p++){ // slices -> phi
			float phi1 = ((float)(p) / slices) * 2 * (float)M_PI; // (phi é 2PI porque é na horizontal, tem que ser à volta da esfera
			float phi2 = ((float)(p + 1) / slices) * 2 * (float)M_PI; // toda, ou seja, 360º = 2PI

			Point vertex1;
			vertex1.x = (float)r*sin(theta1)*cos(phi1);
			vertex1.y = (float)r*sin(theta1)*sin(phi1);
			vertex1.z = (float)r*cos(theta1);

			Point vertex2;
			vertex2.x = (float)r*sin(theta1)*cos(phi2);
			vertex2.y = (float)r*sin(theta1)*sin(phi2);
			vertex2.z = (float)r*cos(theta1);

			Point vertex3;
			vertex3.x = (float)r*sin(theta2)*cos(phi2);
			vertex3.y = (float)r*sin(theta2)*sin(phi2);
			vertex3.z = (float)r*cos(theta2);

			Point vertex4;
			vertex4.x = (float)r*sin(theta2)*cos(phi1);
			vertex4.y = (float)r*sin(theta2)*sin(phi1);
			vertex4.z = (float)r*cos(theta2);

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

	float pi = 3.1415f;

	//Ciclo que gera base superior
	for (float i = 0; i < 2 * pi; i += 2 * pi / slices) {
		Point vertex1;
		vertex1.x = radius*sin(i);
		vertex1.y = height;
		vertex1.z = radius*cos(i);

		Point vertex2;
		vertex2.x = 0;
		vertex2.y = height;
		vertex2.z = 0;

		Point vertex3;
		vertex3.x = radius*sin(i + 2 * pi / slices);	//(i + 2 * pi / slices) = angulo do ponto da proxima fatia
		vertex3.y = height;
		vertex3.z = radius*cos(i + 2 * pi / slices);

		writeTriangleToXML(pRoot, vertex1, vertex2, vertex3);
	}

	//Ciclo que gera base inferior
	for (float i = 0; i < 2 * pi; i += 2 * pi / slices) {

		Point vertex1;
		vertex1.x = radius*sin(i);
		vertex1.y = 0;
		vertex1.z = radius*cos(i);

		Point vertex2;
		vertex2.x = 0;
		vertex2.y = 0;
		vertex2.z = 0;

		Point vertex3;
		vertex3.x = radius*sin(i + 2 * pi / slices);
		vertex3.y = 0;
		vertex3.z = radius*cos(i + 2 * pi / slices);

		writeTriangleToXML(pRoot, vertex1, vertex3, vertex2);
	}

	//face curva
	//ciclo que gera as camadas da face 
	for (float l = height / stacks, h1 = 0.0f, h2 = l; h2 < height; h1 = h2, h2 = h2 + l) {
		// l  - altura de cada camada; ciclo termina antes de gerar ultima camada

		//ciclo que gera os triangulos de cada camada
		for (float i = 0; i < 2 * pi; i += 2 * pi / slices) {

			Point vertex1;
			vertex1.x = radius*sin(i);
			vertex1.y = h2;
			vertex1.z = radius*cos(i);

			Point vertex2;
			vertex2.x = radius*sin(i + 2 * pi / slices);
			vertex2.y = h2;
			vertex2.z = radius*cos(i + 2 * pi / slices);

			Point vertex3;
			vertex3.x = radius*sin(i);
			vertex3.y = h1;
			vertex3.z = radius*cos(i);

			Point vertex4;
			vertex4.x = radius*sin(i + 2 * pi / slices);
			vertex4.y = h1;
			vertex4.z = radius*cos(i + 2 * pi / slices);

			writeTriangleToXML(pRoot, vertex1, vertex2, vertex3);
			writeTriangleToXML(pRoot, vertex2, vertex4, vertex3);
		}

	}
	//ciclo que gera a ultima camada( solução para problema de arredondamento que por vezes levava a que a ultima camada nao fosse gerada)
	for (float i = 0; i < 2 * pi; i += 2 * pi / slices) {

		Point vertex1;
		vertex1.x = radius*sin(i);
		vertex1.y = height;
		vertex1.z = radius*cos(i);

		Point vertex2;
		vertex2.x = radius*sin(i + 2 * pi / slices);
		vertex2.y = height;
		vertex2.z = radius*cos(i + 2 * pi / slices);

		Point vertex3;
		vertex3.x = radius*sin(i);
		vertex3.y = height - height / stacks;
		vertex3.z = radius*cos(i);

		Point vertex4;
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

	float pi = 3.1415f;

	//Ciclo que gera base inferior
	for (float i = 0; i < 2 * pi; i += 2 * pi / slices) {

		Point vertex1;
		vertex1.x = radius*sin(i);
		vertex1.y = 0;
		vertex1.z = radius*cos(i);

		Point vertex2;
		vertex2.x = 0;
		vertex2.y = 0;
		vertex2.z = 0;

		Point vertex3;
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

			Point vertex1;
			vertex1.x = (rAct - r)*sin(i);
			vertex1.y = h2;
			vertex1.z = (rAct - r)*cos(i);

			Point vertex2;
			vertex2.x = (rAct - r)*sin(i + 2 * pi / slices);
			vertex2.y = h2;
			vertex2.z = (rAct - r)*cos(i + 2 * pi / slices);

			Point vertex3;
			vertex3.x = rAct*sin(i);
			vertex3.y = h1;
			vertex3.z = rAct*cos(i);

			Point vertex4;
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

		Point vertex1;
		vertex1.x = 0;
		vertex1.y = height;
		vertex1.z = 0;

		Point vertex2;
		vertex2.x = r*sin(i);
		vertex2.y = height - height / stacks;
		vertex2.z = r*cos(i);

		Point vertex3;
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

	Point v1, v2, v3, v4;

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

void drawBezierPatchesXML(vector<Point> vertices, vector<vector<unsigned int>> indices, unsigned int resu, unsigned int resv, char *filename){
	vector<Patch> vertices_res;
	Point pontos_control[NM + 1][NM + 1];
	for (unsigned int p = 0; p < indices.size(); p++) {
		//obter pontos de controlo para este patch
		makeControlPoints(p, pontos_control, vertices, indices);
		/*matriz com os pontos ao longo de u e v
		esta matriz é uma grelha com todos os pontos
		da superficie de bezier com resu colunas e resvv linhas*/
		Patch patch(resu);
		//superficies de bezier contem curvas de bezier ao longo de
		//u e v, teremos que as percorrer e obter todos os pontos resultantes
		for (unsigned int ru = 0; ru <= resu - 1; ru++) {
			//colocar u entre 0 e 1
			float u = ru / (float)(resu - 1);
			for (unsigned int rv = 0; rv <= resv - 1; rv++) {
				//colocar v entre 0 e 1
				float v = rv / (float)(resv - 1);
				//adicionar ponto no fim desta linha, posição (ru,rv)
				patch[ru].push_back(getBezierPoint(pontos_control, u, v));
			}
		}
		vertices_res.push_back(patch);
	}

	//guardar no xml
	using namespace tinyxml2;
	XMLNode * pRoot = xmlDoc.NewElement("beziersurface");
	xmlDoc.InsertEndChild(pRoot);
	Point a, b, c, d;
	for (unsigned int p = 0; p < indices.size(); p++) {
		for (unsigned int ru = 0; ru < resu - 1; ru++){
			for (unsigned int rv = 0; rv < resv - 1; rv++) {
				// 1 quadrado ABCD = 2 triangles CBA + ADC
				a = vertices_res[p][ru][rv];
				b = vertices_res[p][ru][rv + 1];
				c = vertices_res[p][ru + 1][rv + 1];
				d = vertices_res[p][ru + 1][rv];
				// CBA
				writeTriangleToXML(pRoot, c, b, a);
				// ADC
				writeTriangleToXML(pRoot, a, d, c);
			}
		}
	}
	xmlDoc.SaveFile(filename);
}

bool readBezierFile(string filename, vector<vector<unsigned int>> &indices, vector<Point> &vertices){
	int i = 0, j;
	string line;
	string token;
	float nindices, npontos;
	float aux[3];
	ifstream file(filename);
	if (file.is_open()){
		//saber numero de indices
		if (i == 0) {
			file >> nindices;
			i++;
		}
		//buscar os indices e guardar no vetor de vetores
		for (i = 1; i <= nindices + 1; i++) {
			vector<unsigned int> v;
			getline(file, line);
			stringstream ss(line);
			while (getline(ss, token, ',')) {
				//indices começam a 1 em vez de 0
				v.push_back(atoi(token.c_str()) + 1);
			}
			if (i != 1) indices.push_back(v);
		}
		//saber numero de pontos
		file >> npontos;
		i = 1;
		//buscar os pontos e guardar no vetor de pontos
		while (!file.eof()){
			ponto f;
			getline(file, line);
			stringstream ss(line);
			j = 0;
			while (getline(ss, token, ',')) {
				aux[j] = (float)atof(token.c_str());
				j++;
			}
			f.x = aux[0];
			f.y = aux[1];
			f.z = aux[2];
			if (i != 1) vertices.push_back(f);
			i++;
		}
		file.close();
		return true;
	}
	else{
		puts("Ficheiro não existe!");
		return false;
	}
}

int main(int argc, char **argv){
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
				printf("Piramide gravada em %s com %Lf de base e %Lf de altura\n", argv[4], (float)atof(argv[2]), (float)atof(argv[3]));
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
				drawParallelpipedXML((float)atof(argv[2]), (float)atof(argv[3]), (float)atof(argv[4]), argv[5]);
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
				drawPlaneXML((float)atof(argv[2]), (float)atof(argv[3]), argv[4]);
				printf("Plano gravado em %s com %Lf de largura e %Lf de altura.\n", argv[4], atof(argv[2]), atof(argv[3]));
			}
			else{
				printf("Erro nos argumentos!\n");
			}
		}
		else if (strcmp(argv[1], "bezier") == 0){
			if (argc == 6){
				vector<vector<unsigned int>> indices;
				vector<Point> vertices;
				if (readBezierFile(argv[2], indices, vertices)){
					int resu = atoi(argv[3]);
					int resv = atoi(argv[4]);
					if (resu < 0 || resv < 0)
						puts("Resoluções inválidas! Número inteiros positivos apenas!");
					drawBezierPatchesXML(vertices, indices, resu, resv, argv[5]);
					printf("Modelo gravado em %s", argv[5]);
				}
				else
					puts("Ficheiro não existe!");
			}
			else{
				puts("Erro nos argumentos!");
			}
		}
		else{
			printf("Figura invalida!");
		}
	}
}
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include "tinyxml2.h"
#include <sstream>
#include "Modelador.h"

typedef struct vertex{
	double x;
	double y;
	double z;
}Vertex;

tinyxml2::XMLDocument xmlDoc;

static void writeVertexToXML(tinyxml2::XMLNode * pRoot, Vertex v){
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

void drawCubeXML(double size){
	int i;
	using namespace tinyxml2;
	XMLNode *pRoot = xmlDoc.NewElement("cubo");
	xmlDoc.InsertFirstChild(pRoot);
	XMLElement *mode = xmlDoc.NewElement("mode");
	mode->SetText("QUAD");
	pRoot->InsertEndChild(mode);

	// Top face (y = size)
	writeVertexToXML(pRoot, size, size, -size);
	writeVertexToXML(pRoot, -size, size, -size);
	writeVertexToXML(pRoot, -size, size, size);
	writeVertexToXML(pRoot, size, size, size);

	// Bottom face (y = -size)
	writeVertexToXML(pRoot, size, -size, size);
	writeVertexToXML(pRoot, -size, -size, size);
	writeVertexToXML(pRoot, -size, -size, -size);
	writeVertexToXML(pRoot, size, -size, -size);

	// Front face  (z = size)
	writeVertexToXML(pRoot, size, size, size);
	writeVertexToXML(pRoot, -size, size, size);
	writeVertexToXML(pRoot, -size, -size, size);
	writeVertexToXML(pRoot, size, -size, size);

	// Back face (z = -size)
	writeVertexToXML(pRoot, size, -size, -size);
	writeVertexToXML(pRoot, -size, -size, -size);
	writeVertexToXML(pRoot, -size, size, -size);
	writeVertexToXML(pRoot, size, size, -size);

	// Left face (x = -size)
	writeVertexToXML(pRoot, -size, size, size);
	writeVertexToXML(pRoot, -size, size, -size);
	writeVertexToXML(pRoot, -size, -size, -size);
	writeVertexToXML(pRoot, -size, -size, size);

	// Right face (x = size)
	writeVertexToXML(pRoot, size, size, -size);
	writeVertexToXML(pRoot, size, size, size);
	writeVertexToXML(pRoot, size, -size, size);
	writeVertexToXML(pRoot, size, -size, -size);
	

	xmlDoc.SaveFile("cubo.3d");
}

void drawSphereXML(double r, int stacks, int slices){
	using namespace tinyxml2;
	XMLNode * pRoot = xmlDoc.NewElement("esfera");
	xmlDoc.InsertFirstChild(pRoot);
	double theta1, theta2, phi1, phi2;
	int t, p;
	for ( t= 0; t < stacks; t++){ // stacks are ELEVATION so they count theta
		double theta1 = ((double)(t) / stacks)*M_PI;
		double theta2 = ((double)(t + 1) / stacks)*M_PI;

		for (p = 0; p < slices; p++){ // slices are ORANGE SLICES so they count azimuth
			double phi1 = ((double)(p) / slices) * 2 * M_PI; // azimuth goes around 0 .. 2*PI
			double phi2 = ((double)(p + 1) / slices) * 2 * M_PI;

			Vertex vertex1;
			vertex1.x = r*sin(theta1)*cos(phi1);
			vertex1.y = r*sin(theta1)*sin(phi1);
			vertex1.z = r*cos(theta1);

			Vertex vertex2;
			vertex2.x = r*sin(theta1)*cos(phi2);
			vertex2.y = r*sin(theta1)*sin(phi2);
			vertex2.z = r*cos(theta1);

			Vertex vertex3;
			vertex3.x = r*sin(theta2)*cos(phi2);
			vertex3.y = r*sin(theta2)*sin(phi2);
			vertex3.z = r*cos(theta2);

			Vertex vertex4;
			vertex4.x = r*sin(theta2)*cos(phi1);
			vertex4.y = r*sin(theta2)*sin(phi1);
			vertex4.z = r*cos(theta2);

			// facing out
			if (t == 0){ // top cap
				writeVertexToXML(pRoot, vertex1);
				writeVertexToXML(pRoot, vertex3);
				writeVertexToXML(pRoot, vertex4);
			}
			else if (t + 1 == stacks){ //end cap
				writeVertexToXML(pRoot, vertex3);
				writeVertexToXML(pRoot, vertex1);
				writeVertexToXML(pRoot, vertex2);
			}
			else{
				writeVertexToXML(pRoot, vertex1);
				writeVertexToXML(pRoot, vertex2);
				writeVertexToXML(pRoot, vertex4);
				writeVertexToXML(pRoot, vertex2);
				writeVertexToXML(pRoot, vertex3);
				writeVertexToXML(pRoot, vertex4);
			}
		}
	}
	xmlDoc.SaveFile("esfera.3d");
}



/*
void drawSphereXML2(double r, int lats, int longs) {
	using namespace tinyxml2;
	tinyxml2::XMLDocument xmlDoc;
	XMLNode * pRoot = xmlDoc.NewElement("esfera");
	xmlDoc.InsertFirstChild(pRoot);
	char aux[1024];
	std::string elemText;
	int i, j, total = 1;
	for (i = 0; i <= lats; i++) {

		double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = M_PI * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		for (j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			sprintf_s(aux, "X=%g Y=%g Z=%g", x * zr0, y * zr0, z0);
			XMLElement *elem = xmlDoc.NewElement("vertex");
			elem->SetText(aux);
			pRoot->InsertEndChild(elem);
			sprintf_s(aux, "X=%g Y=%g Z=%g", x * zr1, y * zr1, z1);
			elem = xmlDoc.NewElement("vertex");
			elem->SetText(aux);
			pRoot->InsertEndChild(elem);
		}

	}
	xmlDoc.SaveFile("esfera.3d");
}*/
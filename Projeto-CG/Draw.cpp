#include <valarray>
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

typedef struct triangle{
	Point p1;
	Point p2;
	Point p3;
}Triangle;

static void normal(Triangle triangle){
	Point u, v, normal;

	u.x = triangle.p2.x - triangle.p1.x;
	u.y = triangle.p2.y - triangle.p1.y;
	u.z = triangle.p2.z - triangle.p1.z;

	v.x = triangle.p3.x - triangle.p1.x;
	v.y = triangle.p3.y - triangle.p1.y;
	v.z = triangle.p3.z - triangle.p1.z;

	normal.x = (u.y*v.z) - (u.x*v.y);
	normal.y = (u.z*v.x) - (u.x*v.z);
	normal.z = (u.x*v.y) - (u.y*v.x);

	glNormal3f(normal.x, normal.y, normal.z);
}


void drawVertices(vector<GLfloat> vertices){
	Triangle t;
	Point p1, p2, p3;
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0, j = 0; i < vertices.size(); i += 3, j++){
		p1.x = vertices[i]; p1.y = vertices[i + 1]; p1.z = vertices[i + 2];
		glNormal3f(p1.x, p1.y, p1.z);
		glVertex3f(p1.x, p1.y, p1.z);
		i += 3;
		p2.x = vertices[i]; p2.y = vertices[i + 1]; p2.z = vertices[i + 2];
		glNormal3f(p2.x, p2.y, p2.z);
		glVertex3f(p2.x, p2.y, p2.z);
		i += 3;
		p3.x = vertices[i]; p3.y = vertices[i + 1]; p3.z = vertices[i + 2];
		glNormal3f(p3.x, p3.y, p3.z);
		glVertex3f(p3.x, p3.y, p3.z);
		t.p1 = p1; t.p2 = p2; t.p3 = p3;
	}
	glEnd();
}

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

vector<GLfloat> readVertices(const char *filename) {
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
		vec = readVertices_aux(pElement->FirstChildElement("vertex"),vec);
		pElement = pElement->NextSiblingElement("triangle");
	}
	return vec;
}
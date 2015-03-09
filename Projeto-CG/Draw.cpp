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
}Triangle ;

static void normal(Triangle triangle){
	Point u, v, normal;
	
	u.x = triangle.p2.x - triangle.p1.x;
	u.y = triangle.p2.y - triangle.p1.y;
	u.z = triangle.p2.z - triangle.p1.z;

	v.x = triangle.p3.x - triangle.p1.x;
	v.y = triangle.p3.y - triangle.p1.y;
	v.z = triangle.p3.z - triangle.p1.z;

	normal.x = u.y*v.z - u.x*v.y;
	normal.y = u.z*v.x - u.x*v.z;
	normal.z = u.x*v.y - u.y*v.x;

	glNormal3f(normal.x, normal.y, normal.z);

}

void drawVertices(valarray<GLfloat> vertices){
	Triangle t;
	Point p1, p2, p3;
	for (unsigned int i = 0, j = 0; i < vertices.size(); i += 3, j++){
		glBegin(GL_TRIANGLES);
		p1.x = vertices[i]; p1.y = vertices[i + 1]; p1.z = vertices[i + 2];
		glNormal3f(vertices[i], vertices[i + 1], vertices[i + 2]);
		glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
		i += 3;
		p2.x = vertices[i]; p2.y = vertices[i + 1]; p2.z = vertices[i + 2];
		glNormal3f(vertices[i], vertices[i + 1], vertices[i + 2]);
		glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
		i += 3;
		p3.x = vertices[i]; p3.y = vertices[i + 1]; p3.z = vertices[i + 2];
		glNormal3f(vertices[i], vertices[i + 1], vertices[i + 2]);
		glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
		glEnd();
		t.p1 = p1; t.p2 = p2; t.p3 = p3;
		normal(t);
	}
}

static vector<GLfloat> readVertices_aux(tinyxml2::XMLElement *pElement){
	float x, y, z;
	const char *aux;
	vector<GLfloat> vertices;
	while (pElement != NULL) {
		aux = pElement->GetText();
		sscanf_s(aux, "X=%f Y=%f Z=%f", &x, &y, &z);
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		pElement = pElement->NextSiblingElement("vertex");
	}
	return vertices;
}

valarray<GLfloat> readVertices(const char *filename) {
	using namespace tinyxml2;
	tinyxml2::XMLDocument xmlDoc;
	XMLError eResult = xmlDoc.LoadFile(filename);
	XMLNode * pRoot = xmlDoc.FirstChild();
	if (pRoot == nullptr){
		exit(-1);
	}
	vector<GLfloat> vec = readVertices_aux(pRoot->FirstChildElement("vertex"));
	valarray<GLfloat> vertices(vec.data(),vec.size());
	return vertices;
}
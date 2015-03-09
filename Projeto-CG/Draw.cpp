#include <valarray>
#include <vector>
#include "tinyxml2.h"
#include "Draw.h"
#include <GL/GLUT.h>

using namespace std;

void drawVertices(valarray<GLfloat> vertices){
	for (unsigned int i = 0, j = 0; i < vertices.size(); i += 3, j++){
		glBegin(GL_TRIANGLES);
		glNormal3f(vertices[i], vertices[i + 1], vertices[i + 2]);
		glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
		i += 3;
		glNormal3f(vertices[i], vertices[i + 1], vertices[i + 2]);
		glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
		i += 3;
		glNormal3f(vertices[i], vertices[i + 1], vertices[i + 2]);
		glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
		glEnd();
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
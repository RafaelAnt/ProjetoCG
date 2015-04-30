#include <vector>
#include "Utils.h"
#include "CatmullRom.h"
#include <GL/GLUT.h>


#pragma comment(lib, "glew32.lib")

using namespace std;


// catmull-rom matrix
float m[4][4] = { { -0.5f, 1.5f, -1.5f, 0.5f },
{ 1.0f, -2.5f, 2.0f, -0.5f },
{ -0.5f, 0.0f, 0.5f, 0.0f },
{ 0.0f, 1.0f, 0.0f, 0.0f } };

float EPSILON = 0.0001;

//Prototipos
Point getCatmullRomPoint(float t, int *indices, vector<Point> p);

bool igual(float a, float b){
	float diff = a - b;
	return (diff < EPSILON) && (-diff < EPSILON);
}

/* Metodo que dado um vector com 3 posições, mormaliza-o.
*/
Point normalizeVector(Point vec){
	GLfloat length = (float)sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	vec.x = vec.x / length;
	vec.y = vec.y / length;
	vec.z = vec.z / length;
	return vec;
}

/*Função que calcula o produto vectorial de dois arrays de 3 elementos
*/
Point prodVectorial(Point a, Point b){
	Point res;
	res.x = a.y * b.z - a.z * b.y;
	res.y = a.z * b.x - a.x * b.z;
	res.z = a.x * b.y - a.y * b.x;
	return res;
}

//dado um t global calcular o ponto na curva
Point getGlobalCatmullRomPoint(GLfloat gt, vector<Point> p) {

	int POINT_COUNT = p.size();

	float t = gt * POINT_COUNT; //obter t global verdadeiro
	int index = floor(t);  //segmento
	t = t - index; //posição no segmento

	//obter os indices dos nossos pontos de controlo
	int indices[4];
	indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
	indices[1] = (indices[0] + 1) % POINT_COUNT;
	indices[2] = (indices[1] + 1) % POINT_COUNT;
	indices[3] = (indices[2] + 1) % POINT_COUNT;

	return getCatmullRomPoint(t, indices, p);

}


Point getCatmullRomPoint(float t, int *indices, vector<Point> p) {

	int sum = 0;
	Point res;// res diz a posição do objeto na curva

	// M = catmull-rom matrix


	res.x = 0.0;
	res.y = 0.0;
	res.z = 0.0;

	// Compute point res = T * M * P

	//preencher a matriz T
	float matrizT[4] = { t*t*t, t*t, t, 1 };

	//preencher a matriz P
	// where Pi = p[indices[i]]
	float matrizP[4][3];

	for (int i = 0; i < 4; i++){
		matrizP[i][0] = p[indices[i]].x;
		matrizP[i][1] = p[indices[i]].y;
		matrizP[i][2] = p[indices[i]].z;
	}


	float aux[4];
	for (int i = 0; i < 4; i++) {
		float sumAux = 0;
		for (int j = 0; j < 4; j++) {
			sumAux += matrizT[j] * m[j][i];

		}
		aux[i] = sumAux;
	}

	for (int i = 0; i < 3; i++) {
		float sumAux = 0;
		for (int j = 0; j < 4; j++) {
			sumAux += aux[j] * matrizP[j][i];

		}
		if (i == 0)res.x = sumAux;
		if (i == 1)res.y = sumAux;
		if (i == 2)res.z = sumAux;
	}
	return res;
}


Point getDerivadaAux(float t, int *indices, vector<Point> p) {

	int sum = 0;
	Point res;

	res.x = 0.0;
	res.y = 0.0;
	res.z = 0.0;

	// Compute point res = T * M * P

	//preencher a matriz T
	float matrizT[4] = { 3 * t*t, 2 * t, 1, 0 };

	//preencher a matriz P
	// where Pi = p[indices[i]]
	float matrizP[4][3];

	for (int i = 0; i < 4; i++){
		matrizP[i][0] = p[indices[i]].x;
		matrizP[i][1] = p[indices[i]].y;
		matrizP[i][2] = p[indices[i]].z;
	}


	float aux[4];
	for (int i = 0; i < 4; i++) {
		float sumAux = 0;
		for (int j = 0; j < 4; j++) {
			sumAux += matrizT[j] * m[j][i];

		}
		aux[i] = sumAux;
	}

	for (int i = 0; i < 3; i++) {
		float sumAux = 0;
		for (int j = 0; j < 4; j++) {
			sumAux += aux[j] * matrizP[j][i];

		}
		if (i == 0)res.x = sumAux;
		if (i == 1)res.y = sumAux;
		if (i == 2)res.z = sumAux;
	}

	return res;
}

Point getDerivada(float gt, vector<Point> p){

	int POINT_COUNT = p.size();

	float t = gt * POINT_COUNT; //obter t global verdadeiro
	int index = floor(t);  //segmento
	t = t - index; //posição no segmento

	//obter os indices dos nossos pontos de controlo
	int indices[4];
	indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
	indices[1] = (indices[0] + 1) % POINT_COUNT;
	indices[2] = (indices[1] + 1) % POINT_COUNT;
	indices[3] = (indices[2] + 1) % POINT_COUNT;


	return getDerivadaAux(t, indices, p);
}

void renderCatmullRomCurve(vector<Point> p) {

	// desenhar a curva usando segmentos de reta - GL_LINE_LOOP

	float res[3];

	glBegin(GL_LINE_LOOP);
	for (float f = 0; f < 1; f += 0.01){
		Point res = getGlobalCatmullRomPoint(f, p);
		glVertex3f(res.x, res.y, res.z);
	}
	glEnd();

	for (float f = 0; f < 1; f += 0.01){
		glBegin(GL_LINES);
		Point res = getGlobalCatmullRomPoint(f, p);
		glVertex3f(res.x, res.y, res.z);
		Point dev = getDerivada(f, p);
		glVertex3f(res.x + dev.x, res.y + dev.y, res.z + dev.z);
		glEnd();
	}
}

void catmullRomCurveMovement(GLfloat gt, vector<Point> p){

	Point pos, der, r, up, b = { 0, 1, 0 };

	pos = getGlobalCatmullRomPoint(gt, p);

	der = getDerivada(gt, p);

	r = prodVectorial(b, der);
	printf("%f %f %f\n", r.x, r.y, r.z);
	up = prodVectorial(r, der);
	printf("%f %f %f\n", up.x, up.y, up.z);

	der = normalizeVector(der);
	up = normalizeVector(up);
	r = normalizeVector(r);

	GLfloat matriz[16] = { der.x, der.y, der.z, 0,
		up.x, up.y, up.z, 0,
		r.x, r.y, r.z, 0,
		pos.x, pos.y, pos.z, 1.0f };

	renderCatmullRomCurve(p);
	glMultMatrixf(matriz);

	GLUquadric* cyl = gluNewQuadric();
	GLUquadric* cy2 = gluNewQuadric();
	GLUquadric* cy3 = gluNewQuadric();

	glPushMatrix();
	double len = 1.0;
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(len, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, len, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, len);
	glEnd();
	glColor3f(0, 1.0, 0);
	glPopMatrix();
}
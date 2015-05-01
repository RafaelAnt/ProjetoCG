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


/*float EPSILON = 0.0001;

//Prototipos
Point getCatmullRomPoint(float t, int *indices, vector<Point> p);

/* Metodo que dado um vector com 3 posições, mormaliza-o.
*//*
Point normalizeVector(Point vec){
	GLfloat length = (float)sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	vec.x = vec.x / length;
	vec.y = vec.y / length;
	vec.z = vec.z / length;
	return vec;
}

/*Função que calcula o produto vectorial de dois arrays de 3 elementos
*//*
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
	printf("DER %f %f %f\n", der.x, der.y, der.z);
	printf("%f %f %f\n", r.x, r.y, r.z);
	up = prodVectorial(der, r);
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
}*/

//float p[200][3];

#define POINT_COUNT 5
// Points that make up the loop for catmull-rom interpolation
float p[POINT_COUNT][3] = { { -1, -1, 0 }, { -1, 1, 0 }, { 1, 1, 0 }, { 0, 0, 0 }, { 1, -1, 0 } };



//Prototipos:
void renderCatmullRomCurve();
void catmullRomCurveMovement(float gt, vector<Point> pontos);
void getDerivadaAux(float t, int *indices, float *res);
void getDerivada(float gt, float *res);
void getCatmullRomPoint(float t, int *indices, float *res);
void getGlobalCatmullRomPoint(float gt, float *res);
void prodVectorial(float * a, float * b, float * res);
void normalizeVector(float vec[]);



void catmullRomCurveMovement(float gt, vector<Point> pontos){

	//printf("GT: %f\n", gt);

	/*POINT_COUNT = pontos.size();

	for (int i = 0; i < POINT_COUNT; i++){
		p[i][0] = pontos[i].x;
		p[i][1] = pontos[i].y;
		p[i][2] = pontos[i].z;
	}*/

	float pos[3], der[3], r[3], up[3], b[3] = { 0, 1, 0 };


	renderCatmullRomCurve();


	getGlobalCatmullRomPoint(gt, pos);
	getDerivada(gt, der);

	prodVectorial(der, b, r);
	prodVectorial(r, der, up);

	normalizeVector(der);
	normalizeVector(up);
	normalizeVector(r);

	float matrizTeaPot[16] = { der[0], der[1], der[2], 0,
		up[0], up[1], up[2], 0,
		r[0], r[1], r[2], 0,
		pos[0], pos[1], pos[2], 1.0f };


	//desenhar eixo vertical
	glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(pos[0], pos[1], pos[2]);
		glVertex3f(pos[0] + up[0], pos[1] + up[1], pos[2] + up[2]);
		glColor3f(1, 1, 1);
	glEnd();

	//desenhar eixo vertical
	glBegin(GL_LINES);
		glColor3f(0, 1, 0);
		glVertex3f(pos[0], pos[1], pos[2]);
		glVertex3f(pos[0] + der[0], pos[1] + der[1], pos[2] + der[2]);
		glColor3f(1, 1, 1);
	glEnd();


	//desenhar eixo vertical
	glBegin(GL_LINES);
		glColor3f(0, 0, 1);
		glVertex3f(pos[0], pos[1], pos[2]);
		glVertex3f(pos[0] + r[0], pos[1] + r[1], pos[2] + r[2]);
		glColor3f(1, 1, 1);
	glEnd();

	glMultMatrixf(matrizTeaPot);
	//glutWireTeapot(0.1);
}


void renderCatmullRomCurve() {

	// desenhar a curva usando segmentos de reta - GL_LINE_LOOP

	float res[3];
	float vec[3];

	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
		for (float f = 0; f < 1; f += 0.01){
			getGlobalCatmullRomPoint(f, res);
			glVertex3f(res[0], res[1], res[2]);
		}
	glEnd();

	/*glColor3f(0.25, 0.25, 0.25);
	for (float f = 0; f < 1; f += 0.01){
		glBegin(GL_LINES);
		
			getGlobalCatmullRomPoint(f, res);
			glVertex3f(res[0], res[1], res[2]);
			getDerivada(f, vec);
			glVertex3f(res[0] + vec[0], res[1] + vec[1], res[2] + vec[2]);
		glEnd();
	}
	glColor3f(1, 1, 1);*/
}


void getDerivadaAux(float t, int *indices, float *res){
	int sum = 0;

	// M = catmull-rom matrix
	float m[4][4] = { { -0.5f, 1.5f, -1.5f, 0.5f },
	{ 1.0f, -2.5f, 2.0f, -0.5f },
	{ -0.5f, 0.0f, 0.5f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f } };

	res[0] = 0.0; res[1] = 0.0; res[2] = 0.0;

	// res diz a posição do objeto na curva?
	// Compute point res = T * M * P

	//preencher a matriz T
	float matrizT[4] = { 3 * t*t, 2 * t, 1, 0 };

	//preencher a matriz P
	// where Pi = p[indices[i]]
	float matrizP[4][3];

	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 3; j++){
			matrizP[i][j] = p[indices[i]][j];
		}
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
		res[i] = sumAux;
	}
}

void getDerivada(float gt, float *res){

	float t = gt * POINT_COUNT; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4];
	indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
	indices[1] = (indices[0] + 1) % POINT_COUNT;
	indices[2] = (indices[1] + 1) % POINT_COUNT;
	indices[3] = (indices[2] + 1) % POINT_COUNT;

	getDerivadaAux(t, indices, res);
}


void getCatmullRomPoint(float t, int *indices, float *res) {

	int sum = 0;

	// M = catmull-rom matrix
	float m[4][4] = { { -0.5f, 1.5f, -1.5f, 0.5f },
	{ 1.0f, -2.5f, 2.0f, -0.5f },
	{ -0.5f, 0.0f, 0.5f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f } };

	res[0] = 0.0; res[1] = 0.0; res[2] = 0.0;

	// res diz a posição do objeto na curva?
	// Compute point res = T * M * P

	//preencher a matriz T
	float matrizT[4] = { t*t*t, t*t, t, 1 };

	//preencher a matriz P
	// where Pi = p[indices[i]]
	float matrizP[4][3];

	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 3; j++){
			matrizP[i][j] = p[indices[i]][j];
		}
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
		res[i] = sumAux;
	}


}


// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float *res) {

	float t = gt * POINT_COUNT; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4];
	indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
	indices[1] = (indices[0] + 1) % POINT_COUNT;
	indices[2] = (indices[1] + 1) % POINT_COUNT;
	indices[3] = (indices[2] + 1) % POINT_COUNT;

	getCatmullRomPoint(t, indices, res);
}

void prodVectorial(float * a, float * b, float * res){
	res[0] = (a[1] * b[2]) - (a[2] * b[1]);
	res[1] = -((a[0] * b[2]) - (a[2] * b[0]));
	res[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

void normalizeVector(float vec[]){
	float aux = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
	float aux2 = sqrt(aux);
	vec[0] = vec[0] / aux2;
	vec[1] = vec[1] / aux2;
	vec[2] = vec[2] / aux2;
}
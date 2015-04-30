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


//Prototipos
Point getCatmullRomPoint(float t, int *indices, vector<Point> p);


/* Metodo que dado um vector com 3 posições, mormaliza-o.
*/
void normalizeVector(float vec[]){
	float aux = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
	float aux2 = sqrt(aux);
	vec[0] = vec[0] / aux2;
	vec[1] = vec[1] / aux2;
	vec[2] = vec[2] / aux2;
}

/*Função que calcula o produto vectorial de dois arrays de 3 elementos
*/
void prodVectorial(float * a, float * b, float * res){
	res[0] = (a[1] * b[2]) - (a[2] * b[1]);
	res[1] = -((a[0] * b[2]) - (a[2] * b[0]));
	res[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

//dado um t global calcular o ponto na curva
Point getGlobalCatmullRomPoint(float gt, vector<Point> p) {

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
		for (int j = 0; j < 3; j++){
			if (j == 0)matrizP[i][j] = p[indices[i]].x;
			if (j == 1)matrizP[i][j] = p[indices[i]].y;
			if (j == 2)matrizP[i][j] = p[indices[i]].z;
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
		if (i == 0)res.x = sumAux;
		if (i == 1)res.y = sumAux;
		if (i == 2)res.z = sumAux;
	}

	return res;
}


Point getDerivadaAux(float t, int *indices, vector<Point> p) {

	int sum = 0;
	Point res;// res diz a posição do objeto na curva

	// M = catmull-rom matrix


	res.x = 0.0;
	res.y = 0.0;
	res.z = 0.0;

	// Compute point res = T * M * P

	//preencher a matriz T
	float matrizT[4] = { 3*t*t, 2*t, 1, 0 };

	//preencher a matriz P
	// where Pi = p[indices[i]]
	float matrizP[4][3];

	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 3; j++){
			if (j == 0)matrizP[i][j] = p[indices[i]].x;
			if (j == 1)matrizP[i][j] = p[indices[i]].y;
			if (j == 2)matrizP[i][j] = p[indices[i]].z;
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

	/*for (float f = 0; f < 1; f += 0.01){
	glBegin(GL_LINES);
	getGlobalCatmullRomPoint(f, res);
	glVertex3f(res[0], res[1], res[2]);
	getDerivada(f, vec);
	glVertex3f(res[0] + vec[0], res[1] + vec[1], res[2] + vec[2]);
	glEnd();
	}*/
}

void catmullRomCurveMovement(float gt, vector<Point> p){

	GLfloat pos[3], der[3], r[3], up[3];

	Point aux;

	aux=getGlobalCatmullRomPoint(gt, p);
	pos[0] = aux.x;
	pos[1] = aux.y;
	pos[2] = aux.z;

	aux=getDerivada(gt, p);
	der[0] = aux.x;
	der[1] = aux.y;
	der[2] = aux.z;

	float b[3] = { 0, 1, 0 };

	prodVectorial(der, b, r);

	prodVectorial(r, der, up);


	normalizeVector(der);
	normalizeVector(up);
	normalizeVector(r);

	float matriz[16] = { der[0], der[1], der[2], 0,
		up[0], up[1], up[2], 0,
		r[0], r[1], r[2], 0,
		pos[0], pos[1], pos[2], 1.0f };

	renderCatmullRomCurve(p);
	glMultMatrixf(matriz);

}
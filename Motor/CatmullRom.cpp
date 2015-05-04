#include <vector>
#include "Utils.h"
#include "CatmullRom.h"
#include <GL/GLUT.h>


#pragma comment(lib, "glew32.lib")

using namespace std;


// catmull-rom matrix
static float m[4][4] = { { -0.5f, 1.5f, -1.5f, 0.5f},
						 { 1.0f, -2.5f, 2.0f, -0.5f},
						 { -0.5f, 0.0f, 0.5f, 0.0f},
						 { 0.0f, 1.0f, 0.0f, 0.0f}};

static float EPSILON = 0.0001;
//Metodo que dado um vector, mormaliza-o.

static Point normalizeVector(Point vec){
	GLfloat length = (float)sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	vec.x = vec.x / length;
	vec.y = vec.y / length;
	vec.z = vec.z / length;
	return vec;
}

//obtem um ponto num segmento da curva
static Point getCatmullRomPoint(float t, int *indices, vector<Point> p) {

	int sum = 0;
	Point res;// res diz a posição do objeto na curva
	res.x = 0.0;
	res.y = 0.0;
	res.z = 0.0;

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

//dado um t global calcular o ponto na curva
static Point getGlobalCatmullRomPoint(GLfloat gt, vector<Point> p) {

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

//calcula um ponto na curva definida pelos pontos de controlo p, num instante gt [0,1]
void catmullRomCurveMovement(GLfloat gt, vector<Point> p){

	Point pos, b = { 0, 1, 0 };

	pos = getGlobalCatmullRomPoint(gt, p);

	glTranslatef(pos.x, pos.y, pos.z);
}
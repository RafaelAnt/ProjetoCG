#include <math.h>
#include <assert.h>
#include "Bezier.h"

using namespace std;

Point mult(float f, Point p){
	p.x *= f;
	p.y *= f;
	p.z *= f;
	return p;
}

int factorial(int n) {
	int result = 1;
	for (int i = n; i > 1; i--)
		result *= i;
	return result;
}

float binomial_coefficient(int i, int n) {
	return 1.0f * factorial(n) / (factorial(i) * factorial(n - i));
}

float bernstein_polynomial(int i, int n, float u) {
	return binomial_coefficient(i, n) * powf(u, i) * powf(1 - u, n - i);
}

//constroi os pontos de controlos dado o conjunto de pontos e os vertices
void makeControlPoints(int patch, Point pontos_control[][4], vector<Point> vertices, vector<vector<unsigned int>> indices){
	for (int i = 0, k = 0; i <= NM; i++){
		for (int j = 0; j <= NM; j++, k++){
			pontos_control[i][j] = vertices[indices[patch][k] - 1];
		}
	}
}


void MMult1x4By4x4(float a[4], float b[4][4], float res[4]){
	for (int i = 0; i < 4; i++) {
		float sumAux = 0;
		for (int j = 0; j < 4; j++) {
			sumAux += a[j] * b[j][i];

		}
		res[i] = sumAux;
	}
}

float MMult1x4By4x1(float a[4], float b[4]){
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]);
}


//funçao que obtem um ponto numa superficie bezier dado u , v e os pontos de controlo
Point getBezierPoint(Point pontos[][NM + 1], float u, float v) {

	Point res;

	float U[4] = { u*u*u, u*u, u, 1 };

	float V[4] = { v*v*v, v*v, v, 1 };

	float M[4][4] = {
			{ -1, 3 - 3, 1 },
			{ 3, -6, 3, 0 },
			{ -3, 3, 0, 0 },
			{ 1, 0, 0, 0 }
	};

	float Px[4][4] = {
			{ pontos[0][0].x, pontos[0][1].x, pontos[0][2].x, pontos[0][3].x },
			{ pontos[1][0].x, pontos[1][1].x, pontos[1][2].x, pontos[1][3].x },
			{ pontos[2][0].x, pontos[2][1].x, pontos[2][2].x, pontos[2][3].x },
			{ pontos[3][0].x, pontos[3][1].x, pontos[3][2].x, pontos[3][3].x }
	};

	float Py[4][4] = {
			{ pontos[0][0].y, pontos[0][1].y, pontos[0][2].y, pontos[0][3].y },
			{ pontos[1][0].y, pontos[1][1].y, pontos[1][2].y, pontos[1][3].y },
			{ pontos[2][0].y, pontos[2][1].y, pontos[2][2].y, pontos[2][3].y },
			{ pontos[3][0].y, pontos[3][1].y, pontos[3][2].y, pontos[3][3].y }
	};

	float Pz[4][4] = {
			{ pontos[0][0].z, pontos[0][1].z, pontos[0][2].z, pontos[0][3].z },
			{ pontos[1][0].z, pontos[1][1].z, pontos[1][2].z, pontos[1][3].z },
			{ pontos[2][0].z, pontos[2][1].z, pontos[2][2].z, pontos[2][3].z },
			{ pontos[3][0].z, pontos[3][1].z, pontos[3][2].z, pontos[3][3].z }
	};

	float aux[4];
	float aux2[4];

	/*definir o ponto x*/
	MMult1x4By4x4(U, M, aux);
	MMult1x4By4x4(aux, Px, aux2);
	MMult1x4By4x4(aux2, M, aux);
	res.x = MMult1x4By4x1(aux, V);

	/*definir o ponto y*/
	MMult1x4By4x4(U, M, aux);
	MMult1x4By4x4(aux, Py, aux2);
	MMult1x4By4x4(aux2, M, aux);
	res.y = MMult1x4By4x1(aux, V);

	/*definir o ponto z*/
	MMult1x4By4x4(U, M, aux);
	MMult1x4By4x4(aux, Pz, aux2);
	MMult1x4By4x4(aux2, M, aux);
	res.z = MMult1x4By4x1(aux, V);


	return res;
}

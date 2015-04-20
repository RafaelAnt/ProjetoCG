#include <math.h>
#include "Bezier.h"

using namespace std;

Point mult(float f, Point p){
	p.x *= f;
	p.y *= f;
	p.z *= f;
	return p;
}

int fac(int n) {
	if (n < 0)
		return -1;
	int result = 1;
	for (int i = n; i > 1; i--)
		result *= i;
	return result;
}

float coef_binomial(int i, int n){
	if (i >= 0 || n >= 0)
		return 1.0f * fac(n) / (fac(i) * fac(n - i));
	else
		return -1;
}

float poli_bernstein(int i, int n, float u) {
	return coef_binomial(i, n) * powf(u, i) * powf(1 - u, n - i);
}

//constroi os pontos de controlos dado o conjunto de pontos e os vertices
void makeControlPoints(int patch, Point pontos_control[][NM + 1], vector<Point> vertices, vector<vector<unsigned int>> indices){
	for (int i = 0; i < indices.size(); i++){
		for (int j = 0; j <= NM; j++)
			pontos_control[i][j] = vertices[indices[patch][i]];
	}
}

//funçao que obtem um ponto numa superficie bezier dado u , v e os pontos de controlo
Point getBezierPoint(Point pontos_control[][NM + 1], float u, float v){
	Point res;
	for (int i = 0; i <= NM; i++) {
		for (int j = 0; j <= NM; j++) {
			float poli_i = poli_bernstein(i, NM, u);
			float poli_j = poli_bernstein(j, NM, v);
			res = mult(i, mult(j, pontos_control[i][j])); //i*j*p
		}
	}
	return res;
}

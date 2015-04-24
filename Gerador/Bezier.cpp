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

//funçao que obtem um ponto numa superficie bezier dado u , v e os pontos de controlo
Point getBezierPoint(Point control_points_k[][NM + 1], float u, float v) {
	Point result = { 0.0, 0.0, 0.0 };
	for (int i = 0; i <= NM; i++) {
		for (int j = 0; j <= NM; j++) {
			float poly_i = bernstein_polynomial(i, NM, u);
			float poly_j = bernstein_polynomial(j, NM, v);
			result.x += poly_i * poly_j * control_points_k[i][j].x;
			result.y += poly_i * poly_j * control_points_k[i][j].y;
			result.z += poly_i * poly_j * control_points_k[i][j].z;
		}
	}
	return result;
}

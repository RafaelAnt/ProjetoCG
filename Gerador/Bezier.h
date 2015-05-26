#ifndef BEZIER_H

#define BEZIER_H

#include <vector>
using namespace std;
#define NM 3

typedef struct ponto{
	float x;
	float y;
	float z;
}Point;

typedef struct patch{
	vector<vector<Point>> vertices;
	vector<vector<Point>> normais;
}Patch;

Point getBezierPoint(Point pontos_control[][NM+1], float u, float v);
Point getBezierNormal(Point pontos[][NM + 1], float u, float v);
void makeControlPoints(int patch, Point pontos_control[][NM+1], vector<Point> vertices, vector<vector<unsigned int>> indices);

#endif 



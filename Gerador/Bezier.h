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

Point getBezierPoint(Point pontos_control[][NM+1], float u, float v);
void makeControlPoints(int patch, Point pontos_control[][NM+1], vector<Point> vertices, vector<vector<unsigned int>> indices);

#endif 



#include <vector>
#include <valarray>
#include <GL/GLUT.h>

using namespace std;

vector<GLfloat> readVertices(const char *filename);

void drawVertices(valarray<GLfloat> vertices);


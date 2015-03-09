#include <vector>
#include <valarray>
#include <GL/GLUT.h>

using namespace std;

valarray<GLfloat> readVertices(const char *filename);

void drawVertices(valarray<GLfloat> vertices);


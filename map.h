#ifndef _MAP_
#define _MAP_

void initMap(GLuint program);
void drawMap(mat4 camMatrix);
void readMapFile(mat4 camMatrix, int *maxX, int *maxZ);
#endif

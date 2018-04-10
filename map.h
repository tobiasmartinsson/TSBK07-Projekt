#ifndef _MAP_
#define _MAP_

struct WallPiece{
  mat4 wallTrans;
  char wallType;
};

void initMap(GLuint program);
void readMapFile(char* mapName, mat4 camMatrix);
void reDrawWall(mat4 camMat);

struct WallPiece wallList[10000];
int numOfWalls;



#endif

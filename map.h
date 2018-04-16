#ifndef _MAP_
#define _MAP_

struct WallPiece{
  mat4 wallTrans;
  char wallType;
  float numberSequence[64];
  float lightSequence[64];
  float textureSpeed;
  float textureSpeed2;
};

void initMap(GLuint program);
void readMapFile(char* mapName, mat4 camMatrix);
void reDrawWall(mat4 camMat);

struct WallPiece wallList[10000];
int numOfWalls;



#endif

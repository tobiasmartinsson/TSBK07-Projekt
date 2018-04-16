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
void reDrawMap(mat4 camMat);
void updateLight();


struct WallPiece wallList[10000];
int numOfWalls;
int startPos[2];
int endPos[2];



#endif

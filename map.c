#ifdef __APPLE__
#include <OpenGL/gl3.h>
// Linking hint for Lightweight IDE
// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include  "map.h"



unsigned int vertexArrayObjID;
unsigned int vertexBufferObjID;
unsigned int groundTexCoordBufferObjID;
unsigned int groundNormalBufferObjID;



GLfloat groundMatrix[] = {
	-0.5, 0, 0.5,
	-0.5, 0, -0.5,
	0.5, 0, -0.5,

	0.5, 0, -0.5,
	-0.5, 0, 0.5,
	0.5, 0, 0.5
};

GLfloat groundTextureCoord[]={
	0.0f,8.0f,
	0.0f,0.0f,
	8.0f,0.0f,
	8.0f,0.0f,
	0.0f,8.0f,
	8.0f,8.0f
};

GLfloat groundNormal[] = {
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	0, 1, 0,
	0, 1, 0,
	0, 1, 0
};


int maxX = 0;
int maxZ = 0;
GLuint shaderProgram;
GLuint oneTex, zeroTex;
mat4 groundTransform, wallTransformT, wallTransformB, wallTransformL, wallTransformR;

void initMap(GLuint program){
  shaderProgram = program;
  LoadTGATextureSimple("number1.tga", &oneTex);
	LoadTGATextureSimple("number0.tga", &zeroTex);

  // Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &groundNormalBufferObjID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, oneTex);
	glUniform1i(glGetUniformLocation(shaderProgram, "texUnit"), 0); // Texture unit 0
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, zeroTex);
	glUniform1i(glGetUniformLocation(shaderProgram, "texUnit2"), 1); // Texture unit 0

	glGenBuffers(1, &groundTexCoordBufferObjID);  //TEXTURE

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	//glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, 3*6*sizeof(GLfloat), groundMatrix, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));

	glBindBuffer(GL_ARRAY_BUFFER, groundTexCoordBufferObjID);
	glBufferData(GL_ARRAY_BUFFER,6*2*sizeof(GLfloat), groundTextureCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));

	glBindBuffer(GL_ARRAY_BUFFER, groundNormalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 3*6*sizeof(GLfloat), groundNormal, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));


	groundTransform = IdentityMatrix();
  //groundTransform = Mult(Rx(M_PI/2), groundTransform);
  groundTransform = Mult(T(0,0,0),groundTransform);

	wallTransformT = Mult(Rx(M_PI/2), groundTransform);
  wallTransformT = Mult(T(0,0.5,-0.5),wallTransformT);

	wallTransformB = Mult(Ry(M_PI),wallTransformT);
  //wallTransformB = Mult(T(0,0,1),wallTransformB);

  wallTransformL = Mult(Rz(M_PI/2), groundTransform);
	wallTransformL = Mult(Rx(M_PI/2), wallTransformL);
	wallTransformL = Mult(Ry(M_PI),wallTransformL);
  wallTransformL = Mult(T(-0.5,0.5,0),wallTransformL);

	wallTransformR = Mult(Ry(M_PI),wallTransformL);

	numOfWalls = 0;
}

void drawSquare(mat4 camMat, mat4 squareTransform){
	squareTransform = Mult(camMat, squareTransform);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mdlMatrix"), 1, GL_TRUE, squareTransform.m);
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 2*3);	// draw object
}

void drawFloor(mat4 camMatrix){
	//Draws the ground
	int i, j;
  for(i = 0; i < maxX; i++){
    for(j = 0; j < maxZ; j++){
        groundTransform.m[3] = i;
        groundTransform.m[11] = j;
        drawSquare(camMatrix, groundTransform);
    }
  }
}

void reDrawWall(mat4 camMat){
	int i;
	GLfloat textureTimer = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	glUniform1f(glGetUniformLocation(shaderProgram, "textureTimer"), textureTimer);

	for(i = 0; i < numOfWalls; i++ ){
		glUniform1f(glGetUniformLocation(shaderProgram, "randomValue"), wallList[i].textureSpeed);
		glUniform1f(glGetUniformLocation(shaderProgram, "randomValue2"), wallList[i].textureSpeed2);
		glUniform1fv(glGetUniformLocation(shaderProgram, "numberSequence"),64, wallList[i].numberSequence);
		glUniform1fv(glGetUniformLocation(shaderProgram, "lightSequence"),64, wallList[i].lightSequence);
		drawSquare(camMat, wallList[i].wallTrans);
	}

	drawFloor(camMat);
}

int randSign(){
	if(rand() % 2 == 0) return -1;
	else return 1;
}

void generateRandomSequence (){
	int i;
	for(i = 0; i < 64; i++){
				if(randSign() < 0) wallList[numOfWalls].numberSequence[i] = 0.0f;
				else wallList[numOfWalls].numberSequence[i] = 1.0f;

				if(randSign() < 0) wallList[numOfWalls].lightSequence[i] = 0.0f;
				else wallList[numOfWalls].lightSequence[i] = 1.0f;
	}
}

void drawWall(mat4 camMatrix, mat4 wallTrans, int x, int z, char wallC){
  wallTrans.m[3] += x;
  wallTrans.m[11] += z;
	wallList[numOfWalls].wallTrans = wallTrans;
	wallList[numOfWalls].wallType = wallC;
	wallList[numOfWalls].textureSpeed = randSign() * ((rand() % 25) +1.0f)/10.0f;
	wallList[numOfWalls].textureSpeed2 = randSign() * ((rand() % 25) +1.0f)/10.0f;
	wallList[numOfWalls].numberSequence[0] = 0.0f;
	wallList[numOfWalls].lightSequence[0] = 0.0f;
	generateRandomSequence();
	numOfWalls++;

	glUniform1f(glGetUniformLocation(shaderProgram, "randomValue"), wallList[numOfWalls].textureSpeed);
	glUniform1f(glGetUniformLocation(shaderProgram, "randomValue2"), wallList[numOfWalls].textureSpeed2);
	glUniform1fv(glGetUniformLocation(shaderProgram, "numberSequence"),64, wallList[numOfWalls].numberSequence);
	glUniform1fv(glGetUniformLocation(shaderProgram, "numberSequence"),64, wallList[numOfWalls].lightSequence);
  drawSquare(camMatrix, wallTrans);
}



void evalutateChar(char c, mat4 camMatrix, int charNum, int lineNum){
	switch (c) {
		case 'T':
			drawWall(camMatrix, wallTransformT, charNum, lineNum, 'X');
			break;
		case 'B':
			drawWall(camMatrix, wallTransformB, charNum, lineNum, 'X');
			break;
		case 'L':
			drawWall(camMatrix, wallTransformL, charNum, lineNum, 'Z');
			break;
		case 'R':
			drawWall(camMatrix, wallTransformR, charNum, lineNum, 'Z');
			break;
		case '1':
			drawWall(camMatrix, wallTransformT, charNum, lineNum, 'X');
			drawWall(camMatrix, wallTransformL, charNum, lineNum, 'Z');
			break;
		case '2':
			drawWall(camMatrix, wallTransformT, charNum, lineNum, 'X');
			drawWall(camMatrix, wallTransformR, charNum, lineNum, 'Z');
			break;
		case '3':
			drawWall(camMatrix, wallTransformB, charNum, lineNum, 'X');
			drawWall(camMatrix, wallTransformL, charNum, lineNum, 'Z');
			break;
		case '4':
			drawWall(camMatrix, wallTransformB, charNum, lineNum, 'X');
			drawWall(camMatrix, wallTransformR, charNum, lineNum, 'Z');
			break;
		default:
			break;
	}
}

void readMapFile(char* mapName, mat4 camMatrix){
  FILE *mapFile;
  mapFile = fopen(mapName,"r");
  char * curLine = NULL;
  size_t size = 100;
  int lineNum = 0;

  while(getline(&curLine, &size, mapFile) != -1){
    int charNum = 0;
    char c = *curLine;

    while(c){
      //printf("%c \n",c);
			evalutateChar(c, camMatrix, charNum, lineNum);
			charNum++;
      c = *(curLine+charNum);
    }

		if(charNum > maxX){
			maxX = charNum;
		}

      lineNum++;
  }
	maxZ = lineNum;
  fclose(mapFile);
}

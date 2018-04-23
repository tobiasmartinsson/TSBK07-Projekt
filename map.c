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

Model *phoneBooth;

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
GLuint shaderProgram, phoneBoothProgram;
GLuint oneTex, oneTex2, oneTex3, zeroTex, zeroTex2, zeroTex3, phoneBoothTex;
mat4 groundTransform, wallTransformT, wallTransformB, wallTransformL, wallTransformR, phoneBoothTransform;

mat4 projectionMat;

void initMap(GLuint program){
  shaderProgram = program;

  LoadTGATextureSimple("number1.tga", &oneTex);
	LoadTGATextureSimple("number0.tga", &zeroTex);
	LoadTGATextureSimple("number0kopia.tga", &zeroTex2);
	LoadTGATextureSimple("number0kopia2.tga", &zeroTex3);
	LoadTGATextureSimple("number1kopia.tga", &oneTex2);
	LoadTGATextureSimple("number1kopia2.tga", &oneTex3);
	LoadTGATextureSimple("1796_phone_booth_01_D.tga", &phoneBoothTex);

	phoneBoothProgram = loadShaders("phoneBooth.vert", "phoneBooth.frag");
	phoneBooth = LoadModelPlus("phone_booth.obj");

  // Allocate and activate Vertex Array Object
	glUseProgram(shaderProgram);
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &groundNormalBufferObjID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, oneTex);
	glUniform1i(glGetUniformLocation(shaderProgram, "texUnit"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, zeroTex);
	glUniform1i(glGetUniformLocation(shaderProgram, "texUnit2"), 1);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, zeroTex2);
	glUniform1i(glGetUniformLocation(shaderProgram, "texUnit22"), 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, zeroTex3);
	glUniform1i(glGetUniformLocation(shaderProgram, "texUnit23"), 4);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, oneTex2);
	glUniform1i(glGetUniformLocation(shaderProgram, "texUnit12"), 5);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, oneTex3);
	glUniform1i(glGetUniformLocation(shaderProgram, "texUnit13"), 6);


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

	glUseProgram(phoneBoothProgram);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, phoneBoothTex);
	glUniform1i(glGetUniformLocation(phoneBoothProgram, "texUnit"), 2);

	groundTransform = IdentityMatrix();
  //groundTransform = Mult(Rx(M_PI/2), groundTransform);
  groundTransform = Mult(T(0,0,0),groundTransform);

	wallTransformB = Mult(Rx(M_PI/2), groundTransform);
  wallTransformB = Mult(T(0,0.5,0.5),wallTransformB);

	wallTransformT = Mult(Ry(M_PI),wallTransformB);
  //wallTransformB = Mult(T(0,0,1),wallTransformB);

  wallTransformR = Mult(Rz(M_PI/2), groundTransform);
	wallTransformR = Mult(Rx(M_PI/2), wallTransformR);
	wallTransformR = Mult(Ry(M_PI),wallTransformR);
  wallTransformR = Mult(T(0.5,0.5,0),wallTransformR);

	wallTransformL = Mult(Ry(M_PI),wallTransformR);

	numOfWalls = 0;
	startPos[0] = 0;
	startPos[1] = 0;
	endPos[0] = 0;
	endPos[1] = 0;

	phoneBoothTransform = IdentityMatrix();
	phoneBoothTransform = Mult(S(0.004,0.004,0.004),phoneBoothTransform);
	projectionMat =  frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);
}

void drawSquare(mat4 camMat, mat4 squareTransform){
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "camMatrix"), 1, GL_TRUE, camMat.m);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mdlMatrix"), 1, GL_TRUE, squareTransform.m);
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 2*3);	// draw object
}


int randSign(){
	if(rand() % 2 == 0) return -1;
	else return 1;
}

void generateRandomLightSequence (int wallNum){
	int i;
	for(i = 0; i < 64; i++){
				if(randSign() < 0) wallList[wallNum].lightSequence[i] = 0.0f;
				else wallList[wallNum].lightSequence[i] = 1.0f;
	}
}

void updateLight(){
	int i;
	for(i = 0; i < numOfWalls; i++ ){
		generateRandomLightSequence(i);
	}
}

void drawPhoneBooth(mat4 camMat, vec3 camPos){
	glUseProgram(phoneBoothProgram);
	glUniformMatrix4fv(glGetUniformLocation(phoneBoothProgram, "projMatrix"), 1, GL_TRUE, projectionMat.m);
	glUniformMatrix4fv(glGetUniformLocation(phoneBoothProgram, "camMatrix"), 1, GL_TRUE, camMat.m);
	glUniformMatrix4fv(glGetUniformLocation(phoneBoothProgram, "mdlMatrix"), 1, GL_TRUE, phoneBoothTransform.m);
	glUniform3fv(glGetUniformLocation(shaderProgram, "camPos"),1, &camPos.x);
	DrawModel(phoneBooth,phoneBoothProgram,"inPosition",NULL,"inTexCoord");
}

void reDrawMap(mat4 camMat, vec3 camPos){
	glUseProgram(shaderProgram);
	int i;
	GLfloat textureTimer = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	glUniform1f(glGetUniformLocation(shaderProgram, "textureTimer"), textureTimer);
	glUniform3fv(glGetUniformLocation(shaderProgram, "camPos"),1, &camPos.x);

	for(i = 0; i < numOfWalls; i++ ){
		glUniform1f(glGetUniformLocation(shaderProgram, "randomValue"), wallList[i].textureSpeed);
		glUniform1f(glGetUniformLocation(shaderProgram, "randomValue2"), wallList[i].textureSpeed2);
		glUniform1fv(glGetUniformLocation(shaderProgram, "numberSequence"),64, wallList[i].numberSequence);
		glUniform1fv(glGetUniformLocation(shaderProgram, "lightSequence"),64, wallList[i].lightSequence);
		drawSquare(camMat, wallList[i].wallTrans);
	}
	drawPhoneBooth(camMat, camPos);
	//drawFloor(camMat);
}

void generateRandomSequence(){
	int i;
	for(i = 0; i < 64; i++){
				if(randSign() < 0) wallList[numOfWalls].numberSequence[i] = 0.0f;
				else wallList[numOfWalls].numberSequence[i] = 1.0f;
	}
}



void addSquareToMap(mat4 camMatrix, mat4 wallTrans, int x, int z, char wallC){
  wallTrans.m[3] += x;
  wallTrans.m[11] += z;
	wallList[numOfWalls].wallTrans = wallTrans;
	wallList[numOfWalls].wallType = wallC;
	wallList[numOfWalls].textureSpeed = randSign() * ((rand() % 25) +1.0f)/10.0f;
	wallList[numOfWalls].textureSpeed2 = randSign() * ((rand() % 25) +1.0f)/10.0f;
	wallList[numOfWalls].numberSequence[0] = 0.0f;
	wallList[numOfWalls].lightSequence[0] = 0.0f;
	generateRandomSequence();
	generateRandomLightSequence(numOfWalls);
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
			addSquareToMap(camMatrix, wallTransformT, charNum, lineNum, 'X');
			addSquareToMap(camMatrix, groundTransform, charNum, lineNum, 'G');
			break;
		case 'B':
			addSquareToMap(camMatrix, wallTransformB, charNum, lineNum, 'X');
			addSquareToMap(camMatrix, groundTransform, charNum, lineNum, 'G');
			break;
		case 'L':
			addSquareToMap(camMatrix, wallTransformL, charNum, lineNum, 'Z');
			addSquareToMap(camMatrix, groundTransform, charNum, lineNum, 'G');
			break;
		case 'R':
			addSquareToMap(camMatrix, wallTransformR, charNum, lineNum, 'Z');
			addSquareToMap(camMatrix, groundTransform, charNum, lineNum, 'G');
			break;
		case '1':
			addSquareToMap(camMatrix, wallTransformT, charNum, lineNum, 'X');
			addSquareToMap(camMatrix, wallTransformL, charNum, lineNum, 'Z');
			addSquareToMap(camMatrix, groundTransform, charNum, lineNum, 'G');
			break;
		case '2':
			addSquareToMap(camMatrix, wallTransformT, charNum, lineNum, 'X');
			addSquareToMap(camMatrix, wallTransformR, charNum, lineNum, 'Z');
			addSquareToMap(camMatrix, groundTransform, charNum, lineNum, 'G');
			break;
		case '3':
			addSquareToMap(camMatrix, wallTransformB, charNum, lineNum, 'X');
			addSquareToMap(camMatrix, wallTransformL, charNum, lineNum, 'Z');
			addSquareToMap(camMatrix, groundTransform, charNum, lineNum, 'G');
			break;
		case '4':
			addSquareToMap(camMatrix, wallTransformB, charNum, lineNum, 'X');
			addSquareToMap(camMatrix, wallTransformR, charNum, lineNum, 'Z');
			addSquareToMap(camMatrix, groundTransform, charNum, lineNum, 'G');
			break;
		case 'G':
			addSquareToMap(camMatrix, groundTransform, charNum, lineNum, 'G');
			break;
		case 'W':
			addSquareToMap(camMatrix, wallTransformT, charNum, lineNum, 'X');
			addSquareToMap(camMatrix, wallTransformL, charNum, lineNum, 'Z');
			addSquareToMap(camMatrix, wallTransformB, charNum, lineNum, 'X');
			addSquareToMap(camMatrix, wallTransformR, charNum, lineNum, 'Z');
			break;
		case 'S':
			addSquareToMap(camMatrix, groundTransform, charNum, lineNum, 'G');
			startPos[0] = charNum;
			startPos[1] = lineNum;
			break;
		case 'E':
			addSquareToMap(camMatrix, groundTransform, charNum, lineNum, 'G');
			endPos[0] = charNum;
			endPos[1] = lineNum;
			phoneBoothTransform = IdentityMatrix();
			phoneBoothTransform = Mult(S(0.004,0.004,0.004),phoneBoothTransform);
			phoneBoothTransform.m[3] += endPos[0];
			phoneBoothTransform.m[11] += endPos[1]+0.3;
			break;
		default:
			break;
	}
}

void resetMapStuff(){
	memset(wallList, 0, sizeof(wallList));
	phoneBoothTransform = IdentityMatrix();
	phoneBoothTransform = Mult(S(0.004,0.004,0.004),phoneBoothTransform);
}

void readMapFile(char* mapName, mat4 camMatrix){
  FILE *mapFile;
  mapFile = fopen(mapName,"r");
  char * curLine = NULL;
  size_t size = 100;
  int lineNum = 0;

	resetMapStuff();

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

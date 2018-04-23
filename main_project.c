// Lab 4, terrain generation

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
#include "map.h"
#include <math.h>

mat4 projectionMatrix;



// Reference to shader program
GLuint program;

GLfloat angleY;
GLfloat angleX ;
GLfloat pastT;

bool freeCam = false;

vec3 cam = {0, 0.25, 0};
vec3 lookAtPoint = {1, 0, 1};
vec3 lookAtVector = {1,0,1};
vec3 up = {0,1,0};

GLfloat totalXRot = 0;
mat4 total, modelView, camMatrix;

float movementSpeed = 0.05;
int totalNumOfMaps = 6;
char * mapList[10] = {};
int currentMap;
void init(void)
{
	mapList[0] = "map0.txt";
	mapList[1] = "map1.txt";
	mapList[2] = "map2.txt";
	mapList[3] = "map3.txt";
	mapList[4] = "map4.txt";
	mapList[5] = "map5.txt";
	currentMap = 0;

	// GL inits
	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	pastT = 0;
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);
	angleY= 0.0f;
	angleX = 0.0f;

	camMatrix = lookAt(cam.x, cam.y, cam.z,
		lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
		up.x, up.y, up.z);


	// Load and compile shader
	program = loadShaders("main_project.vert", "main_project.frag");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	initMap(program);
	readMapFile(mapList[0], camMatrix);
	cam.x = startPos[0];
	cam.z = startPos[1];
}


void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	printError("pre display");
	glUseProgram(program);

	/** CAMERA **/

	vec3 tmpLook = lookAtVector;
	lookAtVector = MultVec3(ArbRotate(CrossProduct(tmpLook, up),angleX), lookAtVector);
	vec3 tmpUp = up;
	up = MultVec3(ArbRotate(CrossProduct(tmpLook, tmpUp),angleX), up);
	if(freeCam){
		//For free camera
		lookAtVector = MultVec3(ArbRotate(up, angleY), lookAtVector);
		up = MultVec3(ArbRotate(tmpUp, angleY), up);
	}else{
		//For fps camera
		lookAtVector = MultVec3(Ry(angleY), lookAtVector);
		up = MultVec3(Ry(angleY), up);
	}

	angleX = 0;
	angleY = 0;

	lookAtPoint = VectorAdd(cam, lookAtVector);
	if(!freeCam)
		cam.y = 0.25;

	camMatrix = lookAt(cam.x, cam.y, cam.z,
		lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
		up.x, up.y, up.z);

	/** CAMERA END **/


	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	//glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m);

	/*UPDATES LIGHT EVERY SECOND; FLICKERING LIGHTS*/
	GLfloat t = glutGet(GLUT_ELAPSED_TIME);
	if((t/1000 - pastT) > 1){
		updateLight();
		pastT = t/1000;
	}


	reDrawMap(camMatrix, cam);
	glutSwapBuffers();
}

void resetCamera(){
	angleY= 0.0f;
	angleX = 0.0f;
	cam.x = startPos[0];
	cam.y = 0.25;
	cam.z = startPos[1];
	lookAtVector.x = 1;
	lookAtVector.y = 0;
	lookAtVector.z = 1;
	up.x = 0;
	up.y = 1;
	up.z = 0;
	totalXRot = 0.0f;
}

bool wallCollision(vec3 movementVector){
	int j;
	if(movementVector.y <= 1.0f){
		for(j = 0; j < numOfWalls; j++){
			float wallX = wallList[j].wallTrans.m[3];
			float wallZ = wallList[j].wallTrans.m[11];
			float dist = 1000;

			if(wallList[j].wallType == 'X'){
				if(sqrt(pow(wallX-movementVector.x,2)) < 0.5){
					dist = sqrt(pow(wallZ-movementVector.z,2));
				}
			}else if(wallList[j].wallType == 'Z'){
				if(sqrt(pow(wallZ-movementVector.z,2)) < 0.5){
					dist = sqrt(pow(wallX-movementVector.x,2));
				}
			}
//0.65
			//printf("%.2f\n",dist );
			if(sqrt(pow(dist,2)) <= 0.25f){
				//printf("true");
				return true;
			}
		}
	}
	return false;
}

bool isAtEnd(){
	if((cam.x - (endPos[0]-0.5) > 0) && (cam.x - (endPos[0] +0.5) < 0) && (cam.z - (endPos[1]-0.5) > 0) && (cam.z -(endPos[1]+0.5) < 0)){
		return true;
	}

	return false;
}

void loadNextMap(){
	if(currentMap < totalNumOfMaps-1){
		currentMap++;
		resetCamera();
		readMapFile(mapList[currentMap], camMatrix);
		cam.x = startPos[0];
		cam.z = startPos[1];
		printf("%d\n", currentMap );
	}else
		printf("no more maps\n");
}

void moveCamera(){
	if(isAtEnd()) loadNextMap();

	if(glutKeyIsDown('w') || glutKeyIsDown('W')){
		vec3 moveVec = lookAtVector;
		if(!wallCollision(VectorAdd(cam,ScalarMult(moveVec,movementSpeed))))
			cam = VectorAdd(cam,ScalarMult(moveVec,movementSpeed));

		if(!freeCam)
			cam.y = 0.25;
	}
	if(glutKeyIsDown('s') || glutKeyIsDown('S')){
		vec3 moveVec = lookAtVector;
		if(!wallCollision(VectorSub(cam,ScalarMult(moveVec,movementSpeed))))
			cam = VectorSub(cam,ScalarMult(moveVec,movementSpeed));
		if(!freeCam)
			cam.y = 0.25;
	}
	if(glutKeyIsDown('a') || glutKeyIsDown('A')){

		vec3 left = Normalize(CrossProduct(up, lookAtVector));
		if(!wallCollision(VectorAdd(cam, ScalarMult(left,movementSpeed))))
			cam = VectorAdd(cam, ScalarMult(left,movementSpeed));
	}
	if(glutKeyIsDown('d') || glutKeyIsDown('d')){
		vec3 right = Normalize(CrossProduct(lookAtVector,up));
		if(!wallCollision(VectorAdd(cam, ScalarMult(right,movementSpeed))))
			cam = VectorAdd(cam, ScalarMult(right,movementSpeed));
	}
	if(glutKeyIsDown('r') || glutKeyIsDown('R')){
		resetCamera();
	}

	if(glutKeyIsDown('t') || glutKeyIsDown('T')){
			freeCam = !freeCam;
	}
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
	moveCamera();
}

	int prevX = 0;
	int prevY = 0;
	void mouse(int x, int y)
	{
		//printf("%d %d\n", x, y);

		if(!glutKeyIsDown('b')){
			glutHideCursor();
			if((x != prevX) || (y != prevY)){
				if((prevX - x) < 0){
					angleY -= 0.03;
				}
				else if ((prevX - x) > 0){
					angleY += 0.03;
				}

				if((totalXRot < M_PI/2) && (totalXRot > -M_PI/2)){
					if((prevY - y) < 0){
						angleX -= 0.03;
						totalXRot -= 0.03;
					}
					else if((prevY - y) > 0){
						angleX += 0.03;
						totalXRot += 0.03;
					}
				}else if(totalXRot >= M_PI/2){
					if((prevY - y) < 0){
						angleX -= 0.03;
						totalXRot -= 0.03;
					}
				}else if(totalXRot <= -M_PI/2){
					if((prevY - y) > 0){
						angleX += 0.03;
						totalXRot += 0.03;
					}
				}
				prevX = x;
				prevY = y;


				glutWarpPointer(300, 300);
			}
		}else{
			glutShowCursor();
		}
	}

	int main(int argc, char **argv)
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
		glutInitContextVersion(3, 2);
		glutInitWindowSize (600, 600);
		glutCreateWindow ("TSBK07 Project");
		glutDisplayFunc(display);
		init ();
		glutTimerFunc(20, &timer, 0);

		glutPassiveMotionFunc(mouse);

		glutMainLoop();
		exit(0);
	}

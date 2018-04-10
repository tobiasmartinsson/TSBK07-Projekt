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

mat4 projectionMatrix;

// vertex array object
Model *m;

// Reference to shader program
GLuint program;

GLfloat angleY;
GLfloat angleX ;

mat4 camTrans;

bool freeCam = false;


// vertex array object
unsigned int vertexArrayObjID;
unsigned int vertexBufferObjID;
unsigned int groundTexCoordBufferObjID;
unsigned int groundNormalBufferObjID;
vec3 cam = {0, 0.25, 0};
vec3 lookAtPoint = {0, 0, -1};
vec3 lookAtVector = {0,0,-1};
vec3 up = {0,1,0};

GLfloat totalXRot = 0;

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");


	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);
	angleY= 0.0f;
	angleX = 0.0f;
	camTrans = T(0,0,0);


	// Load and compile shader
	program = loadShaders("main_project.vert", "main_project.frag");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	initMap(program);
}


void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	mat4 total, modelView, camMatrix;
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


	//camMatrix = Mult(Ry(angleY),camMatrix);
	//camMatrix = Mult(Rx(angleX),camMatrix);
	//camMatrix = Mult(camTrans,camMatrix);

	/** CAMERA END **/


	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	//glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m);

	drawMap(camMatrix);
	glutSwapBuffers();
}

void resetCamera(){
	angleY= 0.0f;
	angleX = 0.0f;
	cam.x = 0;
	cam.y = 0.25;
	cam.z = 0;
	lookAtVector.x = 0;
	lookAtVector.y = 0;
	lookAtVector.z = -1;
	up.x = 0;
	up.y = 1;
	up.z = 0;
	totalXRot = 0.0f;
}

void moveCamera(){
	if(glutKeyIsDown('w')){
		vec3 moveVec = lookAtVector;
		//if(!freeCam)
			//moveVec.y  = 0.25;
		cam = VectorAdd(cam,ScalarMult(moveVec,0.3));
		if(!freeCam)
			cam.y = 0.25;
		//lookAtPoint = VectorAdd(ScalarMult(Normalize(VectorSub(lookAtPoint, cam)),0.3),lookAtPoint);
	}
	if(glutKeyIsDown('s')){
		vec3 moveVec = lookAtVector;
		//moveVec.y  = 0.25;
		cam = VectorSub(cam,ScalarMult(moveVec,0.3));
		if(!freeCam)
			cam.y = 0.25;
		//lookAtPoint = VectorSub(lookAtPoint,ScalarMult(Normalize(VectorSub(lookAtPoint, cam)),0.3));
	}
	if(glutKeyIsDown('a')){
		//vec3 left = Normalize(CrossProduct(VectorSub(cam,lookAtPoint),up));
		vec3 left = Normalize(CrossProduct(up, lookAtVector));
		//left.y = 0.25;
		cam = VectorAdd(cam, ScalarMult(left,0.3));
		//lookAtPoint = VectorAdd(ScalarMult(left,0.3),lookAtPoint);

	}
	if(glutKeyIsDown('d')){
		//vec3 right = Normalize(CrossProduct(up,VectorSub(cam,lookAtPoint)));
		vec3 right = Normalize(CrossProduct(lookAtVector,up));
		//right.y = 0.25;
		cam = VectorAdd(cam, ScalarMult(right,0.3));
		//lookAtPoint = VectorAdd(ScalarMult(right,0.3),lookAtPoint);
	}
	if(glutKeyIsDown('r')){
		resetCamera();
	}

	if(glutKeyIsDown('t')){
		/*if(freeCam)
			freeCam = false;
		else
			freeCam = true;*/
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

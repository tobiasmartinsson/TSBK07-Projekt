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

mat4 projectionMatrix;

// Create Model and upload to GPU:

/*	Model* model = LoadDataToModel(
vertexArray,
normalArray,
texCoordArray,
NULL,
indexArray,
vertexCount,
triangleCount*3);

return model;
}*/

GLfloat groundMatrix[] = {
	-0.5, 0, 0.5,
	-0.5, 0, -0.5,
	0.5, 0, -0.5,

	0.5, 0, -0.5,
	-0.5, 0, 0.5,
	0.5, 0, 0.5
};

GLfloat groundTextureCoord[]={
	0.0f,10.0f,
	0.0f,0.0f,
	10.0f,0.0f,
	10.0f,0.0f,
	0.0f,10.0f,
	10.0f,10.0f
};

GLfloat groundNormal[] = {
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	0, 1, 0,
	0, 1, 0,
	0, 1, 0
};

// vertex array object
Model *m;

// Reference to shader program
GLuint program;

GLfloat angleY;
GLfloat angleX ;

mat4 camTrans;

mat4 groundTransform;

GLuint groundTex;


// vertex array object
unsigned int vertexArrayObjID;
unsigned int vertexBufferObjID;
unsigned int groundTexCoordBufferObjID;
unsigned int groundNormalBufferObjID;


void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	LoadTGATextureSimple("grass.tga", &groundTex);

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);
	angleY= 0.0f;
	angleX = 0.0f;
	camTrans = T(0,0,0);

	// Load and compile shader
	program = loadShaders("main_project.vert", "main_project.frag");
	glUseProgram(program);
	printError("init shader");




	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);





	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &groundNormalBufferObjID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, groundTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
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
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	mat4 total, modelView, camMatrix;
	printError("pre display");

	glUseProgram(program);

	//glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	//glActiveTexture(GL_TEXTURE0);


	// Build matrix

	vec3 cam = {0, 0, 0};
	vec3 lookAtPoint = {0, 0, -1};
	camMatrix = lookAt(cam.x, cam.y, cam.z,
		lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
		0.0, 1.0, 0.0);
		camMatrix = Mult(Ry(angleY),camMatrix);
		camMatrix = Mult(Rx(angleX),camMatrix);
		camMatrix = Mult(camTrans,camMatrix);
		modelView = IdentityMatrix();
		total = Mult(camMatrix, modelView);
		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
		//glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m);


		groundTransform = IdentityMatrix();

		//groundTransform = Mult(Rx(M_PI/2), groundTransform);
		groundTransform = Mult(T(0,-5,-2),groundTransform);
		groundTransform = Mult(camMatrix, groundTransform);
		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, groundTransform.m);
		glBindVertexArray(vertexArrayObjID);	// Select VAO
		glDrawArrays(GL_TRIANGLES, 0, 2*3);	// draw object


		glutSwapBuffers();
	}

	void moveCamera(){
		if(glutKeyIsDown('i')){
			//camMatrix = Mult(Rx(0.1),camMatrix);
			angleX -= 0.01;
		}
		if(glutKeyIsDown('k')){
			//camMatrix = Mult(Rx(-0.1),camMatrix);
			angleX += 0.01;
		}
		if(glutKeyIsDown('j')){
			//camMatrix = Mult(Ry(-0.1),camMatrix);
			angleY+= 0.01;
		}
		if(glutKeyIsDown('l')){
			//camMatrix = Mult(Ry(0.1),camMatrix,);
			angleY-= 0.01;
		}
		if(glutKeyIsDown('w')){
			//cameraPos = Mult(cameraPos,T(0,0,0.5));
			camTrans = Mult(T(0,0,0.3),camTrans);
		}
		if(glutKeyIsDown('s')){
			//cameraPos = Mult(cameraPos,T(0,0,-0.5));
			camTrans = Mult(T(0,0,-0.3),camTrans);
		}
		if(glutKeyIsDown('a')){
			//cameraPos = Mult(cameraPos,T(0.5,0,0));
			camTrans = Mult(T(0.3,0,0),camTrans);
		}
		if(glutKeyIsDown('d')){
			//cameraPos = Mult(cameraPos,T(-0.5,0,0));
			camTrans = Mult(T(-0.3,0,0),camTrans);
		}
		if(glutKeyIsDown('r')){
			//cameraPos = Mult(cameraPos,T(-0.5,0,0));
			camTrans = T(0,0,0);
			angleY = 0.0f;
			angleX = 0.0f;
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
		printf("%d %d\n", x, y);
		if(!glutKeyIsDown('b')){
			if((x != prevX) || (y != prevY)){
				if((prevX - x) < 0){
					angleY += 0.01;
				}
				else if ((prevX - x) > 0){
					angleY -= 0.01;
				}
				if((prevY - y) < 0){
					angleX += 0.01;
				}
				else if((prevY - y) > 0){
					angleX -= 0.01;
				}
				prevX = x;
				prevY = y;

				glutWarpPointer(300, 300);
			}
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

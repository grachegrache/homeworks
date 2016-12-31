#include "gl_io_func.h"
#include "camera.h"
#include "texture.h"

class camera cam;
class Texture texture;

bool mouseClick = false;

int lastx;
int lasty;

#define SLICES 32
#define STATCKS 32

void display() {
	float LightPosition0[4] = { 0,5,-2.5,0 };    //directional light 0
	GLfloat value[9];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
	glLoadIdentity();

	if (cam.angle_v < M_PI / 2)
		gluLookAt(cam.x, cam.y, cam.z, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	else if (cam.angle_v < M_PI / 2 * 3)
		gluLookAt(cam.x, cam.y, cam.z, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
	else
		gluLookAt(cam.x, cam.y, cam.z, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition0);  //set light direction

	glEnable(GL_LIGHTING);

	//body
	value[0] = 0; value[1] = 0.8; value[2] = 0;
	value[3] = 0; value[4] = 0; value[5] = 0;
	value[6] = 0.5; value[7] = 0.3; value[8] = 0.5;
	drawShape(value, SHAPE_SPHERE, texture._textureId[0]);

	value[0] = 0; value[1] = 0.8; value[2] = 0;
	value[3] = 0; value[4] = 0.2; value[5] = 0;
	value[6] = 0.5;
	drawShape(value, SHAPE_CYLINDER, texture._textureId[0]);

	value[0] = 0; value[1] = 0.2; value[2] = 0;
	value[3] = 0; value[4] = -0.2; value[5] = 0;
	value[6] = 0.5;
	drawShape(value, SHAPE_CYLINDER, texture._textureId[1]);

	value[0] = 0; value[1] = -0.2; value[2] = 0;
	value[3] = 0; value[4] = 0; value[5] = 0;
	value[6] = 0.5; value[7] = 0.25; value[8] = 0.5;
	drawShape(value, SHAPE_SPHERE, texture._textureId[1]);

	//face
	value[0] = 0; value[1] = 0.65; value[2] = 0;
	value[3] = 0; value[4] = 0.55; value[5] = 0;
	value[6] = 0.51;
	drawShape(value, SHAPE_CYLINDER, texture._textureId[2]);

	value[0] = 0.24; value[1] = 0.6; value[2] = -0.5;
	value[3] = 0.24; value[4] = 0.6; value[5] = -0.55;
	value[6] = 0.26;
	drawShape(value, SHAPE_CYLINDER, texture._textureId[4]);

	value[0] = -0.24; value[1] = 0.6; value[2] = -0.5;
	value[3] = -0.24; value[4] = 0.6; value[5] = -0.55;
	value[6] = 0.26;
	drawShape(value, SHAPE_CYLINDER, texture._textureId[4]);

	value[0] = 0.24; value[1] = 0.6; value[2] = -0.55;
	value[3] = 0.24; value[4] = 0.6; value[5] = -0.551;
	value[6] = 0.2;
	drawShape(value, SHAPE_CYLINDER, texture._textureId[3]);

	value[0] = -0.24; value[1] = 0.6; value[2] = -0.55;
	value[3] = -0.24; value[4] = 0.6; value[5] = -0.551;
	value[6] = 0.2;
	drawShape(value, SHAPE_CYLINDER, texture._textureId[3]);

	//arm
	value[0] = 0.4; value[1] = 0.1; value[2] = 0;
	value[3] = 0.8; value[4] = -0.3; value[5] = 0;
	value[6] = 0.05;
	drawShape(value, SHAPE_CYLINDER, texture._textureId[0]);

	value[0] = 0.8; value[1] = -0.3; value[2] = 0;
	value[3] = 0; value[4] = 0; value[5] = 0;
	value[6] = 0.1; value[7] = 0.1; value[8] = 0.1;
	drawShape(value, SHAPE_SPHERE, texture._textureId[2]);

	value[0] = -0.4; value[1] = 0.1; value[2] = 0;
	value[3] = -0.8; value[4] = -0.3; value[5] = 0;
	value[6] = 0.05;
	drawShape(value, SHAPE_CYLINDER, texture._textureId[0]);

	value[0] = -0.8; value[1] = -0.3; value[2] = 0;
	value[3] = 0; value[4] = 0; value[5] = 0;
	value[6] = 0.1; value[7] = 0.1; value[8] = 0.1;
	drawShape(value, SHAPE_SPHERE, texture._textureId[2]);

	//reg
	value[0] = 0.25; value[1] = -0.2; value[2] = 0;
	value[3] = 0.25; value[4] = -0.6; value[5] = 0;
	value[6] = 0.05;
	drawShape(value, SHAPE_CYLINDER, texture._textureId[0]);

	value[0] = 0.25; value[1] = -0.6; value[2] = -0.1;
	value[3] = 0; value[4] = 0; value[5] = 0;
	value[6] = 0.1; value[7] = 0.05; value[8] = 0.15;
	drawShape(value, SHAPE_SPHERE, texture._textureId[2]);

	value[0] = -0.25; value[1] = -0.2; value[2] = 0;
	value[3] = -0.25; value[4] = -0.6; value[5] = 0;
	value[6] = 0.05;
	drawShape(value, SHAPE_CYLINDER, texture._textureId[0]);

	value[0] = -0.25; value[1] = -0.6; value[2] = -0.1;
	value[3] = 0; value[4] = 0; value[5] = 0;
	value[6] = 0.1; value[7] = 0.05; value[8] = 0.15;
	drawShape(value, SHAPE_SPHERE, texture._textureId[2]);

	glDisable(GL_LIGHTING);

	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)

}

void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
											   // Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
								  // Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 1.0f, 1000.0f);

}

void keyboardFunc(unsigned char KeyPressed, int X, int Y) {
	switch (KeyPressed) {
	case 27:
		exit(0);
	case 'z'://zoom out
	case 'Z':
		cam.setDistance(0.1); break;
	case 'a'://zoom in
	case 'A':
		cam.setDistance(-0.1); break;
	}

	glutPostRedisplay();
}

void mouseFunc(int btn, int state, int x, int y)
{
	switch (state) {
	case GLUT_DOWN:
		mouseClick = true;
		break;
	case GLUT_UP:
		mouseClick = false;
		break;
	}

}

void motionFunc(int x, int y)
{
	float movx = lastx - x;
	float movy = lasty - y;

	lastx = x;
	lasty = y;

	if (mouseClick) {
		cam.setLocation(movx, movy);
		glutPostRedisplay();

	}
}

/*
* sphere
* transform
* rotate
* scale
*
* cylinder
* first point
* second point
* radius
*
*/
void drawShape(GLfloat value[], int shape, GLuint texture) {
	glPushMatrix();

	GLUquadricObj *quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, (GLenum)GLU_FILL);
	gluQuadricNormals(quadric, (GLenum)GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glBindTexture(GL_TEXTURE_2D, texture);

	switch (shape) {
	case SHAPE_SPHERE:
		glTranslatef(value[0], value[1], value[2]);

		// changing in transformation matrix.

		glRotatef(value[3], 1.0, 0.0, 0.0);// rotation about X axis
		glRotatef(value[4], 0.0, 1.0, 0.0);// rotation about Y axis
		glRotatef(value[5], 0.0, 0.0, 1.0);// rotation about Z axis

		glScalef(value[6], value[7], value[8]);// scaling transfomation 

		gluSphere(quadric, 1, SLICES, STATCKS);
		break;
	case SHAPE_CYLINDER:

		gluQuadricNormals(quadric, GLU_SMOOTH);
		renderCylinder(value[0], value[1], value[2], value[3], value[4], value[5], value[6], SLICES, quadric);
		break;
	default:
		break;
	}

	glPopMatrix();
}

void renderCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions, GLUquadricObj *quadric)
{
	float vx = x2 - x1;
	float vy = y2 - y1;
	float vz = z2 - z1;

	//handle the degenerate case of z1 == z2 with an approximation
	if (vz == 0)
		vz = .0001;

	float v = sqrt(vx*vx + vy*vy + vz*vz);
	float ax = 57.2957795*acos(vz / v);
	if (vz < 0.0)
		ax = -ax;
	float rx = -vy*vz;
	float ry = vx*vz;
	glPushMatrix();

	//draw the cylinder body
	glTranslatef(x1, y1, z1);
	glRotatef(ax, rx, ry, 0.0);
	gluQuadricOrientation(quadric, GLU_OUTSIDE);
	gluCylinder(quadric, radius, radius, v, subdivisions, 1);

	//draw the first cap
	gluQuadricOrientation(quadric, GLU_INSIDE);
	gluDisk(quadric, 0.0, radius, subdivisions, 1);
	glTranslatef(0, 0, v);

	//draw the second cap
	gluQuadricOrientation(quadric, GLU_OUTSIDE);
	gluDisk(quadric, 0.0, radius, subdivisions, 1);
	glPopMatrix();
}

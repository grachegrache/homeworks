#include <glut.h>

#include <Windows.h>
#include "gl_io_func.h"
#include "texture.h"
#include "camera.h"

extern class camera cam;
extern class Texture texture;

void initGL();

int main(int argc, char** argv) {	
	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Enable double buffered mode
	glutInitWindowSize(640, 480);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow("minions");          // Create window with the given title

	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);

	initGL();                       // Our own OpenGL initialization
	
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}

void initGL() {
							 //light properties
	float LightAmbient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float LightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float LightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float ModelAmbient[4] = { 0.6f, 0.6f, 0.6f, 1.0f };

	//set up perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 640 / 480, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, 640, 480);

	//set state variables
	glClearColor(1, 1, 1, 0);
	glEnable(GL_DEPTH_TEST);
	
	//set lighting properties
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ModelAmbient);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
	glEnable(GL_LIGHT0);

	glShadeModel(GL_SMOOTH);   // Enable smooth shading

	texture.initRendering();

}

#pragma once
#include <glut.h>

#define SHAPE_SPHERE 1
#define SHAPE_CYLINDER 2

void display();
void reshape(GLsizei, GLsizei);
void keyboardFunc(unsigned char, int, int);
void mouseFunc(int btn, int state, int x, int y);
void motionFunc(int x, int y);
void drawShape(GLfloat[], int, GLuint);
void renderCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int subdivisions, GLUquadricObj *quadric);

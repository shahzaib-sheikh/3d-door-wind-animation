#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "glut.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define PROJECT_NAME "3D Door Animation"

float doorAngle = 0;
float inc = 5;


float ex = 0;
float ey = 0;
float ez = 0;
float cx = 2;
float cy = 4;
float cz = 2;


void drawDoor(float width, float height, float breadth) {


	//lower base
	glColor3f(0.1, 0.9, 0.5);
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, breadth / 2);
	glVertex3f(width, 0, breadth / 2);
	glVertex3f(width, 0, -breadth / 2);
	glVertex3f(0, 0, -breadth / 2);

	glEnd();

	//upperbase

	glColor3f(0.5, 0.9, 0.1);
	glBegin(GL_POLYGON);
	glVertex3f(0, height, breadth / 2);
	glVertex3f(width, height, breadth / 2);
	glVertex3f(width, height, -breadth / 2);
	glVertex3f(0, height, -breadth / 2);
	glEnd();


	//left side
	glColor3f(0.9, 0.9, 0.1);
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, breadth / 2);
	glVertex3f(0, 0, -breadth / 2);
	glVertex3f(0, height, -breadth / 2);
	glVertex3f(0, height, breadth / 2);
	glEnd();

	//right side
	glColor3f(0.9, 0.3, 0.1);
	glBegin(GL_POLYGON);
	glVertex3f(width, 0, breadth / 2);
	glVertex3f(width, 0, -breadth / 2);
	glVertex3f(width, height, -breadth / 2);
	glVertex3f(width, height, breadth / 2);
	glEnd();

	//front side
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, breadth / 2);
	glVertex3f(width, 0, breadth / 2);
	glVertex3f(width, height, breadth / 2);
	glVertex3f(0, height, breadth / 2);
	glEnd();

	//back side
	glColor3f(0.5, 0.1, 0.5);
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, -breadth / 2);
	glVertex3f(width, 0, -breadth / 2);
	glVertex3f(width, height, -breadth / 2);
	glVertex3f(0, height, -breadth / 2);
	glEnd();

}

void drawOrigin()
{
	glBegin(GL_LINES);
	glVertex3f(-3, 0, 0);
	glVertex3f(3, 0, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, -3, 0);
	glVertex3f(0, 3, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, 0, -3);
	glVertex3f(0, 0, 3);
	glEnd();
}



void mainDisplayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0.3, 0.3, 0);
	gluLookAt(ex, ey, ez, cx, cy, cz, 0, 1, 0); // we need a model that have prominent back and front faces to better understand this command...
	//gluPerspective();

	///glutWireCube(.5);

	drawOrigin();
	glPushMatrix();
	glRotatef(doorAngle, 0, 1, 0);
	drawDoor(0.3, 0.7, .02);
	glPopMatrix();

	glutSwapBuffers();
}
float initialAngularVelocity = 0;
float initialAngle = 0;
float previousTick = 0;
float currentTick = 0;
float calculateAngle()
{

	float timeInSec = (currentTick-previousTick)/1000;
	float angularAcceleration = 1;



	float newAngle = (initialAngularVelocity*timeInSec + (0.5*angularAcceleration*timeInSec*timeInSec)) + initialAngle;
	initialAngularVelocity = initialAngularVelocity + angularAcceleration*timeInSec;
	initialAngle = newAngle;
	return newAngle*(180/M_PI);
}

void timerFunc(int time) {
	previousTick = currentTick;
	currentTick = time;
	if (doorAngle >= 90) {
		inc = -5;
	}
	if (doorAngle <= 0) {
		inc = 5;
	}
	doorAngle = calculateAngle();
	glutPostRedisplay();
	glutTimerFunc(50, timerFunc, time + 50);
	std::cout << "\n\n\n+++++\n";
	std::cout << previousTick;
	std::cout << "\n\n\n+++++\n";
}

void keyboardUpHandler(unsigned char key, int x, int y) {

}

void keyboardHandler(unsigned char key, int x, int y) {
	if(key=='r'|| key == 'R')
	{
		initialAngle = 0;
		initialAngularVelocity = 0;
	}
}

int prex = 0;
int prey = 0;
void mouseHandler(int button, int state, int x, int y) {

	int diffx = 0;
	int diffy = 0;
	if (state == 0) {
		prex = x;
		prey = y;

	}
	else {
		diffx = x - prex;
		diffy = x - prey;
	}


	if (button == 2 && state == 1) {

		ex = 0;
		ey = 0;
		ez = 0;
		cx = 2;
		cy = 4;
		cz = 2;
	}
	else if (button == 0 && state == 1) {
		cx += diffx * 0.5;
		cy += diffy *0.5;
		cz++;
	}
	std::cout << "diffx" << diffx << "\n";
	std::cout << "diffy" << diffy << "\n";
	std::cout << "cx" << cx << "\n";
	std::cout << "cy" << cy << "\n";
	std::cout << "cz" << cz << "\n";

}
void passiveFunc(int x, int y) {
	x /= 53;
	y /= 53;

	cx = x - 7.5;
	cy = y - 7.5;

	glutPostRedisplay();
}
void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}
int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow(PROJECT_NAME);

	glutDisplayFunc(mainDisplayFunc);
	glutKeyboardFunc(keyboardHandler);
	glutKeyboardUpFunc(keyboardUpHandler);
	glutMouseFunc(mouseHandler);
	glutPassiveMotionFunc(passiveFunc);
	glutTimerFunc(50, timerFunc, 0);

	initGL();
	glutMainLoop();
	return 0;
}


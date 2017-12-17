#define _USE_MATH_DEFINES

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <glut.h>
#include <Windows.h>
#include "glut.h"
#include "RGBA.h"
#include "wav.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define PROJECT_NAME "3D Door Animation"

float doorAngle = M_PI / 2;
float inc = 5;


float ex = 0;
float ey = 0;
float ez = 0;
float cx = 2;
float cy = 4;
float cz = 2;

RGBApixmap doorFront;
RGBApixmap doorBack;

void drawDoor(float width, float height, float breadth) {
	glEnable(GL_TEXTURE_2D);
	//front side
	glBindTexture(GL_TEXTURE_2D,2001);
	glBegin(GL_POLYGON);

	glTexCoord2f(0,0);
	glVertex3f(0, 0, breadth / 2);

	glTexCoord2f(1,0);
	glVertex3f(width, 0, breadth / 2);

	glTexCoord2f(1,1);
	glVertex3f(width, height, breadth / 2);

	
	glTexCoord2f(0,1);
	glVertex3f(0, height, breadth / 2);
	glEnd();

	//back side
	glBindTexture(GL_TEXTURE_2D,2001);
	glBegin(GL_POLYGON);
	
	glTexCoord2f(0,0);
	glVertex3f(0, 0, -breadth / 2);

	
	glTexCoord2f(1,0);
	glVertex3f(width, 0, -breadth / 2);

	
	glTexCoord2f(1,1);
	glVertex3f(width, height, -breadth / 2);

	
	glTexCoord2f(0,1);
	glVertex3f(0, height, -breadth / 2);
	glEnd();
	glDisable(GL_TEXTURE_2D);

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
	glRotatef(doorAngle * (180 / M_PI), 0, 1, 0);
	drawDoor(0.3, 0.7, .02);
	glPopMatrix();

	glutSwapBuffers();
}
float initialAngularVelocity = 0;

float initialAngularAcceleration = 0;
float initialAngle = M_PI / 2;
float previousTick = 0;
float currentTick = 0;
float angularMomentum = 0;
float forcePerUnitAreaOfWind =10;
float massOfGate = 30;
float widthOfDoor = 0.76;
float heightOfDoor = 1.8;
float friction = 2;
float l = widthOfDoor/2;
float momentOfInertia = (4/3) * massOfGate * l * l;
float scaleOfOscilation = 0.003;


float calculateAcceleration()
{ 
	float l = widthOfDoor/2;
	float momentOfInertia = (4/3) * massOfGate * l * l;


	float effectiveArea = (widthOfDoor * sin(initialAngle)) * heightOfDoor;
	float forceOnEffectiveArea = forcePerUnitAreaOfWind * effectiveArea;


	float forceDueToWind = l * forceOnEffectiveArea * sin(initialAngle);   //torque
	forceDueToWind-= friction;
	return forceDueToWind / momentOfInertia; 
}
float calculateAngle()
{

	float timeInSec = (currentTick-previousTick)/1000;
	float angularAcceleration = calculateAcceleration();
	initialAngularAcceleration=angularAcceleration;
	float newDisplacement=(initialAngularVelocity*timeInSec + (0.5*angularAcceleration*timeInSec*timeInSec));
	if(newDisplacement<0){
		newDisplacement = 0; 
	}else{
		newDisplacement*=-1;
	}
	float newAngle = newDisplacement + initialAngle;  //displacement is clockwise 
	                                                                                                                      //thats why negative sign...
	initialAngularVelocity = initialAngularVelocity + angularAcceleration*timeInSec;
	initialAngle = newAngle;
	return newAngle;
}
void clear() {
    COORD topLeft  = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}
void timerFunc(int time) {
	previousTick = currentTick;
	currentTick = time;
	if (doorAngle <= 0) {
	angularMomentum = momentOfInertia * initialAngularVelocity;
	PlayForegroundSound("close_door_1.wav");
	Sleep(3333);
		initialAngularVelocity=0;
		initialAngle=M_PI/2;
		doorAngle=M_PI/2;
	doorAngle = calculateAngle();

	}else if(initialAngularVelocity<0){
		doorAngle+=sin((float)time)*scaleOfOscilation;
	}else{
	doorAngle = calculateAngle();

	}
	glutPostRedisplay();
	glutTimerFunc(50, timerFunc, time + 50);
	//std::cout << "\n\n\n+++++\n";
	//std::cout << previousTick;
	//std::cout << "\n\n\n+++++\n";
	system("CLS");
	clear();
	std::cout << "\n========== Door Animation ==========";
	std::cout << "\nMass of Door = ";
	std::cout << massOfGate;
		std::cout << " Kilogram";

	std::cout << "\nWidth of Door = ";
	std::cout << widthOfDoor;
		std::cout << " Meter";

	std::cout << "\nHeight of Door = ";
	std::cout << heightOfDoor;
		std::cout << " Meter";

	std::cout << "\nForce per unit Area of Wind = ";
	std::cout << forcePerUnitAreaOfWind;
		std::cout << " Newton/meter^2";

	std::cout << "\nDoor Angle w.r.t x-axis = ";
	std::cout << doorAngle*(180/M_PI);
		std::cout << " degrees";
	
	std::cout << "\nWind Angle w.r.t x-axis = ";
		std::cout << "0 degrees";
	
	
	
	
	std::cout << "\nDoor Friction = ";
	std::cout << friction;
	std::cout << " Newton*Meter";
	
		std::cout << "\nAngular Velocity = ";
		std::cout << initialAngularVelocity;
	std::cout << " Radians/Second";
		std::cout << "\nAngular Acceleration = ";
		std::cout << initialAngularAcceleration;
	std::cout << " Radians/Second^2";
	
}

void reshape(int newWidth , int newHeight)
{
	if(newHeight == 0)
		newHeight = 1;

	float aspectRatio = (float)(newWidth / newHeight);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,newWidth,newHeight);
	gluPerspective(60,aspectRatio,0,1000);
	glMatrixMode(GL_MODELVIEW);

}

void keyboardUpHandler(unsigned char key, int x, int y) {

}

void keyboardHandler(unsigned char key, int x, int y) {
	if(key=='r'|| key == 'R')
	{
		initialAngle = 0;
		initialAngularVelocity = 0;
	}else if(key=='f'||key=='F'){
		friction+=0.1;
	}else if(key=='g'||key=='G'){
		friction-=0.1;
	}else if(key=='m'||key=='M'){
		massOfGate+=0.1;
	}else if(key==','||key=='<'){
		massOfGate-=0.1;
	}else if(key=='w'||key=='W'){
		forcePerUnitAreaOfWind+=0.1;
	}else if(key=='e'||key=='E'){
		forcePerUnitAreaOfWind-=0.1;
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


  PlayBackgroundSound("wind01.wav"); 
}
int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow(PROJECT_NAME);

	doorBack.readBMPFile("doorBack.bmp",0);
	doorBack.setTexture(2001);

	doorFront.readBMPFile("doorFront.bmp",0);
	doorFront.setTexture(2002);

	glutDisplayFunc(mainDisplayFunc);
	glutKeyboardFunc(keyboardHandler);
	glutKeyboardUpFunc(keyboardUpHandler);
	glutMouseFunc(mouseHandler);
	glutPassiveMotionFunc(passiveFunc);
	glutTimerFunc(50, timerFunc, 0);
//	glutReshapeFunc(reshape);
	
	initGL();
	glutMainLoop();
	return 0;
}


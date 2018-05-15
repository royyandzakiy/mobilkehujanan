#pragma once
/*
* OGL02Animation.cpp: 3D Shapes with animation
*/
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <vector>
#include <time.h>
#include "MP3.h"
#include <stdio.h>
#include <string.h>
#include <ctime>
#define PI 3.14159265f
using namespace std;

/* Global variables */
char title[] = "3D Shapes with animation";
int refreshMills = 15;        // refresh interval in milliseconds [NEW]

clock_t start;
double duration;
boolean printScore = true;

GLfloat cubeSize = 5.0f;
GLfloat cubeAngle = 0.0f;     // Rotational angle for cube [NEW]
GLfloat cubePosX = 0.0f;
GLfloat cubePosY = 5.0f; // posisi awal
GLfloat cubePosZ = -50.0f;
GLfloat cubeSpeedX = 0.0f;
GLfloat cubeSpeedY = 0.0f;
GLfloat cubeSpeedZ = 0.0f;
GLfloat cubeSpeedRot = -0.5f;

GLfloat cylinderSize = 5.0f;
GLfloat cylinderAngle = 0.0f;     // Rotational angle for cylinder [NEW]
GLfloat cylinderPosX = 0.0f;
GLfloat cylinderPosY = 5.0f; // posisi awal
GLfloat cylinderPosZ = -50.0f;
GLfloat cylinderSpeedX = 0.0f;
GLfloat cylinderSpeedY = 0.0f;
GLfloat cylinderSpeedZ = 0.0f;
GLfloat cylinderSpeedRot = -0.5f;

GLfloat ballRadius = 0.5f;   // Radius of the bouncing ball
GLfloat ballX = 0.0f;         // Ball's center (x, y) position
GLfloat ballY = 0.0f;

// WINDOW STUFF
float minX = -(20.0f + cubeSize * 3);
float maxX = -minX;
float minY = (20.0 + cubeSize * 3);
float maxY = -minY;

// FIND FURTHEST CUBE
// check if it's gone yet
int cubeMaxY;
float yValMax = 0.0f;
/* Initialize OpenGL Graphics */

void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

class Entity {
public:
	GLfloat size, posX, posY, posZ, angle, speedX, speedY, speedZ, speedRot;
	Entity(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	void move();
	virtual void draw() = 0;
	void cubeDie();
};

Entity::Entity(GLfloat _size, GLfloat _posX, GLfloat _posY, GLfloat _posZ, GLfloat _angle, GLfloat _speedX, GLfloat _speedY, GLfloat _speedZ, GLfloat _speedRot) {
	size = _size;
	posX = _posX;
	posY = _posY;
	posZ = _posZ;
	angle = _angle;
	speedX = _speedX;
	speedY = _speedY;
	speedZ = _speedZ;
	speedRot = _speedRot;
}

void Entity::move() {
	posX -= speedX;
	posY -= speedY;
	posZ -= speedZ;
	angle += speedRot;
}

class Cube : public Entity {
public:
	Cube();
	Cube(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	void draw();
};

Cube::Cube() : Entity(0.1f * (rand() % 10 + 4), 20.0f * (rand() % 10 * -rand() % 1 + 1), 15.0f * (rand() % 5 + 1), -50.0f, 1.0f * (rand() % 10 + 1), 0.0f, 0.1f * (rand() % 5 + 1), 0.0f, -0.1f * (rand() % 10 + 1)) {
}

Cube::Cube(GLfloat _size, GLfloat _posX, GLfloat _posY, GLfloat _posZ, GLfloat _angle, GLfloat _speedX, GLfloat _speedY, GLfloat _speedZ, GLfloat _speedRot) : Entity(_size, _posX, _posY, _posZ, _angle, _speedX, _speedY, _speedZ, _speedRot) {
	// nothing to do...
}

void Cube::draw() {
	glLoadIdentity();                 // Reset the model-view matrix
	glTranslatef(posX, posY, posZ);   // Move right and into the screen
	glRotatef(angle, 1.0f, 1.0f, 1.0f);  // Rotate about (1,1,1)-axis [NEW
	//glRotatef(0.0f, 1.0f, 1.0f, 1.0f);  // Rotate about (1,1,1)-axis [NEW
	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
									  // Top face (y = 1.0f)
									  // Define vertices in counter-clockwise (CCW) order with normal pointing out

	glColor3f(0.0f, 0.5f, 0.0f);      // Green
	glVertex3f(size, size, -size);
	glVertex3f(-size, size, -size);
	glVertex3f(-size, size, size*2.5);
	glVertex3f(size, size, size * 1.5);

	// Bottom face (y = -size)
	glColor3f(1.0f, 0.5f, 0.0f);     // Orange
	glVertex3f(size, -size, size * 1.5); 
	glVertex3f(-size, -size, size*2.5);
	glVertex3f(-size, -size, -size);
	glVertex3f(size, -size, -size);

	// Front face  (z = size)
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(size, size, size*1.5);
	glVertex3f(-size, size, size*2.5);
	glVertex3f(-size, -size, size*2.5);
	glVertex3f(size, -size, size*1.5);

	// Back face (z = -size)
	glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
	glVertex3f(size, -size, -size);
	glVertex3f(-size, -size, -size);
	glVertex3f(-size, size, -size);
	glVertex3f(size, size, -size);

	// Left face (x = -size)
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(-size, size, size*2.5);
	glVertex3f(-size, size, -size);
	glVertex3f(-size, -size, -size);
	glVertex3f(-size, -size, size*2.5);

	// Right face (x = size)
	glColor3f(0.5f, 0.0f, 0.5f);     // Magenta
	glVertex3f(size, size, -size);
	glVertex3f(size, size, size*1.5);
	glVertex3f(size, -size, size *1.5);
	glVertex3f(size, -size, -size);
	glEnd();  // End of drawing color-cube

	move();
}

class Cylinder : public Entity {
public:
	Cylinder();
	Cylinder(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	void draw();
};

Cylinder::Cylinder() : Entity(0.1f * (rand() % 10 + 4), 20.0f * (rand() % 10 * -rand() % 1 + 1), 15.0f * (rand() % 5 + 1), -50.0f, 1.0f * (rand() % 10 + 1), 0.0f, 0.1f * (rand() % 5 + 1), 0.0f, -0.1f * (rand() % 10 + 1)) {
}

Cylinder::Cylinder(GLfloat _size, GLfloat _posX, GLfloat _posY, GLfloat _posZ, GLfloat _angle, GLfloat _speedX, GLfloat _speedY, GLfloat _speedZ, GLfloat _speedRot) : Entity(_size, _posX, _posY, _posZ, _angle, _speedX, _speedY, _speedZ, _speedRot) {
	// nothing to do...
}

void Cylinder::draw() {
	glLoadIdentity();                 // Reset the model-view matrix
	glTranslatef(posX, posY, posZ);   // Move right and into the screen
	glRotatef(angle, 1.0f, 1.0f, 1.0f);  // Rotate about (1,1,1)-axis [NEW
	//glRotatef(0.0f, 1.0f, 1.0f, 1.0f);  // Rotate about (1,1,1)-axis [NEW
	
	// void gluCylinder(GLUquadric* quad,
	// 	GLdouble base,
	// 	GLdouble top,
	// 	GLdouble height,
	// 	GLint slices,
	// 	GLint stacks);

	glClear(GL_COLOR_BUFFER_BIT);

	// CIRCLE DEPAN
	glTranslatef(ballX, ballY, 0.0f);  // Translate to (xPos, yPos)
									   // Use triangular segments to form a circle
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, 0.0f);       // Center of circle
	int numSegments = 100;
	GLfloat angle;
	int c;
	for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
		if (i < numSegments / 2)
			c = i;

		glColor3f(0, 0, 1);  // Blue
		angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
		glVertex2f(cos(angle) * size/5.0f, sin(angle) * size / 5.0f);
	}
	glEnd();
	
	// CYLINDER
	glColor3f(0, 1, 0);
	GLUquadricObj *obj = gluNewQuadric();
	gluCylinder(obj, size/5.0f, size / 5.0f, 1.0f, 30, 30);

	move();
}

Cube cube1(cubeSize, cubePosX, cubePosY, cubePosZ - 10.0f, cubeAngle, cubeSpeedX, cubeSpeedY, cubeSpeedZ, cubeSpeedRot);
Cylinder wheel1(cylinderSize, cylinderPosX, cylinderPosY, cylinderPosZ+10.0f, cylinderAngle, cylinderSpeedX, cylinderSpeedY, cylinderSpeedZ, cylinderSpeedRot);

/* -----------SETTINGS--------------- */
// DON'T TOUCH!
void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective 
}

/*
void initMusic() {
// this is used to hold the user input text
char buffer[256];

// a flag to see if the user wants to quit yet
int done = 0;

// initialise DirectShow
Mp3Init();

// load an mp3 file
string musicFile = "tune.mp3";

char *a = new char[musicFile.size() + 1];
a[musicFile.size()] = 0;
memcpy(a, musicFile.c_str(), musicFile.size());

Mp3Load(a);
Mp3Play();
}
//*/

/* Handler for window-repaint event. Called back when the window first appears and
whenever the window needs to be re-painted. */
void display() {
	GLfloat x, y, z, x_min, x_max, y_min, y_max, z_min, z_max;

	// Draw3D
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	/* OBJETS_START */
	wheel1.draw();
	cube1.draw();
	/* OBJETS_END */

	duration = (clock() - start) / (double)CLOCKS_PER_SEC;
	ClearScreen();

	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

/* Called back when timer expired [NEW] */
void timer(int value) {
	glutPostRedisplay();      // Post re-paint request to activate display()
	glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}



void specialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT:
		//if (pyramid1.posX < maxX) pyramid1.speedX = -pyramid1.power;
		//else pyramid1.speedX = 0;
		break;
	case GLUT_KEY_LEFT:
		//if (pyramid1.posX > minX) pyramid1.speedX = pyramid1.power;
		//else pyramid1.speedX = 0;
		break;
	case GLUT_KEY_UP:
		//if (pyramid1.posY > maxY) pyramid1.speedY = -pyramid1.power;
		//else pyramid1.speedY = 0;
		break;
	case GLUT_KEY_DOWN:
		//if (pyramid1.posY < minY) pyramid1.speedY = pyramid1.power;
		//else pyramid1.speedY = 0;
		break;
	default:
		break;
	}
}

/* Handler for window re-size event. Called back when the window first appears and
whenever the window is re-sized with its new width and height */
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
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	srand(time(NULL));
	// Init Cubes

	printf("START!\n");

	/* OBJECTS_START */
	// ...
	/* OBJECTS_END */

	start = clock();

	glutInit(&argc, argv);            // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(700, 700);   // Set the window's initial width & height
	glutInitWindowPosition(400, 50); // Position the window's initial top-left corner
	glutCreateWindow(title);          // Create window with the given title

	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(reshape);       // Register callback handler for window re-size event
	glutSpecialFunc(specialKeys);

	initGL();                       // Our own OpenGL initialization
									// initMusic();
	glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}
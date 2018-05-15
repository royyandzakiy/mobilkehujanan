//------------------------------------------------------------
/// \file	Main.cpp
/// \author	Rob Bateman
/// \date	9-feb-2005
/// \brief	This shows how to make your code frame rate
///			independant. If you change the window size the 
///			object will still spin at the same speed.
//------------------------------------------------------------

#include <stdlib.h>
#include <GL/glut.h>
#include "FrameTimer.h"

/// a structure to hold a particle
struct Particle {

	/// the current particle position
	float position[3];

	/// the direction of the particle
	float direction[3];

	/// the lifespan
	float life;

	/// constructor
	Particle() {

		// set position
		position[0] = position[2] = 0.0f;
		position[1] = 3.0f;

		// create random direction
		direction[0] = (10000 - rand()%20000)/10000.0f;
		direction[1] = (10000 - rand()%20000)/10000.0f;
		direction[2] = (10000 - rand()%20000)/10000.0f;

		// set random lifespan
		life         = rand()%10000/9500.0f;
	}

	/// pointer to the next particle
	Particle* next;
};

/// the first particle in the linked list
Particle* pList;

//------------------------------------------------------------	NewParticle()
//
void NewParticle() {

	// create new particle and add as first in list
	Particle* p = new Particle;
	p->next = pList;
	pList = p;
}
//------------------------------------------------------------	UpdateParticles()
//
void UpdateParticles(float dt) {

	// traverse all particles and update
	Particle* p = pList;
	while(p) {
		// decrease lifespan
		p->life -= dt;

		// apply gravity
		p->direction[1] -= 9.81f*dt;

		// modify position
		p->position[0] += dt * p->direction[0];
		p->position[1] += dt * p->direction[1];
		p->position[2] += dt * p->direction[2];

		// goto next particle
		p=p->next;
	}
}
//------------------------------------------------------------	RemoveDeadParticles()
//
void RemoveDeadParticles() {

	// iterate over particles
	Particle* curr = pList;
	Particle* prev = 0;

	while (curr) {

		// if dead
		if (curr->life<0) {

			// update the previous pointer to skip over the curr 
			// particle, or just remove the particle if it's the 
			// first in the list that we need to remove.
			//
			if (prev) {
				prev->next = curr->next;
			}
			else {
				pList = curr->next;
			}

			// take temporary reference
			Particle* temp = curr;

			// skip over particle in list
			curr = curr->next;

			// delete particle
			delete temp;
		}
		else {
			// move to next if not removing
			prev = curr;
			curr = curr->next;
		}
	}
}

//------------------------------------------------------------	DrawParticles()
//
void DrawParticles() {

	// iterate over all particles and draw a point
	Particle* curr = pList;

	glPointSize(2);
	glBegin(GL_POINTS);
	while (curr) {
		glVertex3fv(curr->position);
		curr = curr->next;
	}
	glEnd();
}




/// the rotation of the teapot
double g_Rotation=0;

//------------------------------------------------------------	OnReshape()
//
void OnReshape(int w, int h)
{
	if (h==0) {
		h=1;
	}

	// set the drawable region of the window
	glViewport(0,0,w,h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// just use a perspective projection
	gluPerspective(45,(float)w/h,0.1,100);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//------------------------------------------------------------	Draw()
//
void OnDraw() {

	// clear the screen & depth buffer
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	// clear the previous transform
	glLoadIdentity();

	// set the camera position
	gluLookAt(	1,1,10,	//	eye pos
				0,0,0,	//	aim point
				0,1,0);	//	up direction

	DrawParticles();

	// currently we've been drawing to the back buffer, we need
	// to swap the back buffer with the front one to make the image visible
	glutSwapBuffers();
}

//------------------------------------------------------------	OnInit()
//
void OnInit() {
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
}

//------------------------------------------------------------	OnExit()
//
void OnExit() {
}

//------------------------------------------------------------	OnIdle()
//
void OnIdle() {

	// update the frame time
	SortFrameTimer();

	// create a new particle every frame
	NewParticle();

	// update the particle simulation
	UpdateParticles(FrameTime());

	// remove any dead particles
	RemoveDeadParticles();

	// redraw the screen
	glutPostRedisplay();
}


//------------------------------------------------------------	main()
//
int main(int argc,char** argv) {

	// initialise glut
	glutInit(&argc,argv);

	// request a depth buffer, RGBA display mode, and we want double buffering
	glutInitDisplayMode(GLUT_DEPTH|GLUT_RGBA|GLUT_DOUBLE);

	// set the initial window size
	glutInitWindowSize(640,480);

	// create the window
	glutCreateWindow("A basic glut example");

	// set the function to use to draw our scene
	glutDisplayFunc(OnDraw);

	// set the function to handle changes in screen size
	glutReshapeFunc(OnReshape);

	// set the idle callback
	glutIdleFunc(OnIdle);
	
	// run our custom initialisation
	OnInit();

	// set the function to be called when we exit
	atexit(OnExit);

	// initialise the frame timer
	InitFrameTimer();

	// this function runs a while loop to keep the program running.
	glutMainLoop();
	return 0;
}

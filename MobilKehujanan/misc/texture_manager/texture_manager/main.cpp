//------------------------------------------------------------
/// \file	Main.cpp
/// \author	Rob Bateman
/// \date	9-feb-2005
/// \brief	This is an example of how to handle texturing
/// 		within OpenGL. 3 file formats are supported, namely
/// 		tga, bmp and pcx. The texture.cpp and corresponding
/// 		header define a basic management system for the
/// 		textures that are loaded into the application
//------------------------------------------------------------

#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include "texture.h"

/// this will hold the openGL texture object for the loaded texture
unsigned int g_Texture=0;

//------------------------------------------------------------	OnReshape()
//
void OnReshape(int w, int h)
{
	// prevent a division by 0 when minimises
	if (h==0)
		h=1;

	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20,20,20,-20,-20,20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//------------------------------------------------------------	OnDraw()
//
void OnDraw() {

	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	// binds the texture to any geometry about to be rendered
	glBindTexture(GL_TEXTURE_2D,g_Texture);

	// whenever you apply a texture to an object, you will need
	// to specify texture co-ordinates which define the placement
	// of the texture on the polygons. Changing the value of the
	// tex coords changes the placement of the texture...
	//
	GLfloat size = 15.0f;
	glBegin(GL_QUADS);
		// glVertex2f(ke_kanan, ke_bawah)

		glTexCoord3f(0, 0, 0);
		glVertex3f(-size, size, 1.0f);
		glTexCoord3f(0, 1, 0);
		glVertex3f(-size, -size, -1.0f);
		glTexCoord3f(1, 1, 0);
		glVertex3f(size, -size, 1.0f);
		glTexCoord3f(1, 0, 0);
		glVertex3f(size, size, -1.0f);
		//*/

		/*
		// kiri bawah
		glTexCoord2f(0, 0);
		glVertex2f(-15, 15);

		// kiri atas
		glTexCoord2f(0,1);
		glVertex2f(-15,-15);

		// kanan atas
		glTexCoord2f(1,1);
		glVertex2f(15,-15);

		// kanan bawah
		glTexCoord2f(1,0);
		glVertex2f(15,15);
		//*/
	glEnd();

	glutSwapBuffers();
}

//------------------------------------------------------------	OnInit()
//
void OnInit() {

	// enable the depth test and 2D texturing
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective 

	// load texture as compressed
	g_Texture = LoadTexture("LAND.BMP");

	// outputs the size of the texture
	std::cout << "Texture Size=" << (GetTextureSize(g_Texture)/1024.0f) << "Kb"<< std::endl;
}

//------------------------------------------------------------	OnExit()
//
void OnExit() {

	// deletes all loaded texture
	DeleteAllTextures();
}

//------------------------------------------------------------	main()
//
int main(int argc,char** argv) {

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize(640,480);
	glutCreateWindow("A Texture Manager Example");

	glutDisplayFunc(OnDraw);
	glutReshapeFunc(OnReshape);
	
	OnInit();

	atexit(OnExit);

	glutMainLoop();
	return 0;
}

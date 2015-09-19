/*****************************************************************************
        FILE : submit.cpp (csci3260 2015-2016 Assignment 1)
        NOTE : you have to implement functions in this file
*****************************************************************************/
/*****************************************************************************
		Student Information
		Student ID:
		Student Name:
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <gl\glut.h>
#include <windows.h>
#include <math.h>
#include <time.h>

int winWidth = 600;
int winHeight = 600;

float cam_X(0), cam_Y(0), cam_Z(-250);
float cam_ViewX(0), cam_ViewY(0), cam_ViewZ(-500);


void init(void) // All Setup For OpenGL Goes Here
{
	// Light 0 Settings
	static GLfloat light0pos[] = {200.f, 100.f, 400.f, 0.f};
	static GLfloat light0_mat1[] = {0.8, 0.8, 0.8, 1.f};
	static GLfloat light0_diff1[] = {0.9, 0.9, 0.9, 1.f};
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_mat1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);		// Cull all the back face
	glFrontFace(GL_CCW);		// Define Counter-Clockwise as front face

	glEnable(GL_COLOR_MATERIAL); 
}



void display(void) // Here's Where We Do All The Drawing
{
	glClearColor(0.0, 0.0, 0.0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glPushMatrix();
	gluLookAt(cam_X,cam_Y,cam_Z, cam_ViewX, cam_ViewY, cam_ViewZ, 0, 1, 0);
	glTranslatef(0, 0, -550);

	
	
		// A ball  
		// Remove this part and start to add your own drawing
			GLfloat no_mat[] = {0.0,0.0,0.0,1.0};
			GLfloat mat_diffuse[] = {0.1,0.2,0.9,1.0};
			GLfloat mat_specular[] = {0.8,0.8,0.8,1.0};
			GLfloat high_shininess[] = {20.0};
			glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
			glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
			glMaterialfv(GL_FRONT,GL_SHININESS,high_shininess);
			glMaterialfv(GL_FRONT,GL_EMISSION,no_mat);
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

			glColor4f(0.1, 0.4, 0.1, 1.0);
			glutSolidSphere(40, 15, 15);
		//End of A ball  


	// TODO:
	// Draw grounds and objects here
	





	glPopMatrix();
	glutSwapBuffers();
	glFlush();	


	// Refresh the frame
	glutPostRedisplay();
}


void reshape(int w, int h) // Resize the GL Window. w=width, h=height
{
	winWidth = w; winHeight = h;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45, (float)w/(float)h, 250, 900); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


void special(int key, int x, int y) // Handle special keys
{	
	switch (key) 
	{
		case GLUT_KEY_LEFT:
			break;
		case GLUT_KEY_UP:
			break;
	}
}

void keyboard(unsigned char key, int x, int y) // Handle the keyboard events here
{
	switch (key) 
	{
		case '\033'://press 'esc' to quit
		   exit(0);
		   break;
		   // TODO:
		   // Add keyboard control here

	}
}

void mouseclick(int button, int state, int x, int y) // Handle the mouse click events here
{
	printf("Button %d, State %d, Position %d, %d \n", button, state, x, y);
}

void mousemove(int x, int y) // Handle the mouse movement events here
{
	printf("Mouse moved to position %d %d \n", x, y);	
}

void idle()
{
	Sleep(10);
}


void main(int argc, char** argv)
{

	/*Initialization of GLUT Library */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

	/*Create a window with title specified */
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment 1");

	init();	/*not GLUT call, initialize several parameters */

	/*Register different CALLBACK function for GLUT to response 
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutPassiveMotionFunc(mousemove);
	glutMouseFunc(mouseclick);
	glutIdleFunc(idle);
	
	
	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();
	
}
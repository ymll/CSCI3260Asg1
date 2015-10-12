/*****************************************************************************
FILE : submit.cpp (csci3260 2015-2016 Assignment 1)
NOTE : you have to implement functions in this file
*****************************************************************************/
/*****************************************************************************
Student Information
Student ID: 1155032377
Student Name: Yung Man Lee
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <gl\glut.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#include <vector>
using namespace std;

const float PI = 3.14159265;

int winWidth = 1000;
int winHeight = 800;

int xlimit = 700;
int zlimit = 1200;
int zlimitneg = -400;

float keyboardCameraMoveSpeed(10.0);
float mouseCameraMoveSpeed(0.5);
float mouseCameraMoveDirection[2] = {0, 0};

bool isExtraLightOn = true;
float light_X(-500), light_Y(300), light_Z(-1000);

float cam_X(0), cam_Y(350), cam_Z(800);
float cam_ViewX(0), cam_ViewY(0), cam_ViewZ(0);

GLfloat no_mat[] = {0.0,0.0,0.0,1.0};
GLfloat mat_diffuse[] = {0.8,0.2,0.5,1.0};
GLfloat mat_specular[] = {1.0,1.0,1.0,1.0};
GLfloat high_shininess[] = {100.0};
GLfloat special_tree_amb[] = {0.19225, 0.19225, 0.19225, 1.0};
GLfloat special_tree_diff[] = {0.50754, 0.50754, 0.50754, 1.0};
GLfloat special_tree_spec[] = {0.508273, 0.508273, 0.508273, 1.0};
GLfloat special_tree_shininess[] = {0.4};

float groundWidth(1600.0), groundLong(1800.0);
GLUquadric *quad;

struct snowflake
{
	float x;
	float y;
	float z;
	int radius;
	int timeOnFloor;
};

struct snowman
{
	float x;
	float z;
	float r;
	float g;
	float b;
	vector<double> radius;
};

struct gift
{
	float x;
	float z;
	float boxsize;
	float r1;
	float g1;
	float b1;
	float r2;
	float g2;
	float b2;
};

float snowflakeFallingSpeed = 1.0f;
vector<snowflake> snowflakes;
bool isSnowFalling = false;
int maxTimeSnowOnFloor = 200;

vector<snowman> snowmen;
vector<gift> gifts;

void addRandomSnowflake();

void init(void) // All Setup For OpenGL Goes Here
{
	// Light 0 Settings
	static GLfloat light0pos[] = {200.f, 100.f, 400.f, 0.f};
	static GLfloat light0_mat1[] = {0.8, 0.8, 0.8, 1.f};
	static GLfloat light0_diff1[] = {0.9, 0.9, 0.9, 1.f};
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_mat1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff1);

	// Light 1 Settings
	static GLfloat ambientLight[] = { 0.01f, 0.01f, 0.0f, 1.0f };
	static GLfloat diffuseLight[] = { 0.25f, 0.25f, 0.0f, 1.0f };
	static GLfloat specularLight[] = { 0.05f, 0.05f, 0.0f, 1.0f };
	static GLfloat position[] = { light_X, light_Y, light_Z, 1.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT1, GL_POSITION, position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	if(isExtraLightOn)
	{
		glEnable(GL_LIGHT1);
	}

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);		// Cull all the back face
	glFrontFace(GL_CCW);		// Define Counter-Clockwise as front face
	
	glEnable(GL_COLOR_MATERIAL); 

	quad = gluNewQuadric();
	srand(time(NULL));

	snowman snowman;
	vector<double> snowballsRadius;
	snowman.radius.push_back(0.0);
	snowman.radius.push_back(50.0);
	snowman.radius.push_back(35.0);
	snowman.x = 0;
	snowman.z = 0;
	snowman.r = 0.8;
	snowman.b = 0.8;
	snowmen.push_back(snowman);

	gift gift;
	gift.x = -120;
	gift.z = 30;
	gift.boxsize = 60;
	gift.r1 = 1.0f;
	gift.r2 = 1.0f;
	gift.g2 = 1.0f;
	gifts.push_back(gift);
}

// Move camera to specified position without changing view angle
void moveCameraTo(float newCamX, float newCamY, float newCamZ)
{
	if (newCamX <= xlimit && newCamX >= -xlimit && newCamZ <= zlimit && newCamZ >= zlimitneg)
	{
		cam_ViewX = (cam_ViewX - cam_X) + newCamX;
		cam_ViewY = (cam_ViewY - cam_Y) + newCamY;
		cam_ViewZ = (cam_ViewZ - cam_Z) + newCamZ;
		cam_X = newCamX;
		cam_Y = newCamY;
		cam_Z = newCamZ;
		printf("moveCameraTo: %f, %f, %f; %f, %f, %f\n", cam_X,cam_Y,cam_Z, cam_ViewX, cam_ViewY, cam_ViewZ);
	}
}

// Move camera by magnitude of movement
void moveCameraBy(float deltaX, float deltaY, float deltaZ)
{
	moveCameraTo(cam_X + deltaX, cam_Y + deltaY, cam_Z + deltaZ);
}

void updateCamera() 
{
	gluLookAt(cam_X,cam_Y,cam_Z, cam_ViewX, cam_ViewY, cam_ViewZ, 0, 1, 0);
	moveCameraBy(mouseCameraMoveSpeed * mouseCameraMoveDirection[0], 0, mouseCameraMoveSpeed * mouseCameraMoveDirection[1]);
}

void drawOrigin()
{
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	{
		glVertex4f(0.0f, 0.0f, 0.0f, 1.0f);
		glVertex4f(0.0f, 1.0f, 0.0f, 0.0f);
	}
	glEnd();
	glBegin(GL_LINES);
	{
		glVertex4f(0.0f, 0.0f, 0.0f, 1.0f);
		glVertex4f(1.0f, 0.0f, 0.0f, 0.0f);
	}
	glEnd();
	glBegin(GL_LINES);
	{
		glVertex4f(0.0f, 0.0f, 0.0f, 1.0f);
		glVertex4f(0.0f, 0.0f, 1.0f, 0.0f);
	}
	glEnd();
	glPopMatrix();
}

void addRandomSnowflake()
{
	snowflake snowflake;
	snowflake.radius = ((rand() % 100) + 10.0) / 20;
	snowflake.x = (rand() % (int)groundWidth) - groundWidth/2;
	snowflake.z = (rand() % (int)groundLong) - groundLong/2;
	snowflake.y =  (float)((rand() % 200) + 100.0);
	snowflake.timeOnFloor = 0;

	snowflakes.push_back(snowflake);
}

void drawGiftBox()
{
	for (int g=0; g<gifts.size(); g++)
	{
		glPushMatrix();
		glTranslatef(gifts[g].x, 1.0f, gifts[g].z);
		glRotatef(-90, 1, 0, 0);
		//draw red box
		glPushMatrix();
		glColor3f(gifts[g].r1, gifts[g].g1, gifts[g].b1);
		glScalef(gifts[g].boxsize, gifts[g].boxsize, gifts[g].boxsize);
		glutSolidCube(1.0f);
		glPopMatrix();
		//draw yellow ribbon
		glPushMatrix();
		glColor3f(gifts[g].r2, gifts[g].g2, gifts[g].b2);
		glScalef(10.0f, gifts[g].boxsize+5.0f, gifts[g].boxsize+5.0f);
		glutSolidCube(1.0f);
		glPopMatrix();
		//draw yellow ribbon
		glPushMatrix();
		glScalef(gifts[g].boxsize+5.0f, 10.0f, gifts[g].boxsize+5.0f);
		glutSolidCube(1.0f);
		glPopMatrix();

		glPopMatrix();
	}
}

void drawGround()
{
	glPushMatrix();
	glScalef(groundWidth, 0.1f, groundLong);
	glColor3f(0.55f, 0.65f, 0.8f);
	glutSolidCube(1.0f);
	glPopMatrix();
}

void drawSun()
{
	if (isExtraLightOn)
	{
		glPushMatrix();
		glTranslatef(light_X, light_Y, light_Z);
		glColor3f(1.0f, 0.6f, 0.0f);
		glutSolidSphere(200.0, 30, 8);
		glPopMatrix();
	}
}

void drawPool()
{
	glPushMatrix();
	glTranslatef(100.0f, 1.0f, 200.0f);
	glRotatef(-90, 1, 0, 0);
	glColor3f(0.5f, 0.6f, 0.85f);
	gluDisk(quad, 0.0, 85, 20, 1);
	glPopMatrix();
}

void drawTree(float x, float z, float size)
{
	glPushMatrix();
	//draw wooden base
	glTranslatef(x, 0.0f, z);
	glRotatef(-90, 1, 0, 0);
	glColor3f(0.15f, 0.05f, 0.0f);
	gluCylinder(quad, 30 * size, 28 * size, 40 * size, 50, 10);
	//draw snow
	glTranslatef(0.0f, 0.0f, 30.0f);
	glColor3f(0.8f, 0.8f, 0.8f);
	gluCylinder(quad, 80 * size, 60 * size, 15 * size, 20, 10);
	//draw leaves cone
	glTranslatef(0.0f, 0.0f, 8.0f);
	glColor3f(0.1f, 0.5f, 0.3f);
	gluCylinder(quad, 70 * size, 0, 100 * size, 8, 10);
	//draw snow
	glTranslatef(0.0f, 0.0f, 50.0f);
	glColor3f(0.8f, 0.8f, 0.8f);
	gluCylinder(quad, 60 * size, 40 * size, 15 * size, 20, 10);
	//draw upper cone
	glTranslatef(0.0f, 0.0f, 8.0f);
	glColor3f(0.1f, 0.5f, 0.3f);
	gluCylinder(quad, 50 * size, 0, 80 * size, 8, 10);
	glPopMatrix();
}

void drawTrees()
{
	drawTree(580, -200, 1);
	drawTree(-400, -150, 1);
	drawTree(-350, 80, 0.8);
	drawTree(350, 80, 0.8);
	drawTree(-500, 150, 0.7);
	drawTree(600, 200, 0.6);
	drawTree(-400, 250, 0.6);
	drawTree(-150, 300, 0.7);
	drawTree(550, 300, 0.6);
	drawTree(-600, 400, 0.7);
	drawTree(350, 400, 0.7);
	drawTree(-500, 500, 0.8);
	drawTree(-650, 600, 1);
	drawTree(200, 500, 0.6);
	drawTree(700, 650, 1);
	drawTree(0, 750, 1.2);
	drawTree(300, 800, 1.3);
	drawTree(-200, 850, 1);
	drawTree(500, 850, 0.8);

	// Tree wth different material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, special_tree_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, special_tree_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, special_tree_spec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, special_tree_shininess);
	drawTree(0, -550, 1);
	drawTree(600, -500, 0.6);
	drawTree(-300, -500, 0.8);
	drawTree(200, -500, 1);
	drawTree(-600, -450, 1.2);
	drawTree(500, -400, 1.1);
	drawTree(-200, -350, 1.2);
}

void drawSnowmen()
{
	for (int s=0; s<snowmen.size(); s++)
	{
		double stickiness = 20.0;
		vector<double> snowballsRadius = snowmen[s].radius;
	
		//int snowballCount = sizeof(snowballsRadius) / sizeof(snowballsRadius[0]) - 1;

		glPushMatrix();

		// Draw snowballs
		glColor3f(0.7f, 0.7f, 0.7f);
		glTranslatef(snowmen[s].x, 0.0, snowmen[s].z);
		for(unsigned int i=1; i<snowballsRadius.size(); i++)
		{
			glTranslatef(0.0, snowballsRadius[i] + snowballsRadius[i-1] - stickiness, 0.0);
			gluSphere(quad, snowballsRadius[i], 60, 60);
		}

		// Draw hat
		glColor3f(snowmen[s].r, snowmen[s].g, snowmen[s].b);
		glTranslatef(0.0, snowballsRadius[snowballsRadius.size()-1] - 10.0, 0.0);
		glRotatef(-90, 1, 0, 0);
		glutSolidCone(20.0, 30.0, 60, 60);

		// Draw eyes
		glColor3f(0.1f, 0.1f, 0.1f);
		glTranslatef(snowballsRadius[snowballsRadius.size()-1] / 5, snowballsRadius[snowballsRadius.size()-1] * -0.85, snowballsRadius[snowballsRadius.size()-1] * -0.5);
		gluSphere(quad, 5, 10, 10);
		glTranslatef(snowballsRadius[snowballsRadius.size()-1] / -2.5, 0.0, 0.0);
		gluSphere(quad, 5, 10, 10);

		glPopMatrix();
	}
}

void drawSnowflakes()
{
	const int armCount = 6;

	glColor3f(1.0f, 1.0f, 1.0f);
	for(vector<snowflake>::iterator it = snowflakes.begin(); it != snowflakes.end(); it++)
	{
		glPushMatrix();

		if(isSnowFalling)
		{
			it->y = max(it->y - snowflakeFallingSpeed, 0);
		}
		glTranslatef(it->x, it->y, it->z);
		gluDisk(quad, 0, it->radius/1.5, armCount, 1);

		// Draw arms
		glLineWidth(2.0);
		for (int i=0; i<armCount/2; i++)
		{
			glBegin(GL_LINE);
			{
				float angle = i*PI/armCount*2 + 11.0;
				glVertex3f(-it->radius*cos(angle), -it->radius*sin(angle), 0);
				glVertex3f(it->radius*cos(angle), it->radius*sin(angle), 0);
			}
			glEnd();
		}

		// Check if fall on floor
		if (it->y < 0.0001)
		{
			it->timeOnFloor++;
		}
		glPopMatrix();
	}
}

void display(void) // Here's Where We Do All The Drawing
{
	glClearColor(0.0, 0.0, 0.0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	// Default material
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,high_shininess);

	glLoadIdentity();
	updateCamera();

	// TODO:
	// Draw grounds and objects here
	drawOrigin();
	drawGround();
	drawSun();
	drawPool();
	drawGiftBox();
	drawSnowmen();
	drawSnowflakes();
	drawTrees();

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

	gluPerspective(45, (float)w/(float)h, 100, 2000); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


void special(int key, int x, int y) // Handle special keys
{	
	switch (key) 
	{
	case GLUT_KEY_LEFT:
		moveCameraBy(-keyboardCameraMoveSpeed, 0, 0);
		break;
	case GLUT_KEY_UP:
		moveCameraBy(0, 0, -keyboardCameraMoveSpeed);
		break;
	case GLUT_KEY_RIGHT:
		moveCameraBy(keyboardCameraMoveSpeed, 0, 0);
		break;
	case GLUT_KEY_DOWN:
		moveCameraBy(0, 0, keyboardCameraMoveSpeed);
		break;
	}
}

void keyboard(unsigned char key, int x, int y) // Handle the keyboard events here
{
	int snowballsCount;
	double baseRadius;
	snowman snowman;
	gift gift;

	switch (key) 
	{
	case '\033'://press 'esc' to quit
		exit(0);
		break;
	case 's'://press 's' to snow
		isSnowFalling = true;
		for(int i=0; i<1000; i++)
		{
			addRandomSnowflake();
		}
	break;
	case 'a'://press 'a' to add a snowman
		snowballsCount = rand() % 2 + 2;
		baseRadius = (double) (rand() % 20) + 40.0;
		snowman.radius.push_back(0.0);
		for (int i=0;i<snowballsCount;i++)
		{
			snowman.radius.push_back(baseRadius - (double) (rand() % 2 + 8) * i);
		}
		snowman.x = rand() % 100 * 10 - 300.0;
		snowman.z = rand() % 100 * 10 - 300.0;
		snowman.r = rand() * 1.0 / RAND_MAX;
		snowman.g = rand() * 1.0 / RAND_MAX;
		snowman.b = rand() * 1.0 / RAND_MAX;
		snowmen.push_back(snowman);
	break;
	case 'g'://press 'g' to drop a gift box
		gift.x = rand() % 200 * 5 - 500.0;
		gift.z = rand() % 200 * 5 - 500.0;
		gift.boxsize = rand() % 20 + 30; 
		gift.r1 = rand() * 1.0 / RAND_MAX;
		gift.g1 = rand() * 1.0 / RAND_MAX;
		gift.b1 = rand() * 1.0 / RAND_MAX;
		gift.r2 = rand() * 1.0 / RAND_MAX;
		gift.g2 = rand() * 1.0 / RAND_MAX;
		gift.b2 = rand() * 1.0 / RAND_MAX;
		gifts.push_back(gift);
	break;
	case 'l'://press 'l' to trigger sunlight
		isExtraLightOn = !isExtraLightOn;
		if(isExtraLightOn)
		{
			glEnable(GL_LIGHT1);
		}
		else
		{
			glDisable(GL_LIGHT1);
		}
	}
}

void mouseclick(int button, int state, int x, int y) // Handle the mouse click events here
{
	printf("Button %d, State %d, Position %d, %d \n", button, state, x, y);	
}

void mousemove(int x, int y) // Handle the mouse movement events here
{
	printf("Mouse moved to position %d %d \n", x, y);
	float xRatio = x * 1.0 / winWidth;
	float yRatio = y * 1.0 / winHeight;

	if (xRatio < 0.2)
	{
		mouseCameraMoveDirection[0] = -1.0f;
	} 
	else if (xRatio > 0.8)
	{
		mouseCameraMoveDirection[0] = 1.0f;
	}
	else
	{
		mouseCameraMoveDirection[0] = 0.0f;
	}

	if (yRatio < 0.2)
	{
		mouseCameraMoveDirection[1] = -1.0f;
	} 
	else if (yRatio > 0.8)
	{
		mouseCameraMoveDirection[1] = 1.0f;
	}
	else
	{
		mouseCameraMoveDirection[1] = 0.0f;
	}
}

void idle()
{
	for (vector<snowflake>::iterator it = snowflakes.begin(); it != snowflakes.end(); ) {
		if (it->timeOnFloor > maxTimeSnowOnFloor) {
			it = snowflakes.erase(it);
		} else {
			++it;
		}
	}
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

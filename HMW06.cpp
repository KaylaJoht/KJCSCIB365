/*

 Programmer: Johnson, Kayla
 Assignment: HMW06
 Project Name: Animated Battle
 Purpose: This program creates a 3D model of a pole rotating two ships, and a person with flames rotating around them

 Date: 4/29/2024

*/

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// ****** Function Prototypes ****************
void instruction();
void init(void);
void myDisplayFunc(void);
void myReshapeFunc(int w, int h);
void myKeyboard(unsigned char key, int x, int y);
void myMouse(int button, int state, int x, int y);
void myMotion(int x, int y);
void myIdleFunc();
void displayTitle();

// ****** Global Variables *************

//These variables are time variables to specify how long rotation takes
int timer = 0;

//angle for rotation around the cone
float cone = 0.0; 
//angle for the flames rotation
float flame = 0.0;

//Variable for activating/deactivating idle function
bool idle = false;

//Functions for altering the translation, scale, and rotation using the mouse
//Taken from the code sameple by Dr. Liang
bool leftButtonDown = false;
bool middleButtonDown = false;
bool rightButtonDown = false;
float previousX = 0.0, previousY = 0.0, previousZ = 0.0;
float translateX = 0.0, translateY = 0.0, translateZ = 0.0;
float rotX = 0.0, rotY = 0.0, rotZ = 0.0;
float scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;


// ******** Main routine ***************************
/*
 *  Request double buffer display mode.
 *  Register mouse input callback functions
 */
int main(int argc, char** argv)
{
	// Initialize GLUT
	// Allow cmd line arguments to be passed to the glut
	glutInit(&argc, argv);

	// Using double buffer for animation
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// GLUT Window Initialization
	glutInitWindowSize(1300, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("HMW06");

	init(); // Initialize OpenGL graphics state
	instruction(); // Print user instructions for controls

	// Register callback functions
	glutDisplayFunc(myDisplayFunc);
	glutReshapeFunc(myReshapeFunc);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);

	// Turn the flow of control over to GLUT and enter the main event loop
	glutMainLoop();

	// The code will never reach here!
	return 0;   /* ANSI C requires main to return int. */
}


// ********* Function Implementations ************
void instruction()
{
	cout << "Click space to start/end rotation\n";
	cout << "Click r/R to reset the scene\n";
	cout << "Click l/L to turn on the lights\n";
	cout << "Click s/S to turn off the lights\n";
	cout << "Move your mouse with the left button clicked to rotate the image\n";
	cout << "Move your mouse with the right button clicked to translate the image\n";
	cout << "Move your mouse with the middle button clicked to scale the image\n";

}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); //Set the background color of display window
	GLfloat position[] = { 1.0, 1.0, 0.0, 0.0 };   // w=0 --> a directional source
	
	//Setting the colors for ambient, diffuse, and specular. These are the default colors
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat local_view[] = { 0.0 };

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);  //step 3.1
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHT0);		// Turn on the light 0
	glEnable(GL_LIGHTING);
}

//Function that displays the image
void myDisplayFunc(void)
{
	//Default color values for the material
	float matA[] = { 0.5, 0.5, 0.5, 1.0 };//Ambient
	float matD[] = { 0.5, 0.5, 0.5, 1.0 };//Diffuse
	float matS[] = { 1.0, 1.0, 1.0, 1.0 };//Specular

	float emmision[] = { 0.1, 0.0, 0.1, 1 };
	float shineNess = 0.6;

	//Sets all the material below it to have the colors of the material
	glMaterialfv(GL_FRONT, GL_AMBIENT, matA);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matD);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matS);
	glMaterialf(GL_FRONT, GL_SHININESS, shineNess * 128.0);
	glMaterialfv(GL_FRONT, GL_EMISSION, emmision);

	//Custom colors
	//Only applies to Ambient and Diffusde, as Specular remains the same across all colors
	//Colors for central cone, should be red
	float coneA[] = { 0.1, 0.0, 0.0, 1.0 };
	float coneD[] = { 0.84, 0.0, 0.0, 1.0 };

	//Colors for the Docehedron, should be blue
	float shipA[] = { 0.0, 0.0, 1.0, 1.0 };
	float shipD[] = { 0.0, 0.11, 0.84, 1.0 };

	//Colors for the floating sphere, should be gray
	float sphereA[] = { 0.7, 0.7, 0.7, 1.0 };
	float sphereD[] = { 0.56, 0.56, 0.56, 1.0 };

	//Colors for the pipes that connect the cone to the ships, should be yellow
	float wireA[] = { 1, 1, 0, 1.0 };
	float wireD[] = { 0.92, 1, 0.08, 1.0 };

	//Colors for the persons head, should be lilac
	float headA[] = { 1, 0.5, 1, 1.0 };
	float headD[] = { 0.79, 0.49, 1, 1.0 };

	//Colors for the persons body, should be purple
	float bodyA[] = { 0.7, 0.0, 0.9, 1.0 };
	float bodyD[] = { 0.53, 0.14, 0.69, 1.0 };

	//Colors for the flames, should be orange
	float flameA[] = { 1, 0.0, 0.0, 1.0 };
	float flameD[] = { 1, 0.68, 0.13, 1.0 };

	//Colors for the flames, should be orange
	float pathA[] = { 1, 1, 1, 1.0 };
	float pathD[] = { 1, 1, 1, 1.0 };

	glClear(GL_COLOR_BUFFER_BIT);

	//Creating the often used Cylinder object
	GLUquadricObj* cylinderObj;
	cylinderObj = gluNewQuadric();
	gluQuadricDrawStyle(cylinderObj, GLU_LINE);

	//Creating a matrix that uses the mouse functions on the entire image
	glPushMatrix();

	// apply translation
	glTranslatef(translateX, translateY, translateZ);

	//apply rotation
	glRotatef(rotX, 1.0f, 0.0f, 0.0f);
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);
	glRotatef(rotZ, 0.0f, 0.0f, 1.0f);

	//Apply scaling
	glScalef(scaleX, scaleY, scaleZ);

	//Path for Sphere
	glPushMatrix();
		glTranslatef(0, 0.5, 0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, pathA);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, pathD);
		glRotatef(-90, 1, 0, 0);
		glutSolidTorus(0.01, 3, 50, 40);
	glPopMatrix();

	//Path for Ship
	glPushMatrix();
		glTranslatef(0, 1, 0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, pathA);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, pathD);
		glRotatef(-90, 1, 0, 0);
		glutSolidTorus(0.01, 5, 50, 40);
	glPopMatrix();

	//Path for Person
	glPushMatrix();
		glTranslatef(0, -1, 0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, pathA);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, pathD);
		glRotatef(-90, 1, 0, 0);
		glutSolidTorus(0.01, 4, 50, 40);
	glPopMatrix();

	//Matric for the center cone and all it's features
	glPushMatrix();
		
		//Center cone
		glTranslatef(0, -1, 0); //Moving it back to it's position
		glRotatef(-90, 1.0, 0.0, 0.0); //Rotating it to be upright
		glMaterialfv(GL_FRONT, GL_AMBIENT, coneA);//Changing the ambient color
		glMaterialfv(GL_FRONT, GL_DIFFUSE, coneD);//Changing the Diffuse
		glutSolidCone(0.5, 2.5, 3, 3);//Creating the cone
		glTranslatef(0, 1, 0);//Moving to origin

		/*Basically the rotaion, everything in this matrix rotates around this point*/
		glTranslatef(0, -1, 0);
		glRotatef(cone, 0.0, 0.0, 1.0);
		glTranslatef(0, 1, 0);
		
		//Creating the wires around the cone
		glPushMatrix();
			//Wire 1 -- Shorter one
			glTranslatef(0, -1, 2.5);
			glRotatef(-90, 0.0, 1.0, 0.0);
			glMaterialfv(GL_FRONT, GL_AMBIENT, wireA);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, wireD);
			gluCylinder(cylinderObj, 0.02, 0.02, 3, 3, 3);//Using the cylinder object to create a cylinder
			glTranslatef(0, 1, -2.5);
			//Making the veritcal piece that attached the first arm to the shpere
			glPushMatrix();
			//Connection
				glTranslatef(0, -1, 5.5);
				glRotatef(-90, 0.0, 1.0, 0.0);
				gluCylinder(cylinderObj, 0.02, 0.02, 0.7, 2, 2);
				glTranslatef(0, 1, -5.5);
			glPopMatrix();
			//Matrix that makes the Spherical shape
			glPushMatrix();
				glTranslatef(-1, -1, 5.5);
				glMaterialfv(GL_FRONT, GL_AMBIENT, sphereA);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, sphereD);
				glutSolidSphere(0.5, 10, 8);
			glPopMatrix();
		glPopMatrix();
		
		//Creating the other connector
		glPushMatrix();
		//Wire 2 -- Longer one
			glTranslatef(0, -1, 2.5);
			glRotatef(90, 0.0, 1.0, 0.0);
			glMaterialfv(GL_FRONT, GL_AMBIENT, wireA);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, wireD);
			gluCylinder(cylinderObj, 0.02, 0.02, 5, 3, 3);
			glTranslatef(0, 1, -2.5);
			glPushMatrix();
				//Connection
				glTranslatef(0, -1, 7.5);
				glRotatef(90, 0.0, 1.0, 0.0);
				gluCylinder(cylinderObj, 0.02, 0.02, 0.5, 2, 2);
				glTranslatef(0, 1, 7.5);
			glPopMatrix();
			glPushMatrix();
				//Ship represented by Docehedron
				glTranslatef(0.7, -0.7, 7.5);
				glRotatef(90, 0, 0.5, 0.5);
				glScalef(0.3, 0.2, 0.3);//Because it's a pre-made shape, I have to scale it to a certain size to fit in my image
				glMaterialfv(GL_FRONT, GL_AMBIENT, shipA);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, shipD);
				glutSolidDodecahedron();
				glTranslatef(-0.7, 0.7, -7.5);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix(); //Ending matrix for the cone
		
	//Matrix to create the person with the flames
	glPushMatrix();

		glPushMatrix();

			glRotatef(-cone, 0, 1.0, 0);//She rotates in the opposite direction

			//Path for Flames
			glPushMatrix();
				glTranslatef(4, -0.5, 0);
				glMaterialfv(GL_FRONT, GL_AMBIENT, pathA);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, pathD);
				glRotatef(-90, 1, 0, 0);
				glutSolidTorus(0.01, 1, 50, 40);
			glPopMatrix();

			glTranslatef(4, -0.5, 0);
			glScalef(0.3, 0.3, 0.3);//have to scale pre-made shape
			glMaterialfv(GL_FRONT, GL_AMBIENT, headA);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, headD);
			glutSolidOctahedron();
			
			glPushMatrix();
				//Flame 1
				glRotatef(flame, 0, 1.0, 0);

				glTranslatef(2, 1, 1);
				glRotatef(90, 1, 0, 0);
				glScalef(5, 5, 5);
				glMaterialfv(GL_FRONT, GL_AMBIENT, flameA);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, flameD);
				glutSolidCone(0.1, 0.7, 5, 3);
				glTranslatef(-2, -1, -1);
			glPopMatrix();
			glPushMatrix();
			glRotatef(flame, 0, 1.0, 0);
			//Flame 2
				glTranslatef(-2, 1, 1);
				glRotatef(90, 1, 0, 0);
				glScalef(5, 5, 5);
				glutSolidCone(0.1, 0.7, 5, 3);
				glTranslatef(2, -1, -1);
			glPopMatrix();
			
			glPushMatrix();
			//Body
				glTranslatef(0, -1, 0);
				glRotatef(84, 1, 1, 0);
				glScalef(0.9, 0.9, 0.9);
				glMaterialfv(GL_FRONT, GL_AMBIENT, bodyA);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, bodyD);
				glutSolidTetrahedron();
				glTranslatef(0, 1, 0);
			glPopMatrix();
			
		glPopMatrix();		
		
	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
}

void myReshapeFunc(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0); //Set viewing volume

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(7.0, 2.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //Set the camera
}

/*Function that takes care of rotation according to a timer*/
void myIdleFunc() {
	timer = timer + 1;
	if (timer > 100){
		timer = 0; 
		cone = cone + 6;

		if (cone > 360.0) {
			cone = cone - 360.0; //Reset the angle 
		}

		flame = flame + 30;

		if (flame > 360.0) {
			flame = flame - 360.0; //Reset the angle 
		}
	}
	glutPostRedisplay();
}

/*Function that checks which button is clicked
  Taken from the code provided by Dr. Liang*/
void myMouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		leftButtonDown = true;
	}
	else
		leftButtonDown = false;

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		middleButtonDown = true;
	}
	else
		middleButtonDown = false;

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		rightButtonDown = true;
	}
	else
		rightButtonDown = false;

	//Updates the global coordinates
	previousX = (float)x;
	previousY = (float)y;
	previousZ = (float)y;
}

/*Function that moves the image with the mouse
  Taken from the code provided by Dr. Liang*/
void myMotion(int x, int y) {

	//  Rotate
	if (leftButtonDown)
	{
		rotX += (y - previousY);
		rotY += (x - previousX);
	}

	//  Scale
	if (middleButtonDown)
	{
		scaleX -= (y - previousY) * 0.005;
		scaleY -= (y - previousY) * 0.005;
		scaleZ -= (y - previousY) * 0.005;
	}

	//  Translate
	if (rightButtonDown)
	{
		translateX += (x - previousX) * 0.1;
		translateY -= (y - previousY) * 0.1;
	}

	//Update global coordiantes
	previousX = (float)x;
	previousY = (float)y;
	previousZ = (float)y;

	//Action 3:
	glutPostRedisplay();
}


//Function that holds the keyboard functions
void myKeyboard(unsigned char key, int x, int y) {

	//Action 1:
	switch (key) {
	case 'r':
	case 'R':
		translateX = 0.0, translateY = 0.0, translateZ = 0.0;
		rotX = 0.0, rotY = 0.0, rotZ = 0.0;
		scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;
		break;
	case ' ':
		if (idle == false) {
			glutIdleFunc(myIdleFunc);
			idle = true;
		}
		else if(idle ==true){
			glutIdleFunc(NULL);
			idle = false;
		}
		break;
	case 's':
	case 'S':
		glEnable(GL_LIGHT0);
		break;
	case 'l':
	case 'L':
		glDisable(GL_LIGHT0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}
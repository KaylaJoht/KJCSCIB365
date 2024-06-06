/*
	Programmer: Johnson, Kayla
	Assignment: HMW05
	Project Name: Animated Cat Clock
	Purpose: This program takes the clock from HW04, and animates it using openGL and glut
	Date: 4/3/2024
*/



/* Library and Header File s*/
#include <iostream>
#include <vector>
#include <cmath>	//Using sin and cos functions
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;


/* Function Prototypes */
//This was taken from the handout provided by Dr. Liang
void Init(); //Function that Initializes the program
void instruction(); //Function that Initializes the program
void myIdleFunc(); //Callback of "glutIdleFunc()"
void myKeyboardFunc(unsigned char key, int x, int y); //Callback of keyboard controls

//Menu functions
void setupMenu();
void mainMenu(int id);
void colorMenu(int id);

//Animation functions
void moveMinutes();
void moveHours();
//reColor functions
/*These functions work with the menu to changethe color of various objects in the picture*/
vector<double> clockReColor();
vector<double> earReColor();
vector<double> eyeReColor();
vector<double> noseReColor();
vector<double> whiskerReColor();
vector<double> numberReColor();
vector<double> secondsReColor();
vector<double> minutesReColor();
vector<double> hoursReColor();
vector<double> centerReColor();
vector<double> catReColor();
vector<double> lipReColor();

//Callback functions
//Callback functins basically re-run when anything is changed
//Taken again from the Handout Provided by Dr. Liang
void myDisplay();	                                  // call back function for image definition                               
void myReshape(int newWidth, int newHeight);          // call back function when the size of display window is changed.


//Image functions
//All functions below contribute to the final image
//Since my Image is of a cat clock, all function declarations are written with the idea of being
//used for a cat clock
void createNumbers(int xcenter, int ycenter, int radius, vector<double> colors);//A function for the clock's tick marks
void createHands(double pos[], vector<double> colors);//A function for creating the clock hands
void createEllipse(int xcenter, int ycenter, int xradius, int yradius, vector<double> colors);//Function that creates an ellipse
void createEars(double pos[], vector<double> colors);//Function that creates triangles, used for the cats ears
void createIris(int xcenter, int ycenter, int xradius, int yradius);//Special version of createEllipse for the cats Irises
void createWhiskers(double posx[], double posy[], vector<double> colors);//Creates lines to serve as the whiskers
void createNose(double pos[], vector<double> colors);//creates a Triangle for the nose
void createLips(int xcenter, int ycenter, int radius, vector<double> colors);//Special version of Create circle, used for the cats lips
void createOutline(int xcenter, int ycenter, int radius, vector<double> colors, int thickness);//Special version of create circle, used for the clocks outline
void createTitle(bool isOn);//Creates a title


//Some global variables
//These variables create the size of the window, which is where the image will appear
//Others are used for transformation, rotation, displaying the title, and color theme
//Timer is used for a timer function
int windowPositionX = 500, windowPositionY = 100;
int windowWidth = 700, windowHeight = 700;
float spinAngle = 0; //Rotation angle in degrees
float minuteAngle = 180;//Rotation angle in degrees
float hourAngle = 90;//Rotation angle in degrees
bool isOn = true;//Displaying the title
float moveX = 0;//Starting value for movement
int one = 1;//movement increments
int theme = 0;//value for the color theme
int timer = 0;


/* Main Function*/
// Taken from the Code Handout provided by Dr. Liang
// The main function here will do the following:
//   1. Initialize GLUT;
//   2. Initialize display mode to specify single/double buffer and RGB/index;
//   3. Set up the initial position and size of the display window;
//   4. Create the display window;
//   5. Call user defined functions such as functions to display instructions and
//        functions to execute one time jobs;
//   6. Call one or more call-back functions via GLUT interfaces;
//   7. Start the infinite loop.
//
int main(int argc, char** argv)
{
	// 1. Initialize GLUT.
	glutInit(&argc, argv);

	// 2. Initialize display mode to specify single/double buffer and RGB/index.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);    // Use "GLUT_SINGLE" if single buffer is used
	//glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); // Use "GLUT_DOUBLE" if double buffer is used

	// 3. Set up the initial position and size of the display window
	glutInitWindowPosition(windowPositionX, windowPositionY);
	glutInitWindowSize(windowWidth, windowHeight);

	// 4. Creat the display window
	glutCreateWindow("Cat Clock");

	// 5. Call user defined functions such as functions to display instructions and
	//    functions to execute one time jobs
	Init();         // Execute initialization procedure (one time jobs)
	instruction(); //Execute instructions

	// 6. Call one or more call-back functions via GLUT interfaces
	glutDisplayFunc(myDisplay);	 // Send image definitions to display window.
	glutReshapeFunc(myReshape); // Call the call-back function when the size of display window is changed.
	glutKeyboardFunc(myKeyboardFunc);     //Set the keyboard callback
	setupMenu();
	// 7. Start the infinite loop
	glutMainLoop();

	// We will never reach here. Thus the statement can be omitted.
	return 0;
}

/* Function Implementation*/

// Taken from the Code Handout provided by Dr. Liang
// What: A function to do some one-time jobs
// Input:  None
// Output: None
// Actions: Perform some one-time jobs, e.g., initializations, such as:
//          1. Set the background color of the display window;
void Init(void)
{
	cout << "Funciton Init() is called.\n";
	//These hold the rgb values that will determine the background color
	//double colorRed = 0.09, colorGreen = 0.09, colorBlue = 0.31, alphaValue = 0.0; //Change color for HMW05
	double colorRed = 0.84, colorGreen = 0.44, colorBlue = 0.08, alphaValue = 0.0;
	// 1. Set the background color of the display window
	glClearColor(colorRed, colorGreen, colorBlue, alphaValue);
}

void instruction(void)
{
	cout << "Interaction:" << endl;
	cout << "Press space to turn on animation." << endl;
	cout << "Press s to turn off the animation." << endl;
	cout << "Right click to display menu." << endl;
}

// Taken from the Code Handout provided by Dr. Liang
// What: A call-back function specifys the actions that will be taken when the display window is created or its size is changed.
// Input: 
//			1. w - Current display window's width (in pixels)
//			2. h - Current display window's height (in pixels)
// Output: None
// Actions: Perform some one-time jobs, e.g., initializations, such as:
//			1. Set up the viewport (whole or part of the display window)
//          2. Select and initialize the projection matrix stack used for projections;
//          3. Set up projection matrix, i.e., the viewing volume that has 6 clipping planes;
//          4. Select and initialize the model-view matrix stack used for geometric transformations;
//          5. Other possible one time jobs.
void myReshape(int w, int h)
{

	//Clipping plane is the coordinates that the program uses. 
	cout << "Funciton myReshapeFunc() is called.\n";
	double clippingPlanLeft = -100.0, clippingPlanRight = 100.0;
	double clippingPlanBottom = -100.0, clippingPlanTop = 100.0;
	double clippingPlanNear = -1.0, clippingPlanFar = 1.0;

	// 1. Set up the viewport(whole or part of the display window)
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// 2. Select and initialize the projection matrix stack used for projections
	glMatrixMode(GL_PROJECTION); // Select the projection matrix stack
	glLoadIdentity();            // Initialize the stack with the identity matrix

	// Check the projection matrix stack
	//cout << "Projection matrix befor we set the clipping plane: \n";
	//getTopMatrixInStack(1);

	// 3. Set up projection matrix, i.e., the viewing volume that has 6 clipping planes.
	//    - 2D case: call "glOrtho" or "gluOrtho2D";
	//    - 3D case: call "glOrtho";
	// After this call, a transformation matrix for parallel projection is generated. 
	// The current matrix, i.e., the matrix at the top of the projection mastrix stack is 
	//     multiplied by this matrix and the result replaces the current matrix.
	glOrtho(clippingPlanLeft, clippingPlanRight, clippingPlanBottom, clippingPlanTop, clippingPlanNear, clippingPlanFar);
	// gluOrtho2D sets up a two-dimensional orthographic viewing region, i.e., the clipping plane)
	//gluOrtho2D(clippingPlanLeft, clippingPlanRight, clippingPlanBottom, clippingPlanTop); 

	// Check the projection matrix after the clipping plane was set.
	//cout << "Projection matrix after we set the clipping plane: \n";
	//getTopMatrixInStack(1);

	// 4. Select and initialize the model-view matrix stack used for geometric transformations
	glMatrixMode(GL_MODELVIEW); // Select the model-view matrix stack for transformations.
	glLoadIdentity();           // Initialize the stack with the identity matrix
}

// Taken from the Code Handout provided by Dr. Liang
// What: A display call-back function that defines the picture that will be displayed.
// Action: 
//			1. Clear and then display the background color of the display window;
//			2. Defines color parameters when the same function is called twice;
//			3. Defines position parameters to avoid hardcoding values
//			4. Calls functions to create the cat clock
//			5. Animates the irises
//          6. Rotates the clock hands
//			7. Flush the buffer to display the image into the display window, i.e., show the image.
// Input: None
// Output: None
void myDisplay(void) {

	//1. Clear the background color of the display window and be ready to color the display window
	//       with the color defined by "glClearColor()" function.
	glClear(GL_COLOR_BUFFER_BIT);


	//2. Defines color parameters when the same function is called twice
	//   createEllipse is called to create numerous circles which have different colors
	//   Also, providing some proto-functionality for changing the color in HW05
	//   All colors receive their colors from their own ReColor method
	vector<double> clockColors = clockReColor();//Setting the color for the clock face
	vector<double> eyeColors = eyeReColor();//Setting the colors for the cats sclera
	vector<double> catColors = catReColor();//Setting the color of the actual cat
	vector<double> earColor = earReColor();//Setting the color of the inner ears
	vector<double> noseColors = noseReColor();//Setting the color of the nose
	vector<double> secondsColor = secondsReColor();//Setting the color of the seconds hand
	vector<double> minutesColor = minutesReColor();//Setting the color of the minutes hand
	vector<double> hoursColor = hoursReColor();//Setting the color of the hours
	vector<double> whiskerColors = whiskerReColor();//Setting the colors of the whiskers
	vector<double> centerColors = centerReColor();//Setting the colors of the center of the clock
	vector<double> numberColors = numberReColor();//Setting the colors of the tick marks
	vector<double> lipColors = lipReColor();//Setting the colors of the lips

	//3. Defines position parameters to avoid hardcoding values
	//   Avoiding hard-coded values
	double secondsHand[6] = { 0,1,-1,-40,-15,-10 };
	double minutesHand[6] = { 0,1.5,-1.5,-40,-20,-15 };
	double hoursHand[6] = { 0,2,-2,-40,-25,-20 };
	double whiskersx[6] = { -9, -39,-7, -37, -5, -35 };
	double whiskersy[6] = { 12, 16, 10, 10.5, 8, 4 };
	double outerEar[6] = {-42.2, 24.6, -36.2, 77.8, -8, 43.4};
	double innerEar[6] = {36.8, 34.6, 33.8, 69, 15, 45.2};
	double nose[6] = {-7, 13, 0, 7, 7, 13};

	//4. Calls functions to create the cat clock
	createTitle(isOn);
	createEars(outerEar, catColors);//Creating Cat Ears
	createEars(innerEar, earColor);//Creating Cat Ears
	createEllipse(0, 7, 45, 30, catColors);//Creating cat head
	createEllipse(-9, 13, 11, 11, eyeColors);//Right eye
	createEllipse(9, 13, 11, 11, eyeColors);//Left eye
	
	//5. Moves the Iris
	//The Irises move back and forward through the eyes
	//First we push our matrix to the top
	glPushMatrix();
		glTranslatef(18, 26, 0.0);//Translate Iris to the original position
		glTranslatef(moveX, 0, 0.0);//Moveing the position of the iris 
		glTranslatef(-18, -26, 0.0);//Translate Iris to the origin
		createIris(9, 13, 4, 11);//Creating the Iris in the eyes
	glPopMatrix();//Removing the matrix from the matrix stack
	
	//4. Creating cat features
	createOutline(-9, 13, 14, catColors, 20);//Outer layer of eyes
	createOutline(9, 13, 14, catColors, 20);//Outer layer of eyes
	createLips(-4, 4, 8, lipColors);//Cat lips
	createNose(nose, noseColors);//Cat nose
	createWhiskers(whiskersx, whiskersy, whiskerColors);//Cat Whiskers
	createEllipse(0, -20, 39, 39, clockColors); //Creating the clock face
	createNumbers(0, -40, 35, numberColors);//Creating the tick marks on the clock
	
	
	//6.Rotates clock hands
	//Rotate Seconds hand
	glPushMatrix();
		glTranslatef(0, -40, 0.0); //Move back to original position
		glRotatef(-spinAngle, 0, 0, 1);  // Rotate the axis
		glTranslatef(0, 40, 0.0);//Move the axis of rotation to the origin
		createHands(secondsHand, secondsColor);//Creating the Seconds hand
	glPopMatrix();

	//Rotate minutes hand
	glPushMatrix();
		glTranslatef(0, -40, 0.0);	//Move back to original position
		glRotatef(-minuteAngle, 0, 0, 1);  // Rotate the axis
		glTranslatef(0, 40, 0.0);//Move the axis of rotation to the origin
		createHands(minutesHand, minutesColor);//Creating the Minutes hand
	glPopMatrix();

	//Rotate hours hand
	glPushMatrix();
		glTranslatef(0, -40, 0.0);	//Move back to original position
		glRotatef(-hourAngle, 0, 0, 1);  // Rotate the axis
		glTranslatef(0, 40, 0.0);//Move the axis of rotation to the origin
		createHands(hoursHand, hoursColor);//Creating the Minutes hand
	glPopMatrix();

	//4. Creating cat features
	createEllipse(0, -20, 3, 3, catColors);// Inner Circle for Clock
	createOutline(0, -20, 39, catColors, 10);//Outer layer of Clock


	// 7. Flush the buffer to display the image into the display window, i.e., show the image
	//glFlush();           // When single buffer is used.
	glutSwapBuffers(); //For double buffer
}


// What: keyboard callback
//Input: sizeFactor - a keyboard key defined in ASCII character table
//       xMouse, yMouse - the window relative coordinates when the mouse button state changed
//Output: None
//Actions:  1. Use keys spacebar to start the animation
//          2. Use key s to stop the animation.
void myKeyboardFunc(unsigned char key, int x, int y) {

	switch (key)
	{
	case ' ':
		glutIdleFunc(myIdleFunc);
		break;
	case 's':
		glutIdleFunc(NULL);
		break;
	default:
		break;
	}
}

//What: Function that moves the minutes hand
//Input: None
//Output: None
//Actions: 1. Increase the rotation angle;
//		   2. When minutes make a full rotation, call hours function
//         3. Call the display callback to update the image;
void moveMinutes() {
	//1. Increase the rotation angle;
	minuteAngle = minuteAngle + 6;
	//2. If minutes makes a full rotation...
	if (minuteAngle > 360.0) {
		minuteAngle = minuteAngle - 360.0; //Reset the angle 
		moveHours(); // Call hours function
	}

	//3. Update image
	glutPostRedisplay();
}

//What: Function that moves the hours hand
//Input: None
//Output: None
//Actions: 1. Increase the rotation angle;
//		   2. Call the display callback to update the image;
void moveHours() {
	//1. Increase the rotation angle
	hourAngle = hourAngle + 30;
	//When hour makes a full rotation
	if (hourAngle > 360.0) {
		hourAngle = hourAngle - 360.0; //Reset the angle
	}

	//2. Update Image
	glutPostRedisplay();
}

//What: Callback of glutIdleFunc()
//Input: None
//Output: None
//Actions: 1. Increase the rotation angle for the seconds hand;
//         2. When minutes make a full rotation, call minutes function;
//		   3. Increase the movement variable for Iris movement
//		   4. If the increment one if positive, and our move variable is 9; change one to negative
//		   5. If the increment is negative, and our move variable is less than -9; change one to positive
//		   6. Call the display callback to update the image;
void myIdleFunc() {

	timer = timer + 1;
	if (timer > 50) {
		timer = 0;
		//1. Increase the rotation angle for the seconds hand;
		spinAngle = spinAngle + 6;
		//2. When minutes make a full rotation
		if (spinAngle > 360.0) {
			spinAngle = spinAngle - 360.0; //Reset the angle 
			moveMinutes(); //Call minutes function
		}

		//3. Increase the movement variable for Iris movement
		moveX = moveX + one;
		//4. If the increment one if positive, and our move variable is 9; change one to negative
		if ((one > 0) && (moveX >= 9)) {
			//Now, whenever the display is updated, we subtract one, pushing our object in the opposite direction
			one = -1;
		}
		//5. If the increment is negative, and our move variable is less than -9; change one to positive
		if ((one < 0) && (moveX <= -9)) {
			//Now we changed it to positive, to move back to the original position
			one = 1;
		}
		
	}
	//6. Call the display callback to update the image
	glutPostRedisplay();
}

// What: A function that writes tick marks to simulate the numbers on a clock
// Input: An Integer for the center x-coordinate
//        An Integer for the center y-coordinate
//        An integer for the radius
//		  A vector for the colors
// Output: None. But creates equally spaced Tick Marks
// Action: 1. Define our function-exclusive values
//         2. Generate 12 points which will be our starting points
//         3. Generate 12 points which will be our ending points
//         4. Set the line width and line color
//		   5. Begin a loop to create the lines across the circle
// Caller: myDisplayFunc()
void createNumbers(int xcenter, int ycenter, int radius, vector<double> colors) {
	// 1. Define our function-exclusive values
	double theta;
	double x;
	double y;
	double circle = 2 * 3.14;
	// The lines points are generated by splitting the clock into 12 pieces,
	// So we want the bottom of the lines to be away from the top, so we define it as 5 less then the radius
	int radiusend = radius - 5;

	//Arrays that hold the coordinates for the start and end points for the lines
	int xBegin[12];
	int xEnd[12];
	int yBegin[12];
	int yEnd[12];

	// 2. Generate 12 points which will be our starting points
	for (int i = 0; i < 12; i++) {
		//This is the same function as in createEllipse
		theta = circle * (float(i) / float(12));
		x = xcenter + radius * cos(theta);
		y = ycenter + radius * sin(theta);
		//We have the coordinates for 12 points equally spaced along the clock face
		//They are the points closest to the edge of the clock
		xBegin[i] = x;
		yBegin[i] = y;
	}

	// 3. Generate 12 points which will be our ending points
	for (int i = 0; i < 12; i++) {
		theta = circle * (float(i) / float(12));
		x = xcenter + radiusend * cos(theta);
		y = ycenter + radiusend * sin(theta);
		//We have the coordinates for 12 points equally spaced along the clock face
		//They are the points closest to the center of the clock
		xEnd[i] = x;
		yEnd[i] = y;
	}

	// 4. Set the line width and line color
	glLineWidth(3);
	glColor3f(colors[0], colors[1], colors[2]);
	// 5. Begin a loop to create the lines across the circle
	for (int i = 0; i < 12; i++) {
		//Creating lines by connecting the 2 points
		glBegin(GL_LINES);
		glVertex2f(xBegin[i], yBegin[i]);
		glVertex2f(xEnd[i], yEnd[i]);
		glEnd();
	}

}

// What: Function for creating the clock hands
// Input: An array that holds 6 position coordinates, 
//		  An vector that holds three color values
// Output: None
// Action: 1. Set the color
//		   2. Create the shape for the clock hand
// Caller: myDisplayFunc()
void createHands(double pos[], vector<double> colors) {
	glColor3f(1.0, 0.42, 0.23);
	glColor3f(colors[0], colors[1], colors[2]);
	// 1. Set the color
	glBegin(GL_POLYGON);
	// 2. Create the shape for the Seconds hand
	glVertex2f(pos[0], pos[3]);
	glVertex2f(pos[1], pos[4]);
	glVertex2f(pos[0], pos[5]);
	glVertex2f(pos[2], pos[4]);
	glEnd();
}

// What: Function for creating a filled-in Ellipse
// Input: An Integer for the center x-coordinate
//        An Integer for the center y-coordinate
//        An Integer for the x-coordinates radius
//		  An Integer for the y-coordinates radius
//		  A vector that holds the colors
// Output: None
// Action: 1. Define our function-exclusive values
//         2. Define the color
//         3. Start the Triangle Fan
//         4. Loop to Create 100 points for the triangle fan
//		   5. Print out the points
// Caller: myDisplayFunc()
void createEllipse(int xcenter, int ycenter, int xradius, int yradius, vector<double> colors) {
	// 1. Define our function-exclusive values
	double theta;
	double x;
	double y;
	double circle = 2 * 3.14;
	// 2. Define the color
	glColor3f(colors[0], colors[1], colors[2]);
	// 3. Start the Triangle Fan
	glBegin(GL_TRIANGLE_FAN);
	// 4. Loop to Create 100 points for the triangle fan
	for (int i = 0; i < 100; i++) {
		theta = circle * (float(i) / float(100));
		x = xcenter + xradius * cos(theta);
		y = ycenter + yradius * sin(theta);
		// 5. Print out the point
		glVertex2f(x + xcenter, y + ycenter);
	}
	glEnd();

}

// What: Function for creating the ears
// Input: A set of Points
//		  A vector containing colors
// Output: None. But creates a Triangle
// Action: 1. Set the color
//		   2. Create the shape for the Ears
//		   3. Mirror the ear with negative x values
// Caller: myDisplayFunc()
void createEars(double pos[], vector<double> colors) {
	// 1. Set the color
	glColor3f(colors[0], colors[1], colors[2]);
	// 2. Create the shape for the Ears
	glBegin(GL_POLYGON);        
	glVertex2f(pos[0], pos[1]);
	glVertex2f(pos[2], pos[3]);
	glVertex2f(pos[4], pos[5]);
	glEnd();
	// 3. Mirror the ear with negative x values
	glBegin(GL_POLYGON);        
	glVertex2f(-pos[0], pos[1]);
	glVertex2f(-pos[2], pos[3]);
	glVertex2f(-pos[4], pos[5]);
	glEnd();
}

// What: Specialized Ellipse Function that is used for the Iris Specifically
// Input: An Integer for the center x-coordinate
//        An Integer for the center y-coordinate
//        An Integer for the x-coordinates radius
//		  An Integer for the y-coordinates radius
// Output: None. It creates two Ellipse for the Iris
// Action: 1. Same as the create Ellipse Function
//         2. Creates a second Ellipse with negative x-values
// Caller: myDisplayFunc()
void createIris(int xcenter, int ycenter, int xradius, int yradius) {
	// 1. Same as the createEllipse Function
	double theta;
	double x;
	double y;
	double circle = 2 * 3.14;
	glColor3f(0.0, 0.0, 0.0); 
	glBegin(GL_TRIANGLE_FAN); 
	for (int i = 0; i < 100; i++) {
		theta = circle * (float(i) / float(100));
		x = xcenter + xradius * cos(theta);
		y = ycenter + yradius * sin(theta);
		glVertex2f(x + xcenter, y + ycenter);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);        
	for (int i = 0; i < 100; i++) {
		theta = circle * (float(i) / float(100));
		x = -xcenter + xradius * cos(theta);
		y = ycenter + yradius * sin(theta);
		glVertex2f(x + (-xcenter), y + ycenter);
	}
	glEnd();
}

// What: Funciton for creating Whiskers
// Input: An array that holds the x coordinates for a line, an array that holds the y-coordinates for a line
//		  A vector that holds colors
// Output: None. Creates all 6 whiskers
// Action: 1. Specify the color
//		   2. Create a loop to go through the arrays
//		   3. Create a between the two points
//		   4. Create a mirror image across the x-axis
// Caller: myDisplayFunc()
void createWhiskers(double posx[], double posy[], vector<double> colors) {

	// 1. Specify the color
	glColor3f(colors[0], colors[1], colors[2]);
	// 2. Create a loop to go through the arrays
	for (int i = 0; i < 6; i = i + 2) {
		// 3. Create a between the two points
		// The array holds the points as beginPoint1, endPoint1, beginPoint2, endPoint2 etc
		// So we make the line from i to i+1, and then increment by 2 so we go to the next beginPoint
		glBegin(GL_LINES);
		glVertex2f(posx[i], posy[i]);
		glVertex2f(posx[i + 1], posy[i + 1]);
		glEnd();
	}

	// 4. Create a mirror image across the x - axis
	// WE accomplish this by mapping all the posx values to negative
	glColor3f(colors[0], colors[1], colors[2]);
	for (int i = 0; i < 6; i = i + 2) {
		glBegin(GL_LINES);
		glVertex2f(-(posx[i]), posy[i]);
		glVertex2f(-(posx[i + 1]), posy[i + 1]);
		glEnd();
	}

}

// What: Function that creates the cats nose
// Input: A vector containing posititon variables
//        A vector holding color values
// Output: None
// Action: 1. Specify the color
//		   2. Create a polygon that connects the points
// Caller: myDisplayFunc()
void createNose(double pos[], vector<double> colors) {
	//1. Specify the color
	glColor3f(colors[0], colors[1], colors[2]);
	//2. Create a polygon that connects the points
	glBegin(GL_POLYGON);        
	glVertex2f(pos[0], pos[1]);
	glVertex2f(pos[2], pos[3]);
	glVertex2f(pos[4], pos[5]);
	glEnd();
}

// What: A specialized version of createEllipse that creates the cats lips
// Input: An Integer for the center x-coordinate
//        An Integer for the center y-coordinate
//        An integer for the radius
//		  A vector containing color values
// Output: None. It creates two circles for the cat lips
// Action: 1. Same as createEllipse
//		   2. If y is bigger than the the center height, make all points black
//		   3. If y is less than the center height, make all points white
//		   4. Create the other lip by moving the circle along the x-axis
// Caller: myDisplayFunc()
void createLips(int xcenter, int ycenter, int radius, vector<double> colors) {
	// 1. Same as createEllipse
	double theta;
	double x;
	double y;
	double circle = 2 * 3.14;

	glLineWidth(3);
	glBegin(GL_LINE_LOOP);        
	for (int i = 0; i < 100; i++) {
		theta = circle * (float(i) / float(100));
		x = xcenter + radius * cos(theta);
		y = ycenter + radius * sin(theta);
		if ((y) >= ycenter) {
			// 2. If y is bigger than the the center height, make all points black
			glColor3f(colors[0], colors[1], colors[2]);
			glVertex2f(x + xcenter, y + ycenter);
		}
		else {
			// 3. If y is less than the center height, make all points white
			glColor3f(colors[3], colors[4], colors[5]);
			glVertex2f(x + xcenter, y + ycenter);
		}
	}
	glEnd();

	// 4. Create the other lip by moving the circle along the x-axis
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++) {
		theta = circle * (float(i) / float(100));
		x = xcenter + radius * cos(theta);
		y = ycenter + radius * sin(theta);
		if ((y) >= ycenter) {
			glColor3f(colors[0], colors[1], colors[2]);
			glVertex2f(x + (xcenter + (radius * 2)), y + (ycenter));
		}
		else {
			glColor3f(colors[3], colors[4], colors[5]);
			glVertex2f(x + (xcenter + (radius * 2)), y + (ycenter));
		}
	}
	glEnd();

}

// What: Creates the outline for the clock
// Input: An Integer for the center x-coordinate
//        An Integer for the center y-coordinate
//        An integer for the radius
//        A vector of values for color
// Output: None. But creates a circle when run.
// Action: 1. Same as createEllipse
// Caller: myDisplayFunc()
void createOutline(int xcenter, int ycenter, int radius, vector<double> colors, int thick) {
	// 1. Same as createEllipse
	double theta;
	double x;
	double y;
	double circle = 2 * 3.14;

	glLineWidth(thick);
	glColor3f(colors[0], colors[1], colors[2]);
	// Here we use a LINE_LOOP instead of a Triangle Fan
	// This is because Triangle_Fan creates a filled-in circle, while Line Loop doesn't
	// And we want this to go outside the clock as an outline
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++) {
		theta = circle * (float(i) / float(100));
		x = xcenter + (radius)*cos(theta);
		y = ycenter + (radius)*sin(theta);
		glVertex2f(x + xcenter, y + ycenter);
	}
	glEnd();
}

// What: Funciton that creates a title
// Input: A boolean value that specifies whether it should display
// Output: None. Creates a title
// Action: 1. Set the position of the text
//		   2. Set an array of characters that contain the text
//		   3. Loop through the array to display the characters
// Caller: myDisplayFunc()
void createTitle(bool isOn) {
	// 1. Set the position of the text
	glRasterPos2i(-16, 90);
	// 2. Set an array of characters that contain the text
	char text[] = { 'C', 'a', 't', ' ', 'C', 'l', 'o', 'c', 'k'};
	if (isOn == true) {
		// 3. Loop through the array to display the characters
		for (int i = 0; i < 9; i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
		}
	}
}

// What: Function that sets up a menu
// Input: None
// Output: None
// Action: 1. Create a new pop-up menu
//		   2. Create the callback function
//		   3. Add menu entries or sub-menu(s) to the menu
//		   4: Attach the menu to a mouse button
void setupMenu()
{
	//This is a sub-menu that holds all the color themes.
	int color_menu;

	// Step 1: Create a new pop-up menu
	// Step 2: Create the callback function
	color_menu = glutCreateMenu(colorMenu);

	//Step 3: Add menu entries to the color_menu
	glutAddMenuEntry("Wall clock", 0);//Basic Wall clock
	glutAddMenuEntry("Cat clock", 1);//The clock looks like the cat
	glutAddMenuEntry("White clock", 2);//The wall clock is now a white cat
	glutAddMenuEntry("Rainbow clock", 3);//The clock has a rainbow pallete

	// Step 1: Create a new pop-up menu
	// Step 2: Create the callback function
	//This is the main menu that appears when you right-click the mouse
	int main_menu;
	main_menu = glutCreateMenu(mainMenu);

	//Step 3: Add menu entries and sub-menu to the menu
	glutAddSubMenu("Color", color_menu);//The color_menu sub-menu
	glutAddMenuEntry("Toggle name", 4);//Option that displays name or not
	glutAddMenuEntry("Exit", 5);//Exit option
	//Step 4: Attach the menu to a mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// What: Function that creates the main menu
// Input: an integer value for the switch statement
// Output: None. But creates a menu
// Action: 1. Read the menuEntryId, and assign it to it's proper menu entry
//		   2. Call the color menu, and pass the menuEntry in
//		   3. Switch the value of isOn, and then create the title
//		   4. Exit the program
//		   5. Update the image
void mainMenu(int menuEntryId)
{
	//1. Read the menuEntryId, and assign it to it's proper menu entry
	switch (menuEntryId)
	{
	case 0:	
	case 1:
	case 2:
	case 3:
		//2. Call the color menu, and pass the menuEntry in
		colorMenu(menuEntryId);
		//5. Update the image
		glutPostRedisplay();
		break;
	case 4:
		//3. Switch the value of isOn, and then create the title
		isOn = !isOn;
		createTitle(isOn);
		//5. Update the image
		glutPostRedisplay();
		break;
	case 5:
		//4. Exit the program
		exit(0);
		break;
	default:
		break;
	}
}

// What: Function that picks the theme of the image
// Input: An integer in [0, 3] to represents one of the themes
// Output: None
// Actions: 1. Read the menuEntryId, and assign it to it's proper menu entry
//          2. Update the theme
//			3. Update the image
void colorMenu(int menuEntryId)
{
	//1. Read the menuEntryId, and assign it to it's proper menu entry
	switch (menuEntryId)
	{
	case 0:
		//2. Update the theme
		theme = menuEntryId;
		//3. Update the image
		glutPostRedisplay();
		break;
	case 1:
		//2. Update the theme
		theme = menuEntryId;
		//3. Update the image
		glutPostRedisplay();
		break;
	case 2:
		//2. Update the theme
		theme = menuEntryId;
		//3. Update the image
		glutPostRedisplay();
		break;
	case 3:
		//2. Update the theme
		theme = menuEntryId;
		//3. Update the image
		glutPostRedisplay();
		break;
	default:
		break;
	}
	//3. Update the image
	glutPostRedisplay();
}

// What: Function that passes an array of colors based on the theme
// Input: None. Uses the Global variable theme
// Output: A vector of 3 colors
// Actions: 1. Initialize the vector color
//          2. Fill the vector with the values
//			3. Return the vector
// Used by: clockColors
vector<double> clockReColor()
{
	//1. Initialize the vector color
	vector<double> color;
	switch (theme) {
	case 0:
	case 2:
		//2. Fill the vector with the values
		color.push_back(1.0);
		color.push_back(0.9);
		color.push_back(0.59);
		//3. Return the vector
		return color;
		break;

	case 1:
		//2. Fill the vector with the values
		color.push_back(0.9);
		color.push_back(0.93);
		color.push_back(0.3);
		//3. Return the vector
		return color;
		break;
	case 3:
		//2. Fill the vector with the values
		color.push_back(0.58);
		color.push_back(0.94);
		color.push_back(0.07);
		//3. Return the vector
		return color;
		break;
	default:
		break;
	}
	
}

// What: Function that passes an array of colors based on the theme
// Input: None. Uses the Global variable theme
// Output: A vector of 3 colors
// Actions: 1. Initialize the vector color
//          2. Fill the vector with the values
//			3. Return the vector
// Used by: eyeColors
vector<double> eyeReColor()
{
	vector<double> color;
	switch (theme) {
	case 0:
	case 1:
	
		color.push_back(0.9);
		color.push_back(0.93);
		color.push_back(0.3);
		return color;
		break;
	case 2:
		color.push_back(0.2);
		color.push_back(0.77);
		color.push_back(0.78);
		return color;
		break;
	case 3:
		color.push_back(1);
		color.push_back(0.7);
		color.push_back(0.0);
		return color;
		break;
	default:
		break;
	}

}

// What: Function that passes an array of colors based on the theme
// Input: None. Uses the Global variable theme
// Output: A vector of 3 colors
// Actions: 1. Initialize the vector color
//          2. Fill the vector with the values
//			3. Return the vector
// Used by: catColors
vector<double> catReColor()
{
	vector<double> color;
	switch (theme) {
	case 0:
	case 1:
		color.push_back(0.0);
		color.push_back(0.0);
		color.push_back(0.0);
		return color;
		break;
	case 2:
		color.push_back(1.0);
		color.push_back(1.0);
		color.push_back(1.0);
		return color;
		break;
	case 3:
		color.push_back(1);
		color.push_back(0);
		color.push_back(0);
		return color;
		break;
	default:
		break;
	}

}

// What: Function that passes an array of colors based on the theme
// Input: None. Uses the Global variable theme
// Output: A vector of 3 colors
// Actions: 1. Initialize the vector color
//          2. Fill the vector with the values
//			3. Return the vector
// Used by: earColors
vector<double> earReColor()
{
	vector<double> color;
	switch (theme) {
	case 0:
	case 1:
		color.push_back(1);
		color.push_back(1);
		color.push_back(1);
		return color;
		break;
	case 2:
		color.push_back(1.0);
		color.push_back(0.64);
		color.push_back(0.94);
		return color;
		break;
	case 3:
		color.push_back(0.86);
		color.push_back(0.22);
		color.push_back(0.0);
		return color;
		break;
	default:
		break;
	}

}

// What: Function that passes an array of colors based on the theme
// Input: None. Uses the Global variable theme
// Output: A vector of 3 colors
// Actions: 1. Initialize the vector color
//          2. Fill the vector with the values
//			3. Return the vector
// Used by: noseColors
vector<double> noseReColor()
{
	vector<double> color;
	switch (theme) {
	case 0:
	case 1:
		color.push_back(0.19);
		color.push_back(0.17);
		color.push_back(0.17);
		return color;
		break;
	case 2:
		color.push_back(1.0);
		color.push_back(0.64);
		color.push_back(0.94);
		return color;
		break;
	case 3:
		color.push_back(0.96);
		color.push_back(0.77);
		color.push_back(0.05);
		return color;
		break;
	default:
		break;
	}

}

// What: Function that passes an array of colors based on the theme
// Input: None. Uses the Global variable theme
// Output: A vector of 3 colors
// Actions: 1. Initialize the vector color
//          2. Fill the vector with the values
//			3. Return the vector
// Used by: secondsColors
vector<double> secondsReColor()
{
	vector<double> color;
	switch (theme) {
	case 0:
	case 2:
		color.push_back(1.0);
		color.push_back(0.42);
		color.push_back(0.23);
		return color;
		break;
	case 1:
		color.push_back(0.33);
		color.push_back(0.33);
		color.push_back(0.33);
		return color;
		break;
	case 3:
		color.push_back(0.8);
		color.push_back(0.87);
		color.push_back(0.57);
		return color;
		break;
	default:
		break;
	}

}

// What: Function that passes an array of colors based on the theme
// Input: None. Uses the Global variable theme
// Output: A vector of 3 colors
// Actions: 1. Initialize the vector color
//          2. Fill the vector with the values
//			3. Return the vector
// Used by: minutesColors
vector<double> minutesReColor()
{
	vector<double> color;
	switch (theme) {
	case 0:
	case 2:
		color.push_back(0.87);
		color.push_back(0.29);
		color.push_back(0.19);
		return color;
		break;
	case 1:
		color.push_back(0.19);
		color.push_back(0.17);
		color.push_back(0.17);
		return color;
		break;
	case 3:
		color.push_back(0.07);
		color.push_back(0.0);
		color.push_back(0.9);
		return color;
		break;
	default:
		break;
	}

}

// What: Function that passes an array of colors based on the theme
// Input: None. Uses the Global variable theme
// Output: A vector of 3 colors
// Actions: 1. Initialize the vector color
//          2. Fill the vector with the values
//			3. Return the vector
// Used by: hoursColors
vector<double> hoursReColor()
{
	vector<double> color;
	switch (theme) {
	case 0:
	case 2:
		color.push_back(0.7);
		color.push_back(0.17);
		color.push_back(0.0);
		return color;
		break;
	case 1:
		color.push_back(1);
		color.push_back(1);
		color.push_back(1);
		return color;
		break;
	case 3:
		color.push_back(0.35);
		color.push_back(0.08);
		color.push_back(0.63);
		return color;
		break;
	default:
		break;
	}

}

// What: Function that passes an array of colors based on the theme
// Input: None. Uses the Global variable theme
// Output: A vector of 3 colors
// Actions: 1. Initialize the vector color
//          2. Fill the vector with the values
//			3. Return the vector
// Used by: whiskerColors
vector<double> whiskerReColor()
{
	vector<double> color;
	switch (theme) {
	case 0:
	case 1:
		color.push_back(0.33);
		color.push_back(0.33);
		color.push_back(0.33);
		return color;
		break;
	case 2:
		color.push_back(0.95);
		color.push_back(0.95);
		color.push_back(0.95);
		return color;
		break;
	case 3:
		color.push_back(0.95);
		color.push_back(1);
		color.push_back(0.08);
		return color;
		break;
	default:
		break;
	}

}

// What: Function that passes an array of colors based on the theme
// Input: None. Uses the Global variable theme
// Output: A vector of 3 colors
// Actions: 1. Initialize the vector color
//          2. Fill the vector with the values
//			3. Return the vector
// Used by: centerColors
vector<double> centerReColor()
{
	vector<double> color;
	switch (theme) {
	case 0:
	case 1:
		color.push_back(0.0);
		color.push_back(0.0);
		color.push_back(0.0);
		return color;
		break;
	case 2:
		color.push_back(1.0);
		color.push_back(1.0);
		color.push_back(1.0);
		return color;
		break;
	case 3:
		color.push_back(0.78);
		color.push_back(0.05);
		color.push_back(0.87);
		return color;
		break;
	default:
		break;
	}

}

// What: Function that passes an array of colors based on the theme
// Input: None. Uses the Global variable theme
// Output: A vector of 3 colors
// Actions: 1. Initialize the vector color
//          2. Fill the vector with the values
//			3. Return the vector
// Used by: numbersColors
vector<double> numberReColor()
{
	vector<double> color;
	switch (theme) {
	case 0:
	case 1:
	case 2:
		color.push_back(0);
		color.push_back(0);
		color.push_back(0);
		return color;
		break;
	case 3:
		color.push_back(0.05);
		color.push_back(0.85);
		color.push_back(0.0);
		return color;
		break;
	default:
		break;
	}

}

// What: Function that passes an array of colors based on the theme
// Input: None. Uses the Global variable theme
// Output: A vector of 6 colors
// Actions: 1. Initialize the vector color
//          2. Fill the vector with the values
//			3. Return the vector
// Used by: lipsColors
vector<double> lipReColor()
{
	vector<double> color;
	switch (theme) {
	case 0:
	case 1:
		//This one contains 6 colors
		//This is because half the ellipse that make the lips is the same color as the cat
		//And the other half is the visible color
		color.push_back(0);
		color.push_back(0);
		color.push_back(0);
		color.push_back(1);
		color.push_back(1);
		color.push_back(1);
		return color;
		break;
	case 2:
		color.push_back(1);
		color.push_back(1);
		color.push_back(1);
		color.push_back(0.92);
		color.push_back(0);
		color.push_back(0.55);
		return color;
		break;
	case 3:
		color.push_back(1);
		color.push_back(0);
		color.push_back(0);
		color.push_back(0);
		color.push_back(0);
		color.push_back(0);
		return color;
		break;
	default:
		break;
	}

}

// Chick.cpp : Defines the entry point for the console application.
//
// Authors  : Tamsyn Murphy and Seth Luther
// Date    : 12/05/2014
// Version : 1.0
//
// This program displays a chick which is made up of the letters S u L t M N.  The wings are able to move as well as the feet and legs.
//

// Program behaviour:
// Key 'x/X'	: Rotates the chick around the X axis
// Key 'y/Y'	: Rotates the chick around the Y axis
// Key 'z/Z'	: Rotates the chick around the Z axis
// Key 'f/F'	: Toggle Floor On/Off
// Key 'm/M'	: Cycles through the different models
// Key 'w/W'	: Flap the first wing
// Key 'q/Q'	: Flap the second wing
// Key 'l/L'	: Moves the legs in opposite directions
// Key 'h/H'	: Moves the feet up and down
// Key 'd/D'	: Zooms the view in or out
// Key 'a/b/A'	: View or hide the gridlines for the different axis

//All the files and libraries to be included
#include "stdafx.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

//Number of models & grids
#define NUMBER_OF_MODELS 9

//======================================================
// GLOBAL VARIABLES 
//======================================================
//For the model, floor and grid display
int current_model = 1;
int dont_display_floor = 0;
int x_y_display = 0, y_z_display = 0, x_z_display = 0;

//For the view control
static float G_theta[3]; // View X,Y,Z
static float G_zoom = 0.3;

//bool MousePressed; // Used in Mouse Drag to change the Viewpoint
//float pitch0, yaw0;
//int mouseX0, mouseY0;

//Declaring the values for all the angles
static float wingAngle = 0, footAngle = 0, legAngle = 0, wingAngle2 = 0;
float thetaX = 0.0;
float thetaY = 0.0;
float thetaZ = 0.0;

//======================================================
// PROTOTYPES 
//======================================================
void drawScene(float wingAngle, float footAngle, float legAngle, float wingAngle2);
void drawChick(float wingAngle, float footAngle, float legAngle, float wingAngle2);
void drawLeg(float footAngle, float legAngle);
void drawFoot(float footAngle);
void drawEye();
void drawBody();
void drawMouth();
void drawWing();
void drawWing2();

//======================================================
// CUBE ROUTINES
//======================================================
//Cube points
float vertices[][3] = 
{
		{-0.5,-0.5,-0.5},{0.5,-0.5,-0.5},
		{0.5,0.5,-0.5}, {-0.5,0.5,-0.5}, {-0.5,-0.5,0.5}, 
		{0.5,-0.5,0.5}, {0.5,0.5,0.5}, {-0.5,0.5,0.5}
};

//======================================================
// DRAW POLYGON
//======================================================
void polygon(int a, int b, int c , int d)
{
	//Draw a polygon 
 	glBegin(GL_POLYGON);
		glVertex3fv(vertices[a]);
		glVertex3fv(vertices[b]);
		glVertex3fv(vertices[c]);
		glVertex3fv(vertices[d]);
	glEnd();
	
	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINE_LOOP);
		glVertex3fv(vertices[a]);
		glVertex3fv(vertices[b]);
		glVertex3fv(vertices[c]);
		glVertex3fv(vertices[d]);
	glEnd();
	glDisable(GL_LINE_SMOOTH);
}

//======================================================
// DRAW CUBE
//======================================================
void drawCube()
{
	//Draw a cube centred on the origin
	polygon(0,3,2,1);
	polygon(2,3,7,6);
	polygon(4,7,3,0);
	polygon(1,2,6,5);
	polygon(7,4,5,6);
	polygon(5,4,0,1);
}

//======================================================
// DRAW CURVE
//======================================================
void drawCurve(float startAngle, float endAngle)
{
	//Draw a curve
	float PI = 3.142;
	float r1 = 0.4;
	float r2 = 0.6;

	//Front part of curve
	glBegin(GL_QUAD_STRIP);
		for (float thetaDeg = startAngle; thetaDeg <= endAngle; thetaDeg++)
		{
			float thetaRad = thetaDeg * PI / 180.0;
			//Inner radius 
			glVertex3f(r1 * cos(thetaRad), r1 * sin(thetaRad), 0.5);
			//Outer radius 
			glVertex3f(r2 * cos(thetaRad), r2 * sin(thetaRad), 0.5);
		}
	glEnd();

	//Back part of curve
	glBegin(GL_QUAD_STRIP);
		for (float thetaDeg = startAngle; thetaDeg <= endAngle; thetaDeg++)
		{
			float thetaRad = thetaDeg * PI / 180.0;
			//Inner radius 
			glVertex3f(r1 * cos(thetaRad), r1 * sin(thetaRad), -0.5);
			//Outer radius 
			glVertex3f(r2 * cos(thetaRad), r2 * sin(thetaRad), -0.5);
		}
	glEnd();

	//Inner part of curve
	glBegin(GL_QUAD_STRIP);
		for (float thetaDeg = startAngle; thetaDeg <= endAngle; thetaDeg++)
		{
			float thetaRad = thetaDeg * PI / 180.0;
			//Inner radius 
			glVertex3f(r1 * cos(thetaRad), r1 * sin(thetaRad), -0.5);
			//Outer radius 
			glVertex3f(r1 * cos(thetaRad), r1 * sin(thetaRad), 0.5);
		}
	glEnd();

	//Outer part of curve
	glBegin(GL_QUAD_STRIP);
		for (float thetaDeg = startAngle; thetaDeg <= endAngle; thetaDeg++)
		{
			float thetaRad = thetaDeg * PI / 180.0;
			//Inner radius 
			glVertex3f(r2 * cos(thetaRad), r2 * sin(thetaRad), -0.5);
			//Outer radius 
			glVertex3f(r2 * cos(thetaRad), r2 * sin(thetaRad), 0.5);
		}
	glEnd();

	//Cap at the start angle 
	glBegin(GL_POLYGON);
		float startRad = startAngle * PI / 180;
		glVertex3f(r1 * cos(startRad), r1 * sin(startRad), 0.5);
		glVertex3f(r2 * cos(startRad), r2 * sin(startRad), 0.5);
		glVertex3f(r2 * cos(startRad), r2 * sin(startRad), -0.5);
		glVertex3f(r1 * cos(startRad), r1 * sin(startRad), -0.5);
	glEnd();

	//Cap at the end angle 
	glBegin(GL_POLYGON);
		float endRad = endAngle * PI / 180;
		glVertex3f(r1 * cos(endRad), r1 * sin(endRad), 0.5);
		glVertex3f(r1 * cos(endRad), r1 * sin(endRad), -0.5);
		glVertex3f(r2 * cos(endRad), r2 * sin(endRad), -0.5);
		glVertex3f(r2 * cos(endRad), r2 * sin(endRad), 0.5);
	glEnd();
}

//======================================================
// DRAW AXES and GRIDS
//======================================================
void drawAxes(void)
{
	float offset;
	int gd;
	glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(-20, 0, 0);
		glVertex3f(+20, 0, 0);
		glVertex3f(0, -20, 0);
		glVertex3f(0, +20, 0);
		glVertex3f(0, 0, -20);
		glVertex3f(0, 0, +20);
	glEnd();

	glLineStipple(1, 0xAAAA); //line style = fine dots
	glEnable(GL_LINE_STIPPLE);

	glBegin(GL_LINES);
		if (x_y_display) {
			glColor3f(0.0, 0.7, 0.7);
			for (offset = -10.0; offset<10.1; offset++){
				//draw lines in x-y plane
				glVertex3f(-10.0, offset, 0.0);					// Top Left
				glVertex3f(+10.0, offset, 0.0);					// Top Right
				glVertex3f(offset, -10, 0.0);					// Bottom Right
				glVertex3f(offset, +10.0, 0.0);					// Bottom Left
			}
		}

		if (y_z_display) {
			glColor3f(0.7, 0.0, 0.7);
			for (offset = -10.0; offset<10.1; offset++){
				//draw lines in y-z plane
				glVertex3f(0, offset, -10);
				glVertex3f(0, offset, 10.0);
				glVertex3f(0, -10, offset);
				glVertex3f(0, 10, offset);
			}
		}

		if (x_z_display) {
			glColor3f(0.7, 0.7, 0.0);
			for (offset = -10.0; offset<10.1; offset++){
				//draw lines in x-z plane
				glVertex3f(offset, 0, -10);
				glVertex3f(offset, 0, 10.0);
				glVertex3f(-10, 0, offset);
				glVertex3f(10, 0, offset);
			}
		}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

//======================================================
// DRAW_FLOOR
//======================================================
void drawFloor()
{
	if (dont_display_floor) return;
	//Just draw a square for the floor
	glPushMatrix();
		glTranslatef(0, -1.5, 0); //Draws the floor below 0 so it doesn't draw straight through the animal
		glBegin(GL_POLYGON);
			glColor3f(.75, .75, .75);
			glVertex3f(-10, 0, 10);
			glVertex3f(-10, 0, -10);
			glVertex3f(10, 0, -10);
			glVertex3f(10, 0, 10);
		glEnd();
	glPopMatrix();
}

//======================================================
// DRAW_SCENE 
//======================================================
void drawScene(float wingAngle, float footAngle, float legAngle, float wingAngle2)
{
	//Draw axes
	drawAxes();

	//Draw floor
	drawFloor();

	//No Translation
	//No Rotation
	//Scale the entire drawing bigger so can it can be seen more easily
	glScalef(2.5, 2.5, 2.5);

	glPushMatrix();
		switch (current_model)
		{
		case 1:
			drawChick(wingAngle, footAngle, legAngle, wingAngle2);
			printf("Model = chick\n");
			break;
		case 2:
			drawCube(); printf("Model = cube\n");
			break;
		case 3:
			drawLeg(footAngle, legAngle); printf("Model = legs\n");
			break;
		case 4:
			drawBody();
			printf("Model =  body\n");
			break;
		case 5:
			drawMouth();
			printf("Model = mouth\n");
			break;
		case 6:
			drawWing();
			printf("Model = wing1\n");
		case 7:
			drawWing2();
			printf("Model = wing2\n");
		case 8:
			drawFoot(footAngle);
			printf("Model = feet\n");
		case 9: 
			drawEye();
			printf("Model = eye\n");

		default:
			printf("Unknown model\n");
		}
	glPopMatrix();
}

//======================================================
// DRAW CHICK
//======================================================
void drawChick(float wingAngle, float footAngle, float legAngle, float wingAngle2)
{
	//Draw the Body
	glPushMatrix();
		drawBody();
	glPopMatrix();

	//Draw the Leg and position it accordingly
	glPushMatrix();
		//Using a double translation to move the leg to the origin, performing the rotation and placing it back in its' original position
		glTranslatef(-0.15, 0.0, -0.025);		
		glRotatef(legAngle, 0.0, 0.0, 0.5);
		glTranslatef(-0.05, 0.0, -0.05);
		drawLeg(legAngle, footAngle);
	glPopMatrix();

	//Draw the other leg and position it accordingly
	glPushMatrix();
		//Using a double translation to move the leg to the origin, performing the rotation and placing it back in its' original position
		glTranslatef(-0.15, 0.0, 0.1);
		glRotatef(-legAngle, 0.0, 0.0, 0.5);
		glTranslatef(-0.05, 0.0, 0.2);
		drawLeg(legAngle, footAngle);
	glPopMatrix();
}

//======================================================
// DRAW BODY
//======================================================
void drawBody()
{
	//Draw the Eye
	glPushMatrix();
		drawEye();
	glPopMatrix();

	//Draw the Mouth and position it correctly
	glPushMatrix();
		glTranslatef(0.0, 0.0, 0.011);
		drawMouth();
	glPopMatrix();

	//Using a double translation to move the furthest wing to the origin, performing the rotation and placing it back in its' original position
	glPushMatrix();
		glTranslatef(0.3, 0.0, -0.004);
		glRotatef(-wingAngle, -1.0, 0, 0);
		glTranslatef(-0.15, 0.0, -0.02);
		drawWing2();
	glPopMatrix();

	//Using a double translation to move the closest wing to the origin, performing the rotation and placing it back in its' original position
	glPushMatrix();
		glTranslatef(0.3, 0.0, 0.2);
		glRotatef(-wingAngle2, 1.0, 0, 0);
		glTranslatef(-0.15, 0.0, 0.4);
		drawWing();
	glPopMatrix();

	//S - Top curve of body 
	glPushMatrix();
		glColor3f(1, 1, 0);  //Yellow
		glTranslatef(-0.05, 0.5, -0.01);
		glScalef(0.4, 0.4, 0.3);
		drawCurve(0.0, 180.0);
	glPopMatrix();

	//S - Bottom curve of body
	glPushMatrix();
		glColor3f(1, 1, 0);  //Yellow
		glTranslatef(-0.45, 0.5, -0.01);
		glRotatef(180.0, 1, 0, 0);
		glScalef(0.4, 0.8, 0.3);
		drawCurve(0.0, 140.0);
	glPopMatrix();
	
	//Curve of u for bottom part of body 
	glPushMatrix();
		glColor3f(1, 1, 0);  //Yellow
		glTranslatef(-0.25, 0.16, 0.0);
		glRotatef(180.0, 0, 0, 1);
		glScalef(0.65, 0.90, 0.3);
		drawCurve(0.0, 180.0);
	glPopMatrix();

	//Cube of u for bottom part of body 
	glPushMatrix();
		glColor3f(1, 1, 0);  //Yellow
		glTranslatef(0.1, -0.11, 0.0);
		glRotatef(40, 0.0, 0.0, 0.0);
		glScalef(0.12, 0.7, 0.4);
		drawCube();
	glPopMatrix();
}

//======================================================
// DRAW EYE
//======================================================
void drawEye()
{
	//Left side of N front  | 
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0); //Black
		glTranslatef(-0.05, 0.5, 0.1);
		glRotatef(0, 0, 0, 0);
		glScalef(0.15, 0.05, 0.05);
		drawCube();
	glPopMatrix();

	//Right side of N front   | 
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0); //Black
		glTranslatef(-0.05, 0.38, 0.1);
		glRotatef(0, 0, 0, 0);
		glScalef(0.15, 0.05, 0.05);
		drawCube();
	glPopMatrix();

	//Middle of N left front  \ 
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0); //Black
		glTranslatef(-0.05, 0.45, 0.1);
		glRotatef(45, 0, 0, 1);
		glScalef(0.15, 0.05, 0.05);
		drawCube();
	glPopMatrix();

	//Neft side of N back   | 
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0); //Black
		glTranslatef(-0.05, 0.5, -0.1);
		glRotatef(0, 0, 0, 0);
		glScalef(0.15, 0.05, 0.05);
		drawCube();
	glPopMatrix();

	//Right side of N back   | 
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0); //Black
		glTranslatef(-0.05, 0.38, -0.1);
		glRotatef(0, 0, 0, 0);
		glScalef(0.15, 0.05, 0.05);
		drawCube();
	glPopMatrix();

	//Middle of N left back  \ 
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.0); //Black
		glTranslatef(-0.05, 0.45, -0.1);
		glRotatef(45, 0, 0, 1);
		glScalef(0.15, 0.05, 0.05);
		drawCube();
	glPopMatrix();
}

//======================================================
// DRAW MOUTH
//======================================================
void drawMouth()
{
	//Left side of M front   | 
	glPushMatrix();
		glColor3f(1.0, 0.5, 0);  //Orange
		glTranslatef(0.24, 0.48, -0.02);
		glRotatef(0, 0, 0, 0);
		glScalef(0.25, 0.05, 0.3);
		drawCube();
	glPopMatrix();

	//Right side of M front   | 
	glPushMatrix();
		glColor3f(1.0, 0.5, 0);  //Orange
		glTranslatef(0.25, 0.18, -0.02);
		glRotatef(0, 0, 0, 0);
		glScalef(0.25, 0.05, 0.3);
		drawCube();
	glPopMatrix();

	//Middle of M left front  \ 
	glPushMatrix();
		glColor3f(1.0, 0.5, 0);  //Orange
		glTranslatef(0.3, 0.4, -0.02);
		glRotatef(45, 0, 0, 1);
		glScalef(0.20, 0.05, 0.3);
		drawCube();
	glPopMatrix();

	//Middle of M right front   / 
	glPushMatrix();
		glColor3f(1.0, 0.5, 0);  //Orange
		glTranslatef(0.3, 0.25, -0.02);
		glRotatef(135, 0, 0, 1);
		glScalef(0.20, 0.05, 0.3);
		drawCube();
	glPopMatrix();
}

//======================================================
// DRAW CLOSEST WING
//======================================================
void drawWing()
{
	glPushMatrix();
		glScalef(2.0, 2.0, 2.0);

		//Top cube of t | 
		glPushMatrix();
			glColor3f(1.0, 0.5, 0);  //Orange
			glTranslatef(-0.32, 0.08, -0.15);
			glRotatef(42, 0, 0, 1);
			glScalef(0.06, 0.06, 0.03);
			drawCube();
		glPopMatrix();

		//Across cube of t -  
		glPushMatrix();
			glColor3f(1.0, 0.5, 0);  //Orange
			glTranslatef(-0.3, 0.05, -0.15);
			glRotatef(45, 0, 0, 1);
			glRotatef(92, 0, 1, 0);
			glScalef(0.15, 0.025, 0.1);
			drawCube();
		glPopMatrix();

		//Bottom curve of t 
		glPushMatrix();
			glColor3f(1.0, 0.5, 0);  //Orange
			glTranslatef(-0.27, 0.09, -0.15);
			glRotatef(-135.0, 0, 0, 1);
			glScalef(0.1, 0.15, 0.05);
			drawCurve(0.0, 180.0);
		glPopMatrix();
	glPopMatrix();
}

//======================================================
// DRAW FURTHEST WING
//======================================================
void drawWing2()
{
	glPushMatrix();
		glScalef(2.0, 2.0, 2.0);

		//Top cube of t | 
		glPushMatrix();
			glColor3f(1.0, 0.5, 0);  //Orange
			glTranslatef(-0.32, 0.08, -0.15);
			glRotatef(42, 0, 0, 1);
			glScalef(0.06, 0.06, 0.03);
			drawCube();
		glPopMatrix();

		//Across cube of t -  
		glPushMatrix();
			glColor3f(1.0, 0.5, 0);  //Orange
			glTranslatef(-0.3, 0.05, -0.15);
			glRotatef(45, 0, 0, 1);
			glRotatef(92, 0, 1, 0);
			glScalef(0.15, 0.025, 0.1);
			drawCube();
		glPopMatrix();

		//Bottom curve of t 
		glPushMatrix();
			glColor3f(1.0, 0.5, 0);  //Orange
			glTranslatef(-0.27, 0.09, -0.15);
			glRotatef(-135.0, 0, 0, 1);
			glScalef(0.1, 0.15, 0.05);
			drawCurve(0.0, 180.0);
		glPopMatrix();
	glPopMatrix();
}

//======================================================
// DRAW LEG
//======================================================
void drawLeg(float legAngle, float footAngle)
{
		glPushMatrix();			
			glPushMatrix();
					glColor3f(1.0, 0.5, 0);  //Orange
					glTranslatef(-0.15, -0.45, -0.1);
					glScalef(0.07, 0.25, -0.05);
					drawCube();
			glPopMatrix();

			//Using a double translation to move the foot to the origin, performing the rotation and placing it back in its' original position
			glPushMatrix();	
				glTranslatef(-0.2, -0.45, -0.1);	
				glRotatef(footAngle, 0.0, 0.0, 2.5);
				glTranslatef(0.275, 0.9, 0.001);
				drawFoot(footAngle);
			glPopMatrix();
		glPopMatrix();
}

//======================================================
// DRAW FOOT
//======================================================
void drawFoot(float footAngle)
{
	//Right cube of foot L
	glPushMatrix();
		glColor3f(1.0, 0.5, 0);  //Orange
		glTranslatef(-0.1, -1.0, -0.01);	
		glScalef(0.3, 0.05, -0.05);
		drawCube();
	glPopMatrix();
}

//======================================================
// KEYBOARD FUNCTION 
//======================================================

void myKeyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	//Rotate around the X axis
	case 'x': thetaX++; break;
	case 'X': thetaX--; break;

	//Rotate around the Y axis
	case 'y': thetaY++; break;
	case 'Y': thetaY--; break;

	//Rotate around the Z axis
	case 'z': thetaZ++; break;
	case 'Z': thetaZ--; break;

	//Floor selection
	case 'f': case 'F':
		dont_display_floor++;
		if (dont_display_floor > 1) dont_display_floor = 0;
	break;

	//Model selection
	case 'm': case 'M':
		current_model++;
		if (current_model > NUMBER_OF_MODELS) current_model = 1;
	break;

	//Wing angle
	case 'w': wingAngle++; break;
	case 'W': wingAngle--; break;

	//Wing angle2
	case 'q': wingAngle2++; break;
	case 'Q': wingAngle2--; break;

	//Leg angle
	case 'l': legAngle++; break;
	case 'L': legAngle--; break;

	case 'h': footAngle++; break;
	case 'H': footAngle--; break;

	//ZOOM
	case 'd': G_zoom/=1.5; break;
	case 'D': G_zoom*=1.5; 	break;

	//GRIDS
	case 'a': x_y_display++; if(x_y_display>1) x_y_display=0; break;
	case 'b': y_z_display++; if(y_z_display>1) y_z_display=0; break;
	case 'A': x_z_display++; if(x_z_display>1) x_z_display=0; break;

	default: printf("Unknown input");
	}

	glutPostRedisplay();
}

//======================================================
// DISPLAY RESHAPE CALLBACK ROUTINE 
//======================================================
void reshapeCallBack(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0 * (float)h / (float)w,
		2.0 * (float)h / (float)w, -10.0, 10.0);
	else
		glOrtho(-2.0 * (float)w / (float)h,
		2.0 * (float)w / (float)h, -2.0, 2.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
}


//======================================================
// DISPLAY CALLBACK ROUTINE 
//======================================================
void displayCallBack(void)
{
	// display callback,
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//viewControl();
	drawScene(wingAngle, footAngle, legAngle, wingAngle2);

	glFlush();
	glutSwapBuffers();
}

//======================================================
// VIEW CONTROL ROUTINE 
//======================================================
void viewControl()
{
	// Reset matrix
	glLoadIdentity();

	//Rotate everything 
	glRotatef(G_theta[0], 1.0, 0.0, 0.0);
	glRotatef(G_theta[1], 0.0, 1.0, 0.0);
	glRotatef(G_theta[2], 0.0, 0.0, 1.0);

	//zoom (NB glOrtho projection)
	glScalef(G_zoom, G_zoom, G_zoom);
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glRotatef(thetaX, 1, 0, 0);
	glRotatef(thetaY, 0, 1, 0);
	glRotatef(thetaZ, 0, 0, 1);

	drawAxes();

	drawScene(wingAngle, footAngle, legAngle, wingAngle2);

	glFlush();
	glutSwapBuffers();
}

void idle()
{
	thetaY--;
	thetaX++;
	thetaZ++;
	glutPostRedisplay();
}

void myTimer(int duration)
{
	thetaZ++;
	glutTimerFunc(duration, myTimer, duration);
	glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y)
{
	//\n to go to the next line 
	printf("button = %d, state = %d, x = %d, y = %d\n", button, state, x, y);
}

//======================================================
// MAIN PROGRAM 
//======================================================
int main(int argc, char** argv)
{
	//Add display and callbacks
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Chick");
	glEnable(GL_DEPTH_TEST);
	glutReshapeFunc(reshapeCallBack);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	
	glClearColor(0.8, 0.8, 1.0, 1.0); //light blue 
 
	glutMainLoop();
	return 0;
}
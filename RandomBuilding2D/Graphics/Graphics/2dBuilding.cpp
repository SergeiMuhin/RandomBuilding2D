#define _USE_MATH_DEFINES
#include "GLUT.H"
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

/// NOTICE THIS PROGRAM WAS BASED ON TEACHER EXAMPLE SOME FUNCTION CONTAIN (BIG LETTERS!)

const int NUM_STARS = 1000;
const int WIN_H = 100;
const int WIN_W = 100;
const int MAX_B = 50;
const int MIN_B = 30;
const int MAX_REFLECTION_RES = 50;
const int MAX_MOON_RES = 5;
const int WIN_SIZE_H = 800;
const int WIN_SIZE_W = 800;
double stars[3][NUM_STARS]; // x,y,size
double winlight[WIN_H][WIN_W];
double offset = 0;
int bRoll = 0;

struct COLOR
{
	double red, green, blue;
};
struct MY_BUILDING
{
	double height;  // Building Hight
	double width;   // Building Width
	int window_type;  // Window type in the building
	COLOR type;        // Type building (Color)
	double location ;   // Where it begins.
	int building_type;  // 0 (round) 1 (square) 3 (Null)

};

// Must to after Making Struct !~!
MY_BUILDING buildingList[MAX_B];    // Make Random Building with Random Types

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	int triangleAmount = 50; //# of triangles used to draw circle

							 //GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * M_PI;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for (i = 0; i <= triangleAmount; i++) {
		glVertex2f(
			x + (radius * cos(i *  twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))
		);
	}
	glEnd();
}
void drawSquare(double x1, double y1, double sidelength)
{
	double halfside = sidelength / 2;

	glBegin(GL_POLYGON);

	glVertex2d(x1 + halfside, y1 + halfside);
	glVertex2d(x1 + halfside, y1 - halfside);
	glVertex2d(x1 - halfside, y1 - halfside);
	glVertex2d(x1 - halfside, y1 + halfside);

	glEnd();
}
void init()
{
	int i=0,j;
	double x, y;
	srand(time(NULL)); // Real Random !
	// Create's Start on the Field (SKY)
	for (i = 0; i < NUM_STARS; i++)
	{
		// random value in [-1,1]
		stars[0][i] =-1+ 2*(rand() % 1000)/1000.0;
		// random value in [0,1]
		stars[1][i] = -0.3 +2*(rand() % 1000) / 1000.0; // -0.3 for lower
		// random size
		stars[2][i] = (rand() % 3) ;
	}
	// set windows lights
	for (i = 0; i < WIN_H; i++)
		for (j = 0; j < WIN_W; j++)
			winlight[i][j] = (rand() % 100) / 100.0; //  random value from 0 to 1


	//Random Roll and than Loop for random House's XD

	for (x = -1; x <= 1; x += 0.12) // 0.12 is to have a small space between the buildings
	{
		y = 0.7*fabs(sin(3 * x));
		// Random Height 0 - 0.2
		buildingList[bRoll].height = (rand() % 200) / 1000.0;
		// Random Location on X
		buildingList[bRoll].location = x;
		// add to sinus the Hight.
		buildingList[bRoll].height = buildingList[bRoll].height + y;
		// Random Width.. 90-110 0.09 - 0.11
		buildingList[bRoll].width = (rand() % 30 + 90) / 1000.0;
		// Take so 0.1 distance between Building Kept
		x = x - (0.12 - buildingList[bRoll].width-0.01);
		// Random Building Color
		buildingList[bRoll].type.red = (rand() % 1000) / 1000.0;
		buildingList[bRoll].type.blue = (rand() % 1000) / 1000.0;
		buildingList[bRoll].type.green = (rand() % 1000) / 1000.0;
		// Random Window Type
		buildingList[bRoll].window_type = (rand() % 3);
		buildingList[bRoll].building_type = (rand() % 3);
		bRoll++;
	}


	glClearColor(0.0,0.0,0,0); // sets baclground color 
	glOrtho(-1, 1, -1, 1, 1, -1);
}
void drawTree(double locationX,double locationY)
{
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2d(locationX,locationY);
	glVertex2d(locationX, locationY + 0.1);
	glEnd();
}
void DrawSky()
{
	int i,j;
	double color = 0.95,radius = 0.2;
	// draw sky
	glBegin(GL_POLYGON);
	//All
		glColor3d(0, 0, 0);    // Black
		glVertex2d(-1, 1);     // x(left)  y (top)
		glVertex2d(1, 1);      // x(right) y (top)

		glColor3d(0, 0.1, 0.4);  // Blueish
		glVertex2d(1, -0.3);     //  x(left)  y (30%)
		glVertex2d(-1, -0.3);    // x(right) y (30%)

	glEnd();
	// add stars
	glColor3d(1, 1, 1);//White
	i = rand() % NUM_STARS;
	stars[2][i] = rand() % 3;

	for (j = 0; j < NUM_STARS; j++)
	{

		glPointSize(stars[2][j]);
		glBegin(GL_POINTS);
		glVertex2d(stars[0][j], stars[1][j]);
		glEnd();
	}
	// Drow Moon
	for (i = 0; i <= MAX_MOON_RES; i++)
	{
		glColor3d(color, color, color);
		drawFilledCircle(0.75, 0.75, radius);
		radius = radius - (0.2 / MAX_MOON_RES);
		color = color + (0.05 / MAX_MOON_RES);
	}
}

void DrawBuilding(double x, double height, double width,boolean reflection,COLOR type,int winType,int bType)
{
	int i, j;
	double xx, yy ,radius,winHight,locationX,locationY;

	// Add upper 
	glPointSize(4);
	if (reflection == 0)
	{
		//if scutterd in middles ^^
		// adding the roof (Type)
		if (bType == 1) // square
		{
			glColor3d(1, 1, 1); //White
			drawSquare(x + (width / 2), height - 0.3, (width / 2) + 0.005);
			glColor3d(type.red, type.green, type.blue); //Color of Building
			drawSquare(x + (width / 2), height - 0.3, (width / 2));
			glColor3d(0, 0, 0);//Black
			drawSquare(x + (width / 2), height - 0.3, (width / 4));
		}

		// add White . RAM
		glBegin(GL_POLYGON);
		glColor3d(1, 1, 1); // bottom
		glVertex2d(x - 0.003, -0.3);
		glColor3d(1, 1, 1); // top
		glVertex2d(x - 0.003, height - 0.3 + 0.004);
		glVertex2d(x + 0.003 + width, height - 0.3 + 0.004);
		glColor3d(1, 1, 1); // bottom
		glVertex2d(x + 0.003 + width, -0.3);
		glEnd();

		if (bType == 0) // round
		{
			glColor3d(1, 1, 1); //White
			drawFilledCircle(x + (width / 2), height - 0.3, (width / 2) + 0.003);
			glColor3d(type.red, type.green, type.blue); //Color of Building
			drawFilledCircle(x + (width / 2), height - 0.3, (width / 2));
			glColor3d(0.9, 0.9, 0);
			drawFilledCircle(x + (width / 2), height - 0.3, (width / 4));

		}
		// add Building
		glBegin(GL_POLYGON);
		glColor3d(type.red, type.green, type.blue); // bottom
		glVertex2d(x, -0.3);
		glColor3d(type.red, type.green, type.blue); // top
		glVertex2d(x, height - 0.3);
		glVertex2d(x + width, height - 0.3);
		glColor3d(type.red, type.green, type.blue); // bottom
		glVertex2d(x + width, -0.3);
		glEnd();




		// Window with Type !
		if (winType == 0)
		{
			// add windows
			radius = (width - 0.03) / 4;
			for (xx = x + 0.01, j = 0; xx < x + width - 0.01; xx += 0.03 + radius, j++)
				for (yy = -0.25, i = 0; yy < height - 0.3 - 0.04; yy += 0.03 + radius, i++)
				{
					glColor3d(winlight[i][j], winlight[i][j], 0);
					drawFilledCircle(xx + radius, yy, radius);
				}

		}
		else if(winType == 1)
		{
			winHight = 0.05;
			for (xx = x + 0.01, j = 0; xx < x + width - 0.01; xx = x + width, j++)
				for (yy = -0.25, i = 0; yy < height - 0.355; yy += 0.055, i++)
				{
					glColor3d(winlight[i][j], winlight[i][j], 0);
					glBegin(GL_POLYGON);

					glVertex2d(xx, yy + 0.05 );  // 0 left    top 
					glVertex2d(xx + width - 0.02 , yy + 0.05); // 1 right   top
					glVertex2d(xx + width - 0.02 , yy ); // 2 right   bottom
					glVertex2d(xx , yy ); // 3 left    bottom

					glEnd();
				}

		}
	else if (winType == 2)
	{
		radius = (width - 0.03) / 4;
		for (xx = x + 0.01, j = 0; xx < x + width - 0.01; xx += 0.03 + radius, j++)
			for (yy = -0.25, i = 0; yy < height - 0.3 - 0.04; yy += 0.03 + radius, i++)
			{
				glColor3d(winlight[i][j], winlight[i][j], 0);
				drawSquare(xx + radius, yy, radius*2);
			}

	}


	}

	// ! -------> Reflection <------- ! 

	// Add  (Reflection!)
	else
	{
		// if scutterd for Type
		// adding the roof (Type) reflection
		if (bType == 1) // square
		{
			glColor3d(1, 1, 1); //White
			drawSquare(x + (width / 2), height - 0.3, (width / 2) + 0.005);
			glColor3d(type.red, type.green, type.blue); //Color of Building
			drawSquare(x + (width / 2), height - 0.3, (width / 2));
			glColor3d(0, 0, 0);//Black
			drawSquare(x + (width / 2), height - 0.3, (width / 4));
		}

		//Building Reflection WHITE
		glBegin(GL_POLYGON);
		glColor3d(1, 1, 1); // bottom
		glVertex2d(x - 0.003, -0.3);
		glColor3d(1, 1, 1); // top
		glVertex2d(x - 0.003, height - 0.3 - 0.004);
		glVertex2d(x + 0.003 + width, height - 0.3 - 0.004);
		glColor3d(1, 1, 1); // bottom
		glVertex2d(x + 0.003 + width, -0.3);
		glEnd();

		if (bType == 0) // round
		{
			glColor3d(1, 1, 1); //White
			drawFilledCircle(x + (width / 2), height - 0.3, (width / 2) + 0.003);
			glColor3d(type.red, type.green, type.blue); //Color of Building
			drawFilledCircle(x + (width / 2), height - 0.3, (width / 2));
			glColor3d(0.9, 0.9, 0);
			drawFilledCircle(x + (width / 2), height - 0.3, (width / 4));
		}
		//Building Reflection
		glBegin(GL_POLYGON);
		glColor3d(type.red, type.green, type.blue); // bottom
		glVertex2d(x, -0.3);
		glColor3d(type.red, type.green, type.blue); // top
		glVertex2d(x, height - 0.3);
		glVertex2d(x + width, height - 0.3);
		glColor3d(type.red, type.green, type.blue); // bottom
		glVertex2d(x + width, -0.3);
		glEnd();

		if (winType == 0)
		{
			radius = (width - 0.03) / 4;
			for (xx = x + 0.01, j = 0; xx < x + width - 0.01; xx += 0.03 + radius, j++)
				for (yy = -0.35, i = 0; yy > height + 0.04 - 0.3; yy -= 0.03 + radius, i++)
				{
					glColor3d(winlight[i][j], winlight[i][j], 0);
					drawFilledCircle(xx + radius, yy, radius);
				}

		}
		else if(winType == 1)
		{
			winHight = 0.05;
			for (xx = x + 0.01, j = 0; xx < x + width - 0.01; xx = x + width, j++)
				for (yy = -0.35, i = 0; yy > height - 0.255; yy -= 0.055, i++)
				{
					glColor3d(winlight[i][j], winlight[i][j], 0);
					glBegin(GL_POLYGON);

					glVertex2d(xx, yy - 0.05);  // 0 left    top 
					glVertex2d(xx + width - 0.02, yy - 0.05); // 1 right   top
					glVertex2d(xx + width - 0.02, yy); // 2 right   bottom
					glVertex2d(xx, yy); // 3 left    bottom

					glEnd();
				}

		}
		else if (winType == 2)
		{
			radius = (width - 0.03) / 4;
			for (xx = x + 0.01, j = 0; xx < x + width - 0.01; xx += 0.03 + radius, j++)
				for (yy = -0.35, i = 0; yy > height + 0.04 - 0.3; yy -= 0.03 + radius, i++)
				{
					glColor3d(winlight[i][j], winlight[i][j], 0);
					drawSquare(xx + radius, yy, radius*2);
				}

		}
	}
}

void reflectionLines()
{
	int i;
	for (i = 0; i <= MAX_REFLECTION_RES; i++)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Draw a Red 1x1 Square centered at origin
		glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
		glColor4d(0.1, 0.1, 0.65, 0.15);
		glVertex2d(1, -1);
		glVertex2d(-1, -1);
		glVertex2d(-1, -0.3 - (i*0.7 / MAX_REFLECTION_RES));    // x, y
		glVertex2d(1, -0.3 - (i*0.7 / MAX_REFLECTION_RES));
		glEnd();
		glDisable(GL_BLEND);
	}
}
void display()
{
	int i;
	double x, y,color = 0.85,radius = 0.2;
	glClear(GL_COLOR_BUFFER_BIT); // loads background color

	DrawSky();

	// buildings
	for (i = 0; i < bRoll;i++) // 0.12 is to have a small space between the buildings
	{
		DrawBuilding(buildingList[i].location, buildingList[i].height, buildingList[i].width,0,buildingList[i].type,buildingList[i].window_type,buildingList[i].building_type);
		//reflection of Building
		DrawBuilding(buildingList[i].location, (-1)*buildingList[i].height, buildingList[i].width, 1, buildingList[i].type, buildingList[i].window_type,buildingList[i].building_type);
	}


	//draws Moon For Reflection
	for (i = 0; i <= MAX_MOON_RES; i++)
	{
		glColor3d(color, color, color);
		drawFilledCircle(0.75, -0.75, radius);
		radius = radius - (0.2 / MAX_MOON_RES);
		color = color + (0.15 / MAX_MOON_RES);
	}
	// the middle .
	glColor3d(0.8, 0.8, 0.8);
	glBegin(GL_POLYGON);
	glVertex2d(1, -0.28);                // 0 left    top 
	glVertex2d(-1 , -0.28 );             // 1 right   top
	glVertex2d(-1, -0.32);              // 2 right   bottom
	glVertex2d(1, -0.32);               // 3 left    bottom
	glEnd();

	//draw Lamps
	for (x=-1; x<=1; x +=0.25)
	{
		glColor3d(0, 0, 0); // Brown
		drawTree(x ,-0.28);
		glColor3d(1, 1, 1);    // White
		drawFilledCircle(x, (-0.28 + 0.1), 0.02);
	}
	//draw lamps reflection
	for (x = -1; x <= 1; x += 0.25)
	{
		glColor3d(0, 0, 0); // Brown
		drawTree(x, -0.42);
		glColor3d(1, 1, 1);    // White
		drawFilledCircle(x,-0.42, 0.02);
	}


	//Reflection
	reflectionLines();


	glutSwapBuffers();
}

void idle()
{
	offset += 0.01;
	glutPostRedisplay(); // indirect call to display
}


// always the same Main
void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(WIN_SIZE_W, WIN_SIZE_H);

	glutCreateWindow("Graphics example");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	init();
	glutMainLoop();
}
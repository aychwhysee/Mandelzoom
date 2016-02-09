/*--------------------------------------------------------*/
/*  CS-378           Computer Graphics         Harry Cho  */
/*--------------------------------------------------------*/
/* Assignment 1: Mandelzoom                               */
/*--------------------------------------------------------*/

//Current issues: 
// ** After user chooses to Pop, it pops but automatically Pushes right after.
//    Not sure why.
// ** Implemented 2D array so we can store the value of how many times Z passed
//    Not sure how or why it would be more efficient? Wouldn't we have to do the
//    math again anyway when zooming in and out? Would it work in push/pop better?
//    Anyway, the array is there and is storing the values, but I am not using them anywhere.
//    I wrote out a drawPicture2() function to see how I could use it, but I am out of time now.


#include <cstdlib>
#include <GL/glut.h>
#include <cmath>
#include <cfloat>
#include <iostream>
#include <complex>
#include <vector>
#include <array>

using namespace std;

// Defining default values for window size, shape and location.
#define INITIAL_WIN_W 800
#define INITIAL_WIN_H 800
#define INITIAL_WIN_X 150
#define INITIAL_WIN_Y 50

// Variable for use in rubberbanding.
int xAnchor, yAnchor, xStretch, yStretch;
bool rubberBanding = false;

// Variables for keeping track of the screen window dimensions.
int windowHeight, windowWidth;

// Variables for keeping track of complex plane dimensions...?
double x1, x2, y3, y2;

int MaxIterations = 1000;

//lets make arrays to store values for determining colors
int plop[INITIAL_WIN_W][INITIAL_WIN_H];

//lets make vectors for pushpops
vector<double> leftV;
vector<double> rightV;

void drawPicture()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);

	double realPart = (x2 - x1) / (windowWidth - 1);
	double imaginaryPart = (y2 - y3) / (windowHeight - 1);

	for (int u = 0; u < windowWidth; ++u)
	{
		double complexReal = x1 + u*realPart;
		for (int v = 0; v < windowHeight; ++v)
		{
			double complexImaginary = y3 + v*imaginaryPart;

			double zReal = complexReal, zImaginary = complexImaginary;
			bool isMandelbrot = true;
			int count = 0; //So we can keep track of how many times a Z succeeded
			for (int n = 0; n <= MaxIterations; ++n)
			{
				double zReal2 = zReal*zReal, zImaginary2 = zImaginary*zImaginary;
				if (zReal2 + zImaginary2 > 4)
				{
					isMandelbrot = false;
					break;
				}
				zImaginary = 2 * zReal*zImaginary + complexImaginary;
				zReal = zReal2 - zImaginary2 + complexReal;
				count = n; //How many times succeeded to be in mandelbrot set
				plop[u][v] = count;
			}
			if (count == MaxIterations)
			{
				glColor3f(0.0f, 0.0f, 0.0f);
				glVertex2i(u, v);
			}
			// If more than 75% of iterations pass but not fully 100% blue
			else if (count >= (MaxIterations * 3 / 4) && count <= MaxIterations - 1)
			{
				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex2i(u, v);
			}
			// If more than 50% of iterations pass but less than 75%
			else if (count >= MaxIterations / 2 && count <= (MaxIterations * 3 / 4 - 1))
			{
				glColor3f(0.0f, 0.0f, 0.8f);
				glVertex2i(u, v);
			}
			// If more than 25% of iterations pass but less than 50%
			else if (count >= (MaxIterations * 1 / 4) && count <= MaxIterations / 2 - 1)
			{
				glColor3f(0.2f, 0.5f, 0.7f);
				glVertex2i(u, v);
			}
			// If less than 25%, but at least 1 passes
			else if (count > 0 && count <= (MaxIterations * 1 / 4 - 1))
			{
				glColor3f(0.1f, 0.1f, 0.3f);
				glVertex2i(u, v);
			}
			else
			{
				glColor3f(0.5f, 0.0f, 0.5f);
				glVertex2i(u, v);
			}
		}
	}
	glEnd();
	glFlush();
}

//void drawPicture2()
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	glBegin(GL_POINTS);
//
//	for (int u = 0; u < windowWidth; ++u)
//	{
//		for (int v = 0; v < windowHeight; ++v)
//		{
//			int count = plop[u][v];
//			if (count == MaxIterations)
//			{
//				glColor3f(0.0f, 0.0f, 0.0f);
//				glVertex2i(u, v);
//			}
//			 If more than 75% of iterations pass but not fully 100% blue
//			else if (count >= (MaxIterations * 3 / 4) && count <= MaxIterations - 1)
//			{
//				glColor3f(0.0f, 0.0f, 1.0f);
//				glVertex2i(u, v);
//			}
//			 If more than 50% of iterations pass but less than 75%
//			else if (count >= MaxIterations / 2 && count <= (MaxIterations * 3 / 4 - 1))
//			{
//				glColor3f(0.0f, 0.0f, 0.8f);
//				glVertex2i(u, v);
//			}
//			 If more than 25% of iterations pass but less than 50%
//			else if (count >= (MaxIterations * 1 / 4) && count <= MaxIterations / 2 - 1)
//			{
//				glColor3f(0.2f, 0.5f, 0.7f);
//				glVertex2i(u, v);
//			}
//			 If less than 25%, but at least 1 passes
//			else if (count > 0 && count <= (MaxIterations * 1 / 4 - 1))
//			{
//				glColor3f(0.1f, 0.1f, 0.3f);
//				glVertex2i(u, v);
//			}
//			else
//			{
//				glColor3f(0.5f, 0.0f, 0.5f);
//				glVertex2i(u, v);
//			}
//		}
//	}
//
//	glEnd();
//	glFlush();
//}

void zoomIn(int xOld, int yOld, int xNew, int yNew)
{
	double realPart = (x2 - x1) / (windowWidth - 1);
	double imaginaryPart = (y2 - y3) / (windowHeight - 1);

	double complexReal_xO = x1 + xOld*realPart;
	double complexImaginary_yO = y3 + yOld*imaginaryPart;
	double complexReal_xN = x1 + xNew*realPart;
	double complexImaginary_yN = y3 + yNew*imaginaryPart;

	x1 = complexReal_xO;
	x2 = complexReal_xN;
	y3 = complexImaginary_yO;
	y2 = complexImaginary_yN;

	//Not sure if the below is necessary here
	//rightV.push_back(x1);
	//rightV.push_back(x2);
	//rightV.push_back(y3);
	//rightV.push_back(y2);

	drawPicture();
}

void drawRubberBand(int xA, int yA, int xS, int yS)
{
	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_XOR);
	glBegin(GL_LINE_LOOP);
	glVertex2i(xA, yA);
	glVertex2i(xS, yA);
	glVertex2i(xS, yS);
	glVertex2i(xA, yS);
	glEnd();
	glDisable(GL_COLOR_LOGIC_OP);
	glFlush();
}

void rubberBand(int x, int y)
// Callback for processing mouse motion.
{
	if (rubberBanding)
	{
		drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
		y = windowHeight - y;
		xStretch = x;
		yStretch = y;
		drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
		glFlush();
	}
}

void reshape(int w, int h)
// Callback for processing reshape events.
{
	windowWidth = w;
	windowHeight = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

void escExit(GLubyte key, int, int)
// Callback for processing keyboard events.
{
	if (key == 27 /* ESC */) std::exit(0);
}

void push() //zoom back in one step (go back forwards)
{
	leftV.push_back(x1);
	leftV.push_back(x2);
	leftV.push_back(y3);
	leftV.push_back(y2);

	y2 = rightV.back();
	rightV.pop_back();
	y3 = rightV.back();
	rightV.pop_back();
	x2 = rightV.back();
	rightV.pop_back();
	x1 = rightV.back();
	rightV.pop_back();
	drawPicture();
}

void pop() //zoom back out one step
{
	rightV.push_back(x1);
	rightV.push_back(x2);
	rightV.push_back(y3);
	rightV.push_back(y2);

	y2 = leftV.back();
	leftV.pop_back();
	y3 = leftV.back();
	leftV.pop_back();
	x2 = leftV.back();
	leftV.pop_back();
	x1 = leftV.back();
	leftV.pop_back();
	drawPicture();

	//automatically pushing back to zoom in after popping?
}

void mainMenu(int item)
// Callback for processing main menu.
{
	switch (item)
	{
	case 1: std::exit(0);
	case 2: pop();
	case 3: push();
	}
}

void setMenus()
// Function for creating menus.
{
	glutCreateMenu(mainMenu);
	glutAddMenuEntry("Exit", 1);
	glutAddMenuEntry("Pop", 2); //Zoom out
	glutAddMenuEntry("Push", 3); //Zoom back in
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

void processLeftDown(int x, int y)
// Function for processing mouse left botton down events.
{
	if (!rubberBanding)
	{
		int xNew = x;
		int yNew = windowHeight - y;
		xAnchor = xNew;
		yAnchor = yNew;
		xStretch = xNew;
		yStretch = yNew;
		drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
		rubberBanding = true;
	}
}

void processLeftUp(int x, int y)
// Function for processing mouse left botton up events.
{
	if (rubberBanding)
	{
		//to store current boundaries so we can come get them later
		leftV.push_back(x1);
		leftV.push_back(x2);
		leftV.push_back(y3);
		leftV.push_back(y2);

		//y = windowHeight - y; <--- not needed
		int xNew, yNew;
		drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
		rubberBanding = false;

		int xAnchorT = xAnchor <= xStretch ? xAnchor : xStretch;
		int xStretchT = xAnchor <= xStretch ? xStretch : xAnchor;
		int yAnchorT = yAnchor <= yStretch ? yAnchor : yStretch;
		int yStretchT = yAnchor <= yStretch ? yStretch : yAnchor;

		xAnchor = xAnchorT;
		xStretch = xStretchT;
		yAnchor = yAnchorT;
		yStretch = yStretchT;

		if (abs(xStretch - xAnchor) > abs(yStretch - yAnchor)) //If wider than long
		{
			xNew = xAnchor + abs(yStretch - yAnchor);
			yNew = yStretch;
		}
		else if (abs(yStretch - yAnchor) > abs(xStretch - xAnchor)) //If longer than wide
		{
			xNew = x;
			yNew = yStretch - abs(xStretch - xAnchor);
		}
		else //If perfect square
		{
			xNew = x;
			yNew = yStretch;
		}

		zoomIn(xAnchor, yAnchor, xNew, yNew);
		glFlush();
	}
}

void mouse(int button, int state, int x, int y)
// Function for processing mouse events.
{
	if (button == GLUT_LEFT_BUTTON)
		switch (state)
		{
		case GLUT_DOWN: processLeftDown(x, y); break;
		case GLUT_UP: processLeftUp(x, y); break;
		}
}

int main(int argc, char * argv[])
{
	// Mask floating point exceptions.
	_control87(MCW_EM, MCW_EM);

	// Initialize glut with command line parameters.
	glutInit(&argc, argv);

	// Choose RGB display mode for normal screen window.
	glutInitDisplayMode(GLUT_RGB);

	// Set initial window size, position, and title.
	glutInitWindowSize(INITIAL_WIN_W, INITIAL_WIN_H);
	glutInitWindowPosition(INITIAL_WIN_X, INITIAL_WIN_Y);
	windowWidth = INITIAL_WIN_W;
	windowHeight = INITIAL_WIN_H;
	x1 = atof(argv[1]);
	x2 = atof(argv[2]);
	y3 = atof(argv[3]);
	y2 = atof(argv[4]);
	//x1 = -2.0;
	//x2 = 0.5;
	//y3 = -1.25;
	//y2 = 1.25;
	glutCreateWindow("Mandelzoom!");

	// You don't (yet) want to know what this does.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double)INITIAL_WIN_W, 0.0, (double)INITIAL_WIN_H), glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.375, 0.375, 0.0);

	// Set the callbacks for the normal screen window.
	glutDisplayFunc(drawPicture);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(escExit);
	glutMotionFunc(rubberBand);
	glutPassiveMotionFunc(rubberBand);

	// Set the drawing color.
	glColor3f(1.0, 1.0, 1.0);

	// Set the color for clearing the window.
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Set up the menus.
	setMenus();

	glutMainLoop();
	return 0;
}

// With conceptual help from http://warp.povusers.org/Mandelbrot/

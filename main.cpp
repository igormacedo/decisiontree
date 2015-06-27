/************************************************
* Zeus CMD - OpenGL Tutorial 03 : OpenGL Window *
* By Grant James (ZEUS)                         *
* http://www.zeuscmd.com        				*
* Modified by Igor Macedo		                *
************************************************/
#include <stdlib.h>
#include <GL/glut.h>
#include "DecisionTree.h"
#include "RandomPoint.h"
#include <unistd.h>
#include <chrono>

using namespace std;

bool fullscreen = false;
list<ColorPoint> generatedPoints;
DecisionTree myTree;
double winx = 500.0, winy = 500.0;

//methods
bool init();
void display();
void createPointCloud();
void keyboard(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void createDecisionTree();
void mouseClick(int button, int state, int x, int y);
long map(long x, long in_min, long in_max, long out_min, long out_max);

bool init()
{
	glClearColor(0.93f,0.93f,0.93f,0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);

	glPointSize((float)(15));
	glLineWidth(float(3));
	//generatedPoints = new list<ColorPoint>;

	for(int i = 0; i < 50; i++){
		ColorPoint p = RandomPoint::getRandomPoint(1.0,0.0,1.0,-1.0,RED);
		generatedPoints.push_back(p);
	}

	for(int i = 0; i < 30; i++){
		ColorPoint p = RandomPoint::getRandomPoint(1.0,-1.0,0.0,-1.0,BLUE);
		generatedPoints.push_back(p);
	}

	for(int i = 0; i < 30; i++){
		ColorPoint p = RandomPoint::getRandomPoint(1.0,-1.0,1.0,0.0,GREEN);
		generatedPoints.push_back(p);
	}
	return true;
}

void display()
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//float pointSize = 300;
	//glGetFloatv(GL_POINT_SIZE, &pointSize);


	for(list<ColorPoint>::iterator it = generatedPoints.begin(); it != generatedPoints.end(); it++)
	{
		glBegin(GL_POINTS);

		switch(it->color)
		{
			case RED:
				glColor3f(1.0f,0.0f,0.0f); break;
			case BLUE:
				glColor3f(0.0f,0.0f,1.0f); break;
			case GREEN:
				glColor3f(0.0f,1.0f,0.0f); break;
			default:
				glColor3f(0.0f,0.0f,0.0f); break;
		}

		glVertex3f(it->x,it->y,0.0f);

		glEnd();
	}

	/*
	glBegin(GL_POINTS);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f( 0.7f, -0.5f, 0.0f);
		glVertex3f( 0.6f, -0.7f, 0.0f);
		glVertex3f( 0.0f, -0.0f, 0.0f);
	glEnd();
	*/
	glFlush();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(1);
}

void specialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_F1)
	{
		fullscreen = !fullscreen;

		if (fullscreen)
			glutFullScreen();
		else
		{
			glutReshapeWindow(500, 500);
			glutPositionWindow(50, 50);
		}
	}
	if (key == GLUT_KEY_F2)
	{
		createDecisionTree();
	}
	if (key == GLUT_KEY_F3)
	{
		createPointCloud();
	}
}

void createDecisionTree()
{
	myTree.CreateTree(generatedPoints);
}

void mouseClick(int button, int state, int x, int y)
{
	if(state == GLUT_UP)
	{
		ColorPoint point;

		point.x = map(x,0,winx,-1000,1000)/1000.0;
		point.y = map(y,0,winy,1000,-1000)/1000.0;

		cout << "x = " << point.x << endl;
		cout << "y = " << point.y << endl;


		glBegin(GL_POINTS);
			glColor3f(0.0f,0.0f,0.0f);
			glVertex3f(point.x,point.y,0.0f);
		glEnd();

		glFlush();
		glutSwapBuffers();

		//usleep(500000);
		//cout << "Aqui 1" << endl;

		Color color = myTree.evaluatePoint(point);

		//cout << "Aqui SAIDA" << endl << endl;

		glBegin(GL_POINTS);
			switch(color)
			{
				case RED:
					glColor3f(1.0f,0.0f,0.0f); break;
				case BLUE:
					glColor3f(0.0f,0.0f,1.0f); break;
				case GREEN:
					glColor3f(0.0f,1.0f,0.0f); break;
				default:
					glColor3f(0.0f,0.0f,0.0f); break;
			}
			glVertex3f(point.x,point.y,0.0f);

		glEnd();
		glFlush();
		glutSwapBuffers();

	}
	//display();
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	//map function from arduino library
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void createPointCloud()
{
	for(int i = -25; i <= 25; i++)
	{
		for(int j = -25; j <= 25; j++)
		{
			ColorPoint p(i/25.0,j/25.0);
			p.color = myTree.evaluatePoint(p);

			glBegin(GL_POINTS);
				switch(p.color)
				{
					case RED:
						glColor4f(1.0f,0.0f,0.0f,0.2f); break;
					case BLUE:
						glColor4f(0.0f,0.0f,1.0f,0.2f); break;
					case GREEN:
						glColor4f(0.0f,1.0f,0.0f,0.2f); break;
					default:
						glColor4f(0.0f,0.0f,0.0f,0.2f); break;
				}
				glVertex3f(p.x,p.y,0.0f);

			glEnd();
			glFlush();
			glutSwapBuffers();
		}
	}

}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winx, winy);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutCreateWindow("03 - OpenGL Window");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMouseFunc(mouseClick);

	if (!init())
		return 1;

	display();
	glutMainLoop();

	return 0;
}

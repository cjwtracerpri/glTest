#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

static void init()
{
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

static void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-10, 10, -10 * h / (float)w, 10 * h / (float)w);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex2d(0, 0);
	glEdgeFlag(GL_FALSE);
	glColor3f(0, 0, 1);
	glVertex2d(0, 3);
	glColor3f(1, 0, 0);
	glEdgeFlag(GL_TRUE);
	glVertex2d(4, 3);
	glColor3f(1, 1, 0);
	glVertex2d(6, 1.5);
	glColor3f(0, 1, 0);
	glVertex2d(4, 0);
	glEnd();

	glFlush();
}

static void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	  case 27:
		  exit(0);
		  break;
	}
}

int mainPrimitive(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
#include "gl/glew.h"
#include "GL/glut.h"
#include <cstdlib>
#include "stdio.h"

static
void display()
{
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);


	glBegin(GL_QUADS);
	glVertex2f (0, 100.0);
	glVertex2f (100.0, 00.0);
	glVertex2f (200.0, 100.0);
	glVertex2f (100.0, 200.0);
	glEnd();
	glFlush();
}

static
void init (void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);    
}

static
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	  case 27:
		  exit(0);
		  break;
	}
}

int mainTest(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (350, 150);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc (keyboard);
	const GLubyte* s = glGetString(GL_EXTENSIONS);
	printf("%d", s);

	glutMainLoop();
	return 0;  
}
#include "gl/glew.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

//#define TEX_GEN_SPHERE

#define	stripeImageWidth 32
static
GLubyte stripeImage[stripeImageWidth][4];

#ifdef GL_VERSION_1_1
static GLuint texName;
#endif

static
void makeStripeImage(void)
{
	int j;

	for (j = 0; j < stripeImageWidth; j++) {
		stripeImage[j][0] = (GLubyte) ((j<=16) ? 255 : 0);
		stripeImage[j][1] = (GLubyte) ((j>16) ? 255 : 0);
		stripeImage[j][2] = (GLubyte) 0;
		stripeImage[j][3] = (GLubyte) 255;
	}
}

/*  planes for texture coordinate generation  */
static GLfloat xequalzero[] = {0.0, 0.0, 1.0, 0.0};
static GLfloat slanted[] = {1.0, 1.0, 1.0, 0.0};
static GLfloat *currentCoeff;
static GLenum currentPlane;
static GLint currentGenMode;

static
 void init(void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	makeStripeImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

#ifdef GL_VERSION_1_1
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_1D, texName);
#endif
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
#ifdef GL_VERSION_1_1
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, stripeImageWidth, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, stripeImage);
#else
	glTexImage1D(GL_TEXTURE_1D, 0, 4, stripeImageWidth, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, stripeImage);
#endif

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	currentCoeff = xequalzero;
	currentGenMode = GL_OBJECT_LINEAR;
	currentPlane = GL_OBJECT_PLANE;
#ifndef TEX_GEN_SPHERE
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, currentGenMode);
	glTexGenfv(GL_S, currentPlane, currentCoeff);
#else
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
#endif

	glEnable(GL_TEXTURE_GEN_S);
#ifdef TEX_GEN_SPHERE
	glEnable(GL_TEXTURE_GEN_T);
#endif
	glEnable(GL_TEXTURE_1D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glMaterialf (GL_FRONT, GL_SHININESS, 64.0);
}

static
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix ();
	glRotatef(45.0, 0.0, 0.0, 1.0);
#ifdef GL_VERSION_1_1
	glBindTexture(GL_TEXTURE_1D, texName);
#endif
	glutSolidTeapot(2.0);
	glPopMatrix ();
	glFlush();
}

static
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho (-3.5, 3.5, -3.5*(GLfloat)h/(GLfloat)w, 
		3.5*(GLfloat)h/(GLfloat)w, -3.5, 3.5);
	else
		glOrtho (-3.5*(GLfloat)w/(GLfloat)h, 
		3.5*(GLfloat)w/(GLfloat)h, -3.5, 3.5, -3.5, 3.5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static
void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	  case 'e':
	  case 'E':
		  currentGenMode = GL_EYE_LINEAR;
		  currentPlane = GL_EYE_PLANE;
		  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, currentGenMode);
		  glTexGenfv(GL_S, currentPlane, currentCoeff);
		  glutPostRedisplay();
		  break;
	  case 'o':
	  case 'O':
		  currentGenMode = GL_OBJECT_LINEAR;
		  currentPlane = GL_OBJECT_PLANE;
		  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, currentGenMode);
		  glTexGenfv(GL_S, currentPlane, currentCoeff);
		  glutPostRedisplay();
		  break;
	  case 's':
	  case 'S':
		  currentCoeff = slanted;
		  glTexGenfv(GL_S, currentPlane, currentCoeff);
		  glutPostRedisplay();
		  break;
	  case 'x':
	  case 'X':
		  currentCoeff = xequalzero;
		  glTexGenfv(GL_S, currentPlane, currentCoeff);
		  glutPostRedisplay();
		  break;
	  case 27:
		  exit(0);
		  break;
	  default:
		  break;
	}
}

int mainTexgen(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(256, 256);
	glutInitWindowPosition(100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
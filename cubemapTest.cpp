#include "gl/glew.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#define	imageSize 4
static GLubyte image1[imageSize][imageSize][4];
static GLubyte image2[imageSize][imageSize][4];
static GLubyte image3[imageSize][imageSize][4];
static GLubyte image4[imageSize][imageSize][4];
static GLubyte image5[imageSize][imageSize][4];
static GLubyte image6[imageSize][imageSize][4];

static GLdouble ztrans = 0.0;

static
void makeImages(void)
{
	int i, j, c;

	for (i = 0; i < imageSize; i++) {
		for (j = 0; j < imageSize; j++) {
			c = ((((i&0x1)==0)^((j&0x1))==0))*255;
			image1[i][j][0] = (GLubyte) c;
			image1[i][j][1] = (GLubyte) c;
			image1[i][j][2] = (GLubyte) c;
			image1[i][j][3] = (GLubyte) 255;

			image2[i][j][0] = (GLubyte) c;
			image2[i][j][1] = (GLubyte) c;
			image2[i][j][2] = (GLubyte) 0;
			image2[i][j][3] = (GLubyte) 255;

			image3[i][j][0] = (GLubyte) c;
			image3[i][j][1] = (GLubyte) 0;
			image3[i][j][2] = (GLubyte) c;
			image3[i][j][3] = (GLubyte) 255;

			image4[i][j][0] = (GLubyte) 0;
			image4[i][j][1] = (GLubyte) c;
			image4[i][j][2] = (GLubyte) c;
			image4[i][j][3] = (GLubyte) 255;

			image5[i][j][0] = (GLubyte) 255;
			image5[i][j][1] = (GLubyte) c;
			image5[i][j][2] = (GLubyte) c;
			image5[i][j][3] = (GLubyte) 255;

			image6[i][j][0] = (GLubyte) c;
			image6[i][j][1] = (GLubyte) c;
			image6[i][j][2] = (GLubyte) 255;
			image6[i][j][3] = (GLubyte) 255;
		}
	}
}

static
void init(void)
{
	GLfloat diffuse[4] = {1.0, 1.0, 1.0, 1.0};

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	makeImages();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, imageSize, 
		imageSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, imageSize,
		imageSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, image4);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, imageSize,
		imageSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, imageSize,
		imageSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, image5);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, imageSize,
		imageSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, image3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, imageSize,
		imageSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, image6);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP); 
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_TEXTURE_CUBE_MAP);   
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glMaterialfv (GL_FRONT, GL_DIFFUSE, diffuse);
}

static
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix ();
	glTranslatef (0.0, 0.0, ztrans);
	glutSolidSphere(5.0, 20, 10);
	glPopMatrix ();
	glutSwapBuffers();
}

static
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -20.0);
}

static
void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	  case 'f':
		  ztrans = ztrans - 0.2;
		  glutPostRedisplay();
		  break;
	  case 'b':
		  ztrans = ztrans + 0.2;
		  glutPostRedisplay();
		  break;
	  case 27:
		  exit(0);
		  break;
	  default:
		  break;
	}
}

int mainCubemap(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
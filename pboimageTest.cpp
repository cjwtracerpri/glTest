#include "gl/glew.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#define READ 1

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))
/*Create checkerboard image*/
#define checkImageWidth 64
#define checkImageHeight 64

GLubyte checkImage[checkImageHeight][checkImageWidth][3];
static GLdouble zoomFactor = 1.0;
static GLint height;
static GLuint pixelBuffer;
static GLsizei imageWidth = checkImageWidth;
static GLsizei imageHeight = checkImageHeight;
static GLsizei numComponents = 4; /* four components for GL_RGBA */
static GLsizei bufferSize;

static
void makeCheckImage(void)
{
	int i, j, c;
	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = (((i&0x8)==0)^((j&0x8)==0))*255;
			checkImage[i][j][0] = (GLubyte) c;
			checkImage[i][j][1] = (GLubyte) c;
			checkImage[i][j][2] = (GLubyte) c;
		}
	}
}

static
void init(void)
{
#if READ
	glewInit();
	bufferSize = imageWidth * imageHeight * numComponents * sizeof(GLfloat);
	glGenBuffers(1, &pixelBuffer);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBuffer);
	glBufferData(GL_PIXEL_PACK_BUFFER, bufferSize, NULL, GL_STREAM_READ);
#else
	glewInit();
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenBuffers(1, &pixelBuffer);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelBuffer);
	glBufferData(GL_PIXEL_UNPACK_BUFFER,
		3*checkImageWidth*checkImageHeight,
		checkImage, GL_STATIC_DRAW);
#endif
}

static
void display(void)
{
#if READ
	int i;
	GLsizei numPixels = imageWidth * imageHeight;
	glReadPixels(0, 0, imageWidth, imageHeight, GL_RGBA, GL_FLOAT, BUFFER_OFFSET(0));
	GLfloat *pixels = (GLfloat *)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
	for ( i = 0; i < numPixels; ++i ) {
		/* insert your pixel processing here
		process( &pixels[i*numComponents] );
		*/
	}
	glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
#else
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelBuffer);
	glDrawPixels(checkImageWidth, checkImageHeight, GL_RGB,
		GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
	glFlush();
#endif
}

static
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	height = (GLint) h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	  case 'r':
	  case 'R':
		  zoomFactor = 1.0;
		  glutPostRedisplay();
		  printf ("zoomFactor reset to 1.0\n");
		  break;
	  case 'z':
		  zoomFactor += 0.5;
		  if (zoomFactor >= 3.0) 
			  zoomFactor = 3.0;
		  printf ("zoomFactor is now %4.1f\n", zoomFactor);
		  break;
	  case 'Z':
		  zoomFactor -= 0.5;
		  if (zoomFactor <= 0.5) 
			  zoomFactor = 0.5;
		  printf ("zoomFactor is now %4.1f\n", zoomFactor);
		  break;
	  case 27:
		  exit(0);
		  break;
	  default:
		  break;
	}
}

static
void motion(int x, int y)
{
	static GLint screeny;

	screeny = height - (GLint) y;
	glRasterPos2i (x, screeny);
	glPixelZoom (zoomFactor, zoomFactor);
	glCopyPixels (0, 0, checkImageWidth, checkImageHeight, GL_COLOR);
	glPixelZoom (1.0, 1.0);
	glFlush ();
}

int mainPboimage(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(250, 250);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
	glutMainLoop();
	return 0; 
}
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef GL_VERSION_1_2
#define	iWidth 16
#define	iHeight 16
#define iDepth 16

static GLubyte image[iDepth][iHeight][iWidth][3];
static GLuint texName;

/*  Create a 16x16x16x3 array with different color values in
 *  each array element [r, g, b].  Values range from 0 to 255.
 */

static void makeImage(void)
{
   int s, t, r;
    
   for (s = 0; s < 16; s++)
      for (t = 0; t < 16; t++)
         for (r = 0; r < 16; r++) {
            image[r][t][s][0] = (GLubyte) (s * 17);
            image[r][t][s][1] = (GLubyte) (t * 17);
            image[r][t][s][2] = (GLubyte) (r * 17);
         }
}

static void init(void)
{    
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);

   makeImage();
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_3D, texName);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, 
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, 
                   GL_NEAREST);
   glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, iWidth, iHeight,
                iDepth, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
   glEnable(GL_TEXTURE_3D);
}

static void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glBegin(GL_QUADS);
   glTexCoord3f(0.0, 0.0, 0.0); glVertex3f(-2.25, -1.0, 0.0);
   glTexCoord3f(0.0, 1.0, 0.0); glVertex3f(-2.25, 1.0, 0.0);
   glTexCoord3f(1.0, 1.0, 1.0); glVertex3f(-0.25, 1.0, 0.0);
   glTexCoord3f(1.0, 0.0, 1.0); glVertex3f(-0.25, -1.0, 0.0);

   glTexCoord3f(0.0, 0.0, 1.0); glVertex3f(0.25, -1.0, 0.0);
   glTexCoord3f(0.0, 1.0, 1.0); glVertex3f(0.25, 1.0, 0.0);
   glTexCoord3f(1.0, 1.0, 0.0); glVertex3f(2.25, 1.0, 0.0);
   glTexCoord3f(1.0, 0.0, 0.0); glVertex3f(2.25, -1.0, 0.0);
   glEnd();
   glFlush();
}

static void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -4.0);
}

static void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

int mainTexture3d(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(250, 250);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc (keyboard);
   glutMainLoop();
   return 0; 
}
#else
int mainTexture3d(int argc, char** argv)
{
	const GLubyte *vStr = glGetString(GL_VERSION);
	GLint vMajor, vMinor;
	glGetIntegerv(0x821B, &vMajor);
	glGetIntegerv(0x821C, &vMinor);

    fprintf (stderr, "This program demonstrates a feature which is not in OpenGL Version 1.0 or 1.1.\n");
    fprintf (stderr, "If your implementation of OpenGL has the right extensions,\n");
    fprintf (stderr, "you may be able to modify this program to make it run.\n");
    return 0;
}
#endif

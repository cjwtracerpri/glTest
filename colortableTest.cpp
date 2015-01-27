#include "gl/glew.h"
#include <GL/glut.h>
#include <stdlib.h>

extern GLubyte*  readImage(const char*, GLsizei*, GLsizei* );

static
GLubyte  *pixels;
static
GLsizei   width, height;


static
void init(void)
{
   int   i;
   GLubyte  colorTable[256][3];
   
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glClearColor(0.0, 0.0, 0.0, 0.0);

   /* Set up an inverse color table */
   
   for ( i = 0; i < 256; ++i ) {
       colorTable[i][0] = 255 - i;
       colorTable[i][1] = 255 - i;
       colorTable[i][2] = 255 - i;
   }

   glColorTable(GL_COLOR_TABLE, GL_RGB, 256, GL_RGB, GL_UNSIGNED_BYTE,
		 colorTable);
   glEnable(GL_COLOR_TABLE);
}

static
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glRasterPos2i( 1, 1 );
   glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels );
   glFlush();
}

static
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho (0, w, 0, h, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
}

static
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
   }
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int mainColortable(int argc, char** argv)
{
   //pixels = readImage("Data/leeds.bin", &width, &height);    

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(width, height);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}
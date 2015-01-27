#include "gl/glew.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

static int bgtoggle = 1;

/*  
 *  Print out state values related to multisampling.
 *  Create display list with "pinwheel" of lines and
 *  triangles. 
 */
static
void init(void)
{
   static GLint buf[1], sbuf[1];
   int i, j;

   glClearColor(0.0, 0.0, 0.0, 0.0);
   glGetIntegerv (GL_SAMPLE_BUFFERS_ARB, buf);
   printf ("number of sample buffers is %d\n", buf[0]);
   glGetIntegerv (GL_SAMPLES_ARB, sbuf);
   printf ("number of samples is %d\n", sbuf[0]);

   glNewList (1, GL_COMPILE);
   for (i = 0; i < 19; i++) {
      glPushMatrix();
      glRotatef(360.0*(float)i/19.0, 0.0, 0.0, 1.0);
      glColor4f (1.0, 1.0, 1.0, 0.6);
      glLineWidth((i%3)+1.0);
      glBegin (GL_LINES);
         glVertex2f (0.25, 0.05);
         glVertex2f (0.9, 0.2);
      glEnd ();
      glColor4f (0.0, 1.0, 1.0, 0.3);
      glBegin (GL_TRIANGLES);
         glVertex2f (0.25, 0.0);
         glVertex2f (0.9, 0.0);
         glVertex2f (0.875, 0.10);
      glEnd ();
      glPopMatrix();
   }
   glEndList ();

   glNewList (2, GL_COMPILE);
   glColor4f (1.0, 0.5, 0.0, 0.2);
   glBegin (GL_QUADS);
   for (i = 0; i < 16; i++) {
      for (j = 0; j < 16; j++) {
         if (((i + j) % 2) == 0) {
            glVertex2f (-2.0 + (i * 0.25), -2.0 + (j * 0.25));
            glVertex2f (-2.0 + (i * 0.25), -1.75 + (j * 0.25));
            glVertex2f (-1.75 + (i * 0.25), -1.75 + (j * 0.25));
            glVertex2f (-1.75 + (i * 0.25), -2.0 + (j * 0.25));
         }
      }
   }
   glEnd ();
   glEndList ();
}

/*  Draw two sets of primitives, so that you can 
 *  compare the user of multisampling against its absence.
 *
 *  This code enables antialiasing and draws one display list
 *  and disables and draws the other display list
 */
static
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   if (bgtoggle) 
      glCallList (2);

   glEnable (GL_MULTISAMPLE);
   glPushMatrix();
   glTranslatef (-1.0, 0.0, 0.0);
   glCallList (1);
   glPopMatrix();

   glDisable (GL_MULTISAMPLE);
   glPushMatrix();
   glTranslatef (1.0, 0.0, 0.0);
   glCallList (1);
   glPopMatrix();
   glutSwapBuffers();
}

static
void reshape(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= (2 * h)) 
      gluOrtho2D (-2.0, 2.0, 
         -2.0*(GLfloat)h/(GLfloat)w, 2.0*(GLfloat)h/(GLfloat)w);
   else 
      gluOrtho2D (-2.0*(GLfloat)w/(GLfloat)h, 
         2.0*(GLfloat)w/(GLfloat)h, -2.0, 2.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

static
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 'b':
      case 'B':
         bgtoggle = !bgtoggle;
         glutPostRedisplay();
         break;
      case 27:  /*  Escape Key  */
         exit(0);
      default:
         break;
    }
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int mainMultisamp(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
   glutInitWindowSize (600, 300);
   glutCreateWindow (argv[0]);
   init();
   glutReshapeFunc (reshape);
   glutKeyboardFunc (keyboard);
   glutDisplayFunc (display);
   glutMainLoop();
   return 0;
}
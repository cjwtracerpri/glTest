#include <GL/glut.h>
#include <stdlib.h>

static
GLuint listName;

static void init (void)
{
	int n;
	glGetIntegerv(GL_MAX_LIST_NESTING, &n);
   listName = glGenLists (1);
   glNewList (listName, GL_COMPILE);
	  glPushMatrix();
      glColor3f (1.0, 0.0, 0.0);  /*  current color red  */
      glBegin (GL_TRIANGLES);
      glVertex2f (-1.0, 0.0);
      glVertex2f (0.0, 0.0);
      glVertex2f (-1.0, 1.0);
      glEnd ();
      glTranslatef (1.5, 0.0, 0.0); /*  move position  */
	  glPopMatrix();
   glEndList ();
   glShadeModel (GL_FLAT);
}

static void drawLine (void)
{
   glBegin (GL_LINES);
   glVertex2f (0.0, 0.5);
   glVertex2f (5.0, 0.5);
   glEnd ();
}

static
void display(void)
{
   GLuint i;

   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (0.0, 1.0, 0.0);  /*  current color green  */
   for (i = 0; i < 10; i++)    /*  draw 10 triangles    */
      glCallList (listName);
   drawLine ();  /*  is this line green?  NO!  */
                 /*  where is the line drawn?  */
   glFlush ();
}

static
void reshape(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h) 
      gluOrtho2D (-2, 2, -2 * (GLfloat) h/(GLfloat) w, 
         2 * (GLfloat) h/(GLfloat) w);
   else 
      gluOrtho2D (-1 * (GLfloat) w/(GLfloat) h, 1 * (GLfloat) w/(GLfloat) h, -1, 1); 
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

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int mainList(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(650, 50);
   glutCreateWindow(argv[0]);
   init ();
   glutReshapeFunc (reshape);
   glutDisplayFunc (display);
   glutKeyboardFunc (keyboard);
   glutMainLoop();
   return 0;
}
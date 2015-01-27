#include "GL/glew.h"
#include "GL/glut.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>

#define M_PI       3.14159265358979323846
#define NumberOf(array) (sizeof(array)/sizeof(array[0]))

typedef struct {
	GLfloat x, y, z;
} vec3;


typedef struct {
	vec3 xlate; /* Translation */
	GLfloat angle;
	vec3 axis;
} XForm;

enum { Cube, Cone, NumVAOs };
static
GLuint VAO[NumVAOs];
static
GLenum PrimType[NumVAOs];
static
GLsizei NumElements[NumVAOs];
static
XForm Xform[NumVAOs] = {
	{ { -2.0, 0.0, 0.0 }, 0.0, { 0.0, 1.0, 0.0 } },
	{ { 0.0, 0.0, 2.0 }, 0.0, { 1.0, 0.0, 0.0 } }
};
static
GLfloat Angle = 0.0;

static
void
init()
{
	enum { Vertices, Colors, Elements, NumVBOs };
	GLuint buffers[NumVBOs];
	glGenVertexArrays(NumVAOs, VAO);
	{
		GLfloat cubeVerts[][3] = {
			{ -1.0, -1.0, -1.0 },
			{ -1.0, -1.0, 1.0 },
			{ -1.0, 1.0, -1.0 },
			{ -1.0, 1.0, 1.0 },
			{ 1.0, -1.0, -1.0 },
			{ 1.0, -1.0, 1.0 },
			{ 1.0, 1.0, -1.0 },
			{ 1.0, 1.0, 1.0 },
		};
		GLfloat cubeColors[][3] = {
			{ 0.0, 0.0, 0.0 },
			{ 0.0, 0.0, 1.0 },
			{ 0.0, 1.0, 0.0 },
			{ 0.0, 1.0, 1.0 },
			{ 1.0, 0.0, 0.0 },
			{ 1.0, 0.0, 1.0 },
			{ 1.0, 1.0, 0.0 },
			{ 1.0, 1.0, 1.0 },
		};
		GLubyte cubeIndices[] = {
			0, 1, 3, 2,
				4, 6, 7, 5,
				2, 3, 7, 6,
				0, 4, 5, 1,
				0, 2, 6, 4,
				1, 5, 7, 3
		};
		glBindVertexArray(VAO[Cube]);
		glGenBuffers(NumVBOs, buffers);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, cubeVerts);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors), cubeColors, GL_STATIC_DRAW);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, cubeColors);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
		PrimType[Cube] = GL_QUADS;
		NumElements[Cube] = NumberOf(cubeIndices);
	}
	{
		int i, idx;
		float dTheta;
#define NumConePoints 36
		/* We add one more vertex for the cone's apex */
		GLfloat coneVerts[1+1][3] = {
			{0.0, 0.0, 1.0}
		};
		GLfloat coneColors[NumConePoints+1][3] = {
			{1.0, 1.0, 1.0}
		};
		GLubyte coneIndices[NumConePoints+1];
		dTheta = 2*M_PI / (NumConePoints - 1);
		idx = 1;
		for (i = 0; i < NumConePoints; ++i, ++idx) {
			float theta = i*dTheta;
			coneVerts[idx][0] = cos(theta);
			coneVerts[idx][1] = sin(theta);
			coneVerts[idx][2] = 0.0;
			coneColors[idx][0] = cos(theta);
			coneColors[idx][1] = sin(theta);
			coneColors[idx][2] = 0.0;
			coneIndices[idx] = idx;
		}
		glBindVertexArray(VAO[Cone]);
		glGenBuffers(NumVBOs, buffers);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(coneVerts), coneVerts, GL_STATIC_DRAW);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, coneVerts);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(coneColors), coneColors, GL_STATIC_DRAW);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, coneColors);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Elements]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(coneIndices), coneIndices, GL_STATIC_DRAW);
		PrimType[Cone] = GL_TRIANGLE_FAN;
		NumElements[Cone] = NumberOf(coneIndices);
	}
	glEnable(GL_DEPTH_TEST);
}

static
void
display()
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(Angle, 0.0, 1.0, 0.0);
	for (i = 0; i < NumVAOs; ++i) {
		glPushMatrix();
		glTranslatef(Xform[i].xlate.x, Xform[i].xlate.y,
			Xform[i].xlate.z);
		glRotatef(Xform[i].angle, Xform[i].axis.x,
			Xform[i].axis.y, Xform[i].axis.z);
		glBindVertexArray(VAO[i]);
		glDrawElements(PrimType[i], NumElements[i], GL_UNSIGNED_BYTE, (GLvoid*)VAO[i]);
		glPopMatrix();
	}
	glPopMatrix();
	glutSwapBuffers();
}

static
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	if (w <= h)
		glOrtho(-2.5, 2.5, -2.5*(GLfloat)h/(GLfloat)w,
		2.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-2.5*(GLfloat)w/(GLfloat)h,
		2.5*(GLfloat)w/(GLfloat)h, -2.5, 2.5, -10.0, 10.0);
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

int mainVao(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (350, 350); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc (keyboard);
	glutMainLoop();
	return 0;
}
/******************************************************************************
 * RtQtGLTexture.h class for wrapping opengl textures for QT
 *
 * Oliver Hinds <ohinds@mit.edu> 2010-10-13
 * 
 * based on an the 'texture' opengl example qt program, 
 * which is Copyright (C) 1992-2008 Trolltech ASA.  
 *
 ****************************************************************************/

#include "RtQtGLTexture.h"

#include <qimage.h>
#include <qtimer.h>

const int REDRAW_PERIOD = 50;

/*!
  Create a RtQtGLTexture widget
*/

RtQtGLTexture::RtQtGLTexture(QWidget* parent, const char* name)
    : QGLWidget(parent, name)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(update()));
    timer->start(REDRAW_PERIOD, TRUE);
}


/*!
  Release allocated resources
*/

RtQtGLTexture::~RtQtGLTexture()
{
    makeCurrent();
}


/*!
  Paint the texobj. The actual openGL commands for drawing the texobj are
  performed here.
*/

void RtQtGLTexture::paintGL()
{
//    if (animation) {
//		xRot += 1.0;
//		yRot += 2.5;
//		zRot -= 5.0;
//    }
//    glClear(GL_COLOR_BUFFER_BIT);
//    glPushMatrix();
//    glRotatef(xRot, 1.0, 0.0, 0.0); 
//    glRotatef(yRot, 0.0, 1.0, 0.0); 
//    glRotatef(zRot, 0.0, 0.0, 1.0);
//    glScalef(scale, scale, scale);
//    glCallList(object);
//    glPopMatrix();
//
//    if (animation) {
//		glFlush();
//		timer->start(REDRAW_PERIOD, TRUE);
//    }
}


/*!
  Set up the OpenGL rendering state, and define display list
*/

void RtQtGLTexture::initializeGL()
{
    // Set up the lights

//    GLfloat whiteDir[4] = {2.0, 2.0, 2.0, 1.0};
//    GLfloat whiteAmb[4] = {1.0, 1.0, 1.0, 1.0};
//    GLfloat lightPos[4] = {30.0, 30.0, 30.0, 1.0};
//
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
//    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteAmb);
//
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDir);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, whiteDir);
//    glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
//
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDir);		// enable diffuse
//    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteDir);	// enable specular
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
//
//    // Set up the textures
//
//    QImage tex1, tex2, buf;
//
//    if (!buf.load("gllogo.bmp")) {	// Load first image from file
//		qWarning("Could not read image file, using single-color instead.");
//		QImage dummy(128, 128, 32);
//		dummy.fill(Qt::green.rgb());
//		buf = dummy;
//    }
//    tex1 = QGLWidget::convertToGLFormat(buf);  // flipped 32bit RGBA
//
//    if (!buf.load("qtlogo.bmp")) {	// Load first image from file
//		qWarning("Could not read image file, using single-color instead.");
//		QImage dummy(128, 128, 32);
//		dummy.fill(Qt::red.rgb());
//		buf = dummy;
//    }
//    tex2 = QGLWidget::convertToGLFormat(buf);  // flipped 32bit RGBA
//
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glEnable(GL_TEXTURE_2D);
//
//    // Set up various other stuff
//
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(0.0, (double) width, 0.0, (double) height, 1.0, -1.0);
//
//    glClearColor(0.0, 0.0, 0.0, 0.0); // Let OpenGL clear to black
//    glEnable(GL_CULL_FACE);  	// don't need Z testing for convex objects
//    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
//
//    // Make the object display list
//
//    object = makeObject(tex1, tex2);	// Generate an OpenGL display list
}



/*!
  Set up the OpenGL view port, matrix mode, etc.
*/

void RtQtGLTexture::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 10.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -70.0);
}


/*!
  Generate an OpenGL display list for the object to be shown, i.e. the texobj
*/

GLuint RtQtGLTexture::makeObject(const QImage& tex1, const QImage& tex2)
{
//    GLUquadricObj* q = gluNewQuadric();
//    GLuint cylinderObj = glGenLists(1);
//    glNewList(cylinderObj, GL_COMPILE);
//
//    glTranslatef(0.0, 0.0, -1.0);
//
//    // cylinder
//    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex1.width(), tex1.height(), 0,
//				 GL_RGBA, GL_UNSIGNED_BYTE, tex1.bits());
//    gluQuadricTexture(q, GL_TRUE);
//    gluCylinder(q, 0.6, 0.6, 2.0, 24, 1);
//
//    // end cap
//    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex2.width(), tex2.height(), 0,
//				 GL_RGBA, GL_UNSIGNED_BYTE, tex2.bits());
//    glTranslatef(0.0, 0.0, 2.0);
//    gluDisk(q, 0.0, 0.6, 24, 1);
//
//    // other end cap
//    glTranslatef(0.0, 0.0, -2.0);
//    gluQuadricOrientation(q, (GLenum)GLU_INSIDE);
//    gluDisk(q, 0.0, 0.6, 24, 1);
//
//    glEndList();
//    gluDeleteQuadric(q);
//
//    return cylinderObj;
	return 0;
}

#include "ImageWidget.h"

#include <iostream>

#include <QtWidgets>
//#include <QtOpenGL>

using std::cout;
using std::endl;
using std::vector;

ImageWidget::ImageWidget(QWidget *parent)
  : QGLWidget(parent)
{
}

ImageWidget::~ImageWidget()
{
}

QSize ImageWidget::minimumSizeHint() const {
  return QSize(50, 50);
}

QSize ImageWidget::sizeHint() const {
  return QSize(200, 200);
}

void ImageWidget::addImage(RtMRIImage *img) {
  if (images.empty()) {
    images.push_back(Image());
  }
  images.back().setData(img);
  QMetaObject::invokeMethod(this, "updateGL", Qt::AutoConnection);
}


void ImageWidget::initializeGL() {
  initializeOpenGLFunctions();

#ifdef GL_TEXTURE_2D
  glEnable(GL_TEXTURE_2D);
#endif

}

void ImageWidget::paintGL() {
  qglClearColor(Qt::black);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0);
  glDepthFunc(GL_LEQUAL);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  /* gl init stuff */
  glPushMatrix();
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  // hack to start with a black bg
  glBegin(GL_POINTS);
  glVertex3f(0.0, 0.0, -100.0);
  glEnd();

  glEnable(TEXTURE_TYPE);

  for (vector<Image>::iterator it = images.begin(); it != images.end();
       ++it) {
    it->paint();
  }

  glDisable(TEXTURE_TYPE);

  glFlush();

}

void ImageWidget::resizeGL(int width, int height) {
  int side = qMin(width, height);
  glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void ImageWidget::mousePressEvent(QMouseEvent *event) {
  cout << event->x() << " " << event->y() << endl;
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event) {

}

void ImageWidget::mouseReleaseEvent(QMouseEvent * /* event */) {
  ///emit clicked();
}

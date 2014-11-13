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

QSize ImageWidget::minimumSizeHint() const
{
  return QSize(50, 50);
}

QSize ImageWidget::sizeHint() const
{
  return QSize(200, 200);
}

void ImageWidget::initializeGL()
{
  initializeOpenGLFunctions();

#ifdef GL_TEXTURE_2D
  glEnable(GL_TEXTURE_2D);
#endif

}

void ImageWidget::paintGL()
{
  qglClearColor(Qt::black);
  glClear(GL_COLOR_BUFFER_BIT);

  for (vector<Image>::const_iterator it = images.begin(); it != images.end();
       ++it) {
    glBindTexture(GL_TEXTURE_2D, it->getTextureId());
  }
}

void ImageWidget::resizeGL(int width, int height)
{
  int side = qMin(width, height);
  glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
  cout << event->x() << " " << event->y() << endl;
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{

}

void ImageWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
  ///emit clicked();
}

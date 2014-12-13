#include "ImageWidget.h"

#include <iostream>

#include <QtWidgets>
//#include <QtOpenGL>

#include "RtDataIDs.h"

#include "ActivationImage.h"
#include "MaskImage.h"
#include "MRImage.h"

using std::cout;
using std::endl;
using std::map;
using std::string;

namespace {
  typedef std::pair<string, Image*> Layer;

  const string MR_LAYER = "MRI";
  const string ACTIVATION_LAYER = "Activation";
  const string MASK_LAYER = "Mask";
} // anonymous namespace

ImageWidget::ImageWidget(QWidget *parent)
  : QGLWidget(parent)
{
  layers.insert(Layer(ACTIVATION_LAYER, new ActivationImage()));
  layers.insert(Layer(MR_LAYER, new MRImage()));
  layers.insert(Layer(MASK_LAYER, new MaskImage()));
}

ImageWidget::~ImageWidget()
{
  for (map<string, Image*>::iterator it = layers.begin();
       it != layers.end(); ++it) {
    delete it->second;
  }
}

QSize ImageWidget::minimumSizeHint() const {
  return QSize(50, 50);
}

QSize ImageWidget::sizeHint() const {
  return QSize(200, 200);
}

void ImageWidget::addImage(RtData *img) {

  if (img->getDataID().getModuleID() == ID_CURRENTACTIVATION) {
    layers[ACTIVATION_LAYER]->setData(img);
  }
  else if (img->getDataID().getDataName() == NAME_SCANNERIMG_EPI) {
    layers[MR_LAYER]->setData(img);
  }
  else if (img->getDataID().getDataName() == NAME_MASK) {
    layers[MASK_LAYER]->setData(img);
  }
  else {
    return;
  }

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

  // paint the layers in order
  layers[MR_LAYER]->paint();
  layers[ACTIVATION_LAYER]->paint();
  layers[MASK_LAYER]->paint();

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

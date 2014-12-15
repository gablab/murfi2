#include "ImageWidget.h"

#include <iostream>

#include <QtWidgets>
//#include <QtOpenGL>

#include "qcustomplot.h"

#include "RtDataIDs.h"

#include "ActivationImage.h"
#include "MaskImage.h"
#include "MRImage.h"

using std::cout;
using std::endl;
using std::map;
using std::mutex;
using std::string;
using std::vector;

namespace {
  typedef std::pair<string, Image*> Layer;

  const string MR_LAYER = "MRI";
  const string ACTIVATION_LAYER = "Activation";
} // anonymous namespace

ImageWidget::ImageWidget(QWidget *parent)
  : QGLWidget(parent)
  , roiPlotWidget(NULL)
  , motionPlotWidget(NULL)
  , num_mask_images(0)
{}

ImageWidget::~ImageWidget()
{
  deinitRun();
}

void ImageWidget::initRun(RtConfigFmriRun &config) {
  deinitRun();

  draw_order.push_back(MR_LAYER);
  layers.insert(Layer(MR_LAYER, new MRImage()));

  layers.insert(Layer(ACTIVATION_LAYER, new ActivationImage()));
  draw_order.push_back(ACTIVATION_LAYER);

  vector<string> masks = config.getProcessingModuleNames("mask-load", "roiID");
  for (vector<string>::iterator it = masks.begin(); it != masks.end(); ++it) {
    cout << "it: " << *it << endl;
    layers.insert(Layer(*it, new MaskImage(num_mask_images)));
    draw_order.push_back(*it);
    num_mask_images++;
  }
}

void ImageWidget::deinitRun() {
  draw_order.clear();
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

void ImageWidget::handleData(RtData *img) {
  if (img->getDataID().getModuleID() == ID_CURRENTACTIVATION) {
    draw_mutex.lock();
    layers[ACTIVATION_LAYER]->setData(img);
    draw_mutex.unlock();
  }
  else if (img->getDataID().getDataName() == NAME_SCANNERIMG_EPI) {
    draw_mutex.lock();
    layers[MR_LAYER]->setData(img);
    draw_mutex.unlock();
  }
  else if (img->getDataID().getDataName() == NAME_MASK) {
    string roi_name = img->getDataID().getRoiID();
    if (layers.find(roi_name) == layers.end()) {
      return;
    }

    draw_mutex.lock();
    layers[roi_name]->setData(img);
    draw_mutex.unlock();
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

  draw_mutex.lock();
  // paint the layers in order
  for (vector<string>::const_iterator it = draw_order.begin();
       it != draw_order.end(); ++it) {
    layers[*it]->paint();
  }
  draw_mutex.unlock();

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

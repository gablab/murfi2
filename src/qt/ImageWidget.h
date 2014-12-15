#pragma once

#include <map>
#include <mutex>
#include <string>

#include <QtWidgets>
#include <QGLWidget>
#include <QOpenGLFunctions>

#include "RtConfigFmriRun.h"
#include "RtMRIImage.h"

#include "Image.h"

class QCustomPlot;

class ImageWidget : public QGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit ImageWidget(QWidget *parent=0);
  ~ImageWidget();

  QSize minimumSizeHint() const;
  QSize sizeHint() const;

  void initRun(RtConfigFmriRun &config);
  void deinitRun();

  void handleData(RtData *img);

  void setRoiWidget(QCustomPlot *widget) {
    roiPlotWidget = widget;
  }

  void setMotionWidget(QCustomPlot *widget) {
    motionPlotWidget = widget;
  }

 signals:
  void clicked();

 protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

 private:
  void makeObject();

  std::mutex draw_mutex;

  std::map<string, Image*> layers;
  std::vector<string> draw_order;

  QCustomPlot* roiPlotWidget;
  QCustomPlot* motionPlotWidget;

  int num_mask_images;
};

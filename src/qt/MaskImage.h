#pragma once

#include <QColor>
#include <QtOpenGL>
#include "Image.h"

class MaskImage : public Image {
 public:

  MaskImage(const QColor &color);

  virtual ~MaskImage() {}

 private:

  void updateTexture();

  QColor color;
  float opacity;
};

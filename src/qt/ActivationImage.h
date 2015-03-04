#pragma once

#include <QtOpenGL>
#include "Image.h"

class ActivationImage : public Image {
 public:

  ActivationImage();

  virtual ~ActivationImage() {}

 private:

  void updateTexture();

  float threshold;
  float ceiling;
};

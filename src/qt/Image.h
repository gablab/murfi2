#pragma once

#include <QtOpenGL>

class Image {
 public:

  Image() {}

  ~Image() {}

  GLint getTextureId() const {
    return texture_id;
  }

  void makeTexture();

 private:

  short *data;
  GLint texture_id;
};

#pragma once

#include <QtOpenGL>
#include "RtMRIImage.h"

#ifndef TEXTURE_TYPE
#ifdef WIN32
#define TEXTURE_TYPE GL_TEXTURE_2D
#else
#ifdef GL_TEXTURE_RECTANGLE_EXT
#ifndef GL_TEXTURE_RECTANGLE_NV
#define GL_TEXTURE_RECTANGLE_NV GL_TEXTURE_RECTANGLE_EXT
#endif
#else
#ifdef GL_TEXTURE_RECTANGLE_NV
#ifndef GL_TEXTURE_RECTANGLE_EXT
#define GL_TEXTURE_RECTANGLE_EXT GL_TEXTURE_RECTANGLE_NV
#endif
#endif
#endif
#define TEXTURE_TYPE GL_TEXTURE_RECTANGLE_EXT
#endif
#endif

class Image {
 public:

  Image();

  ~Image() {}

  void setData(RtMRIImage *img);

  void paint();

 private:

  void updateTexture();

  RtMRIImage *data;
  bool needs_update;

  GLuint texture_id;
  bool first_image;

  int image_width;
  int image_height;

};

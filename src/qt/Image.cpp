#include "Image.h"

Image::Image()
  : data(NULL)
  , needs_update(false)
  , texture_id(0)
  , first_image(true)
  , image_width(0)
  , image_height(0) {}

void Image::setData(RtMRIImage *img) {
  data = img;
  needs_update = true;
}

void Image::updateTexture() {
  if (glIsTexture(texture_id)) {
    glDeleteTextures(1, &texture_id);
  }

  glGenTextures(1, &texture_id);

  if (first_image) {
    /* contrast */
    float contrast = data->getAutoContrast();
    glPixelTransferf(GL_RED_SCALE,   contrast);
    glPixelTransferf(GL_GREEN_SCALE, contrast);
    glPixelTransferf(GL_BLUE_SCALE,  contrast);

    /* brightness */
    float brightness = data->getAutoBrightness();
    glPixelTransferf(GL_RED_BIAS,   brightness);
    glPixelTransferf(GL_GREEN_BIAS, brightness);
    glPixelTransferf(GL_BLUE_BIAS,  brightness);

    first_image = false;
  }

  /* create the image texture */
  glBindTexture(TEXTURE_TYPE, texture_id);
  glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  short *data_ptr = NULL;
  if(!data->isMosaic()) {
    data_ptr = data->getMosaicedCopy();
    image_width = data->getMosaicedWidth();
    image_height = data->getMosaicedHeight();
  }
  else {
    data_ptr = data->getDataCopy();
    image_width = data->getDim(0);
    image_height = data->getDim(1);
  }

  glTexImage2D(TEXTURE_TYPE, 0, 1, image_width, image_height,
                 0, GL_LUMINANCE, GL_SHORT, data_ptr);

  if(!glIsTexture(texture_id)) {
    cerr << "ERROR: could not generate a new texture" << endl;
  }
}

void Image::paint() {
  if (needs_update) {
    updateTexture();
    needs_update = false;
  }

  glBindTexture(TEXTURE_TYPE, texture_id);

  glBegin(GL_QUADS); {
    glTexCoord2d(0.0, 0.0);
    glVertex3f(-1.0, 1.0, 0.0);
    glTexCoord2d(image_width, 0.0);
    glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2d(image_width, image_height);
    glVertex3f(1.0, -1.0, 0.0);
    glTexCoord2d(0.0, image_height);
    glVertex3f(-1.0, -1.0, 0.0);
  } glEnd();
}

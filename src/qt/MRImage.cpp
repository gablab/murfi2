#include "MRImage.h"

#include "RtMRIImage.h"

MRImage::MRImage()
  : Image()
  , first_image(true)
{}

void MRImage::updateTexture() {
  have_data = false;

  if (glIsTexture(texture_id)) {
    glDeleteTextures(1, &texture_id);
  }

  glGenTextures(1, &texture_id);

  RtMRIImage *img = static_cast<RtMRIImage*>(data);

  if (first_image) {
    /* contrast */
    float contrast = img->getAutoContrast();
    glPixelTransferf(GL_RED_SCALE,   contrast);
    glPixelTransferf(GL_GREEN_SCALE, contrast);
    glPixelTransferf(GL_BLUE_SCALE,  contrast);

    /* brightness */
    float brightness = img->getAutoBrightness();
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
  if(!img->isMosaic()) {
    data_ptr = img->getMosaicedCopy();
    image_width = img->getMosaicedWidth();
    image_height = img->getMosaicedHeight();
  }
  else {
    data_ptr = img->getDataCopy();
    image_width = img->getDim(0);
    image_height = img->getDim(1);
  }

  glTexImage2D(TEXTURE_TYPE, 0, 1, image_width, image_height,
                 0, GL_LUMINANCE, GL_SHORT, data_ptr);

  delete [] data_ptr;

  if(!glIsTexture(texture_id)) {
    cerr << "ERROR: could not generate a new texture" << endl;
  }
  else {
    have_data = true;
  }
}

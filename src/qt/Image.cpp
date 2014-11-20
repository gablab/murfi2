#include "Image.h"

Image::Image()
  : data(NULL)
  , needs_update(false)
  , have_data(false)
  , texture_id(0)
  , image_width(0)
  , image_height(0) {}

void Image::setData(RtData *img) {
  data = img;
  needs_update = true;
}

void Image::paint() {
  if (needs_update) {
    updateTexture();
    needs_update = false;
  }

  if (!have_data) {
    return;
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

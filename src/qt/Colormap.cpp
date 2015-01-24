#include "Colormap.h"

using std::map;
using std::pair;
using std::string;
using std::vector;

namespace {
  void buildDesignColormap(vector<QColor*> *colors) {
    colors->push_back(new QColor(  0,   0, 255));
    colors->push_back(new QColor(255,   0,   0));
    colors->push_back(new QColor(  0, 255,   0));
    colors->push_back(new QColor(255,   0, 255));
    colors->push_back(new QColor(255, 255,   0));
    colors->push_back(new QColor(  0, 255, 255));
    colors->push_back(new QColor(  0,   0,   0));
  }

  void buildROIColormap(vector<QColor*> *colors) {
    colors->push_back(new QColor(  0, 255,   0));
    colors->push_back(new QColor(255,   0, 255));
    colors->push_back(new QColor(  0, 255,   0));
    colors->push_back(new QColor(255, 255,   0));
    colors->push_back(new QColor(  0, 255, 255));
    colors->push_back(new QColor(  0, 127, 127));
    colors->push_back(new QColor(127, 127,   0));
  }

  void buildMotionColormap(vector<QColor*> *colors) {
    colors->push_back(new QColor(  0,   0, 255));
    colors->push_back(new QColor(255,   0,   0));
    colors->push_back(new QColor(  0, 255,   0));
    colors->push_back(new QColor(255,   0, 255));
    colors->push_back(new QColor(255, 255,   0));
    colors->push_back(new QColor(  0, 255, 255));
  }
} // anonymous namespace


Colormap::Colormap(ColormapType type)
  : color_index(0) {
  switch (type) {
    case DESIGN:
      buildDesignColormap(&colors);
      break;
    case ROI:
      buildROIColormap(&colors);
      break;
    case MOTION:
      buildMotionColormap(&colors);
      break;
  }
}

Colormap::~Colormap() {
  for (vector<QColor*>::iterator it = colors.begin();
       it != colors.end(); ++it) {
    delete *it;
  }
}

const QColor& Colormap::getColor(int index) {
  return *(colors[index % colors.size()]);
}

const QColor& Colormap::getColorForName(const string &name) {
  map<string, int>::iterator it = name_colors.find(name);
  if (it == name_colors.end()) {
    color_index++;
    if (color_index >= colors.size()) {
      color_index = 0;
    }

    it = name_colors.insert(pair<string, int>(name, color_index)).first;
  }

  return *(colors[it->second]);
}

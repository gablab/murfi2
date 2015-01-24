#pragma once

#include <map>
#include <string>
#include <vector>

#include <QColor>

class Colormap {

 public:
  enum ColormapType {
    DESIGN = 0,
    ROI,
    MOTION
  };

  Colormap(ColormapType type);
  ~Colormap();

  const QColor& getColor(int index);

  const QColor& getColorForName(const std::string &name);

 private:

  size_t color_index;
  std::vector<QColor*> colors;
  std::map<std::string, int> name_colors;
};

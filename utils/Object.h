#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <functional>

#include "../cyCodeBase/cyMatrix.h"

using TransformFunc = std::function<cy::Matrix4f(cy::Matrix4f)>;

struct Object {
  Object() {}
  virtual void setup() = 0;
  virtual void read(std::string File) = 0;
  virtual void draw(TransformFunc F, cyVec3f Loc) = 0;
};
#endif

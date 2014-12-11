#ifndef _MAP_H_
#define _MAP_H_

#include "MatrixTransform.h"

class Map : public MatrixTransform
{
public:
  Map();
  Map(char* name);

  virtual void setup();
};

#endif

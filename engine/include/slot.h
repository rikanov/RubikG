#ifndef CUBE_SLOT__H
#define CUBE_SLOT__H

#include <cube_framework.h>

struct Slot
{
  Facet  facet = _NF;
  PosID  pos   = 0;
  CubeID rot   = 0;
};

using CacheID = unsigned int;

#endif // ! CUBE_SLOT__H


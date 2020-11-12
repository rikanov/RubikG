#ifndef CUBE_SLOT__H
#define CUBE_SLOT__H

#include <cube_framework.h>

struct CubeSlot
{
  Facet  facet = _NF;
  PosID  pos   = 0;
  CubeID rot   = 0;
};

using CacheID = unsigned int;
using Counter = unsigned int;

#endif // ! CUBE_SLOT__H


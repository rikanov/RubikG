#ifndef CUBE_SLOT__H
#define CUBE_SLOT__H

#include <cube_framework.h>

constexpr unsigned int pow24[] = { 1, 24, 576, 13824, 331776, 7962624 };

struct CubeSlot
{
  Facet  facet = _NF;
  PosID  pos   = 0;
  CubeID rot   = 0;
};

using CacheID  = unsigned int;
using Counter  = unsigned int;
using CubeList = const std::initializer_list<PosID>&;
#endif // ! CUBE_SLOT__H


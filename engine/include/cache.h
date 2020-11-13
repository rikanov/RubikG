#ifndef CACHE__H
#define CACHE__H

#include <slot.h>

class CCache
{
  CubeID  * m_cachedMoves ;
  Counter * m_cacheCounter;
  Counter * m_cacheLevel;
};

#endif // ! CACHE__H

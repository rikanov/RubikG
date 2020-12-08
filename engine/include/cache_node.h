#ifndef CACHE_NODE__H
#define CACHE_NODE__H

#include<slot.h>

template<unsigned int N>
class CacheNode
{
  CacheNode ** m_router;
  public:
  CacheNode()
    : m_router( new CacheNode * [ 3 * 3 * N ] )
  {
    for ( int i = 0; i < 3 * 3 * N; ++ i )
    {
        m_router[i] = this;
    }
  }

  void addRoute( RotID dir, CacheNode * cNode )
  {
    m_router[dir] = cNode;
    
  }
};

#endif // ! CACHE_NODE__H
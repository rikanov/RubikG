#ifndef ENGINE2_HEADER
#define ENGINE2_HEADER

#include <lcache_generator.h>
#include <algorithm>
#include <vector>

template<unsigned int N>
class Engine2
{
  CFramework<N> * m_CFramework;
  Sentinel2<N>  * m_sentinel;

  std::shared_ptr< const CCache > m_cachedRotations;  
  
  Counter     m_depth;
  Counter     m_maxDepth;
  
  RotPath m_solution;    // path to solution
  public:
    Engine2() {}
};
#endif // ! ENGINE2_HEADER
#ifndef ENGINE2_HEADER
#define ENGINE2_HEADER

#include <lcache_generator.h>
#include <algorithm>
#include <vector>

template<unsigned int N>
class Engine2: public Sentinel2<N>
{
  std::shared_ptr< const CCache > m_cachedRotations;  
  
  Counter     m_depth;
  Counter     m_maxDepth;
  
  RotPath m_solution;    // path to solution
  public:
    Engine2()
    : m_depth( 0 )
    , m_maxDepth( 0 ) 
    {};

};
#endif // ! ENGINE2_HEADER
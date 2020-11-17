#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include <cache_generator.h>
#include <algorithm>
#include <vector>

using RotPath = std::vector<RotID>;

template<unsigned int N> 
class Engine
{
  CFramework<N> * m_CFramework;
  Sentinel<N>   * m_sentinel;

  std::shared_ptr< const CCache > m_cachedRotations;  
  
  Counter     m_depth;
  Counter     m_maxDepth;
  
  RotPath m_solution;    // path to solution

  bool testLayer( const Axis , const Layer );
  bool extend   ( const Axis , const Layer );

public:

  Engine( CubeList P, const bool& solidColor );
  Engine( const CGenerator<N>& CGen, const bool& solidColor );

  void toSolve( CFramework<N> & CF );
  int  solve( Counter, bool exec = true );
  
  ~Engine();
  
  CacheID cacheID() const
  {
    return m_sentinel -> getCacheID();
  }

  const RotPath & getsolution() const
  {
    return m_solution;
  }
};

#include <../template_cpp/engine.cpp>        // engine base definitions
#include <../template_cpp/engine_search.cpp> // engine search definitions 

#endif // !ENGINE_HEADER

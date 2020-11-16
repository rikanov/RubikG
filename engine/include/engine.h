#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include <cache_generator.h>
#include <algorithm>
#include <vector>

template<unsigned int N> 
class Engine
{
  CFramework<N> * m_CFramework;
  Sentinel<N>   * m_sentinel;

  std::shared_ptr< const CCache > m_cachedRotations;  
  
  Counter     m_depth;
  Counter     m_maxDepth;
  
  std::vector<RotID> m_solution;    // path to solution
   
public:

  Engine( CubeList P, const bool& solidColor );
  ~Engine();
  
};

template<unsigned int N>
Engine<N>::Engine( CubeList P, const bool& solidColor )
  : m_sentinel ( new Sentinel<N>( P, solidColor ) )
{
  CGenerator<N> CGen( P, solidColor );
  m_cachedRotations = CGen.getCache();
}
  
template<unsigned int N> 
Engine<N>::~Engine()
{
  delete m_sentinel;
}

  
#endif // !ENGINE_HEADER

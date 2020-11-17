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
  Engine( const CGenerator<N>& CGen, const bool& solidColor );
  void toSolve( CFramework<N> & CF );
  ~Engine();
  
};

template<unsigned int N>
Engine<N>::Engine( CubeList P, const bool& solidColor )
  : m_CFramework      ( nullptr )
  , m_sentinel        ( new Sentinel<N>( P, solidColor ) )
  , m_cachedRotations ( CGenerator<N>  ( P, solidColor ).getCache() )
{

}


template<unsigned int N>
Engine<N>::Engine( const CGenerator<N>& CGen, const bool& solidColor )
  : m_CFramework      ( nullptr )
  , m_sentinel        ( new Sentinel<N> ( CGen.getGenerators(), solidColor ) )
  , m_cachedRotations ( CGen.getCache() )
{

}

template<unsigned int N>
void Engine<N>::toSolve( CFramework<N> & CF)
{
  m_CFramework = &CF;
}

template<unsigned int N> 
Engine<N>::~Engine()
{
  delete m_sentinel;
}

  
#endif // !ENGINE_HEADER

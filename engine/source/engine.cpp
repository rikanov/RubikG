/********** Template definitions, do not add this source file to the project build tasks **********/


#include <engine.h>


template<unsigned int N> 
Engine<N>::Engine( const std::initializer_list<PosID> & P, const bool solidColor )
 , m_sentinel            ( P )
 , m_cacheQeueu          ( P.size() )
 , m_cubeCache           ( P.size(), 9 * N )
 , m_numberOfCachedCubes ( P.size() )
 , m_solidColor          ( solidColor )
{
  initCache();
}

template<unsigned int N> 
void Engine<N>::toSolve( CFramework<N> * C )
{
  m_CFramework = C;
  m_sentinel.toSolve( C );
  m_sentinel.update();
}

template<unsigned int N> 
void Engine<N>::constrain( CubeList & P )
{
  m_sentinel.constrain( P )
}

template<unsigned int N> Engine<N>::~Engine()
{
}


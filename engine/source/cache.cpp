/********** Template definitions, do not add this source file to the project build tasks **********/

#include <engine.h>


static constexpr int CacheBufferSize = 8;

using CacheID = unsigned int;

static constexpr int pow24[] = { 1, 24, 576, 13824, 331776, 7962624 };

static inline CacheID getCacheID (const CubeID x0, const CubeID x1 = 0, const CubeID x2 = 0, const CubeID x3 = 0, const CubeID x4 = 0, const CubeID x5 = 0 )
{
  return x0 * pow24[0] + x1 * pow24[1] + x2 * pow24[2] + x3 * pow24[3] + x4 * pow24[4] + x5 * pow24[5];
}

template<const unsigned int N>
void Engine<N>::initCache( const int& numberOfCubes )
{
  delete[] m_cache;
  m_cache = new CubeID [ CacheBufferSize * pow24[ numberOfCubes ] ];
  
  const int storedNumberOfCubes = m_numberOfCubes;
  for ( int i = numberOfCubes; i < m_numberOfCubes; ++i )
    delCube( m_selectedCubes[i] );
  m_numberOfCubes = numberOfCubes;

  CubeID * qeueu  = new CubeID [ CacheBufferSize * pow24[ numberOfCubes ] ];
  CubeID * qeuIn  = qeueu;
  CubeID * qeuOut = qeueu;
}

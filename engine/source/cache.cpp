/********** Template definitions, do not add this source file to the project build tasks **********/

#include <engine.h>

#define all_turns( axis, layer, turn )      \
  for ( Axis axis : { _X, _Y, _Z } )          \
    for ( Layer layer = 0; layer < N; ++layer ) \
      for ( Turn turn: { 1, 2, 3 } ) 

static constexpr int CacheBufferSize = 8;

static constexpr int pow24[] = { 1, 24, 576, 13824, 331776, 7962624 };

static inline CacheID GetCacheID (const CubeID x0, const CubeID x1 = 0, const CubeID x2 = 0, const CubeID x3 = 0, const CubeID x4 = 0, const CubeID x5 = 0 )
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
  
  initQeueu();
  
  while ( m_qeuOut != m_qeuIn )
  {
    setFromQeueu();
    
    all_turns( axis, layer, turn )
    {
      if ( m_counter[axis][layer] > 0  )
      {
        turnLayer( axis, layer );
        addToQeueu();
      }
    }  
  }
  
  for ( int i = numberOfCubes; i < m_numberOfCubes; ++i )
    addCube( m_selectedCubes[i] );
  m_numberOfCubes = storedNumberOfCubes;
}

template<const unsigned int N>
void Engine<N>::setFromQeueu()
{
  for ( int x = *( m_qeuOut ++ ), i = 0; i < m_numberOfCubes; ++ i, x /= 24 )
  {
    delCube( m_selectedCubes[i] );
    m_selectedCubes[i].rot = x % 24;
    addCube( m_selectedCubes[i] );
  }
}

template<const unsigned int N>
void Engine<N>::addToQeueu()
{
  // ToDO: if (...)
 *( m_qeuIn ++ ) = getCacheID ();
}

template<const unsigned int N>
void Engine<N>::initQeueu()
{
  m_qeueu  = new CacheID [ CacheBufferSize * pow24[ m_numberOfCubes ] ];
  m_qeuIn  = m_qeueu;
  m_qeuOut = m_qeueu;
  
  all_id( cubeID )
  {
    CubeID ids[6] = {};
    for ( int i = 0; i < m_numberOfCubes; ++ i )
    {
      ids[i] = Simplex::Composition( m_selectedCubes[i].rot, cubeID );
    }
    *( m_qeuIn ++ ) = GetCacheID( ids[0], ids[1], ids[2], ids[3], ids[4], ids[5] );
  }
}

template<const unsigned int N>
CacheID Engine<N>::getCacheID() const
{
  CubeID ids[6] = {};
  for ( int i = 0; i < m_numberOfCubes; ++ i )
  {
    ids[i] = m_selectedCubes[i].rot;
  }
  return GetCacheID( ids[0], ids[1], ids[2], ids[3], ids[4], ids[5] );
}

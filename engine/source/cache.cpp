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

 // Qeueu
//  -----
template<const unsigned int N>
void Engine<N>::initQeueu()
{
  clog( "init qeueu" );
  m_qeueu  = new CacheID [ pow24[ m_numberOfCachedCubes ] ] ();
  m_qeuIn  = m_qeueu;
  m_qeuOut = m_qeueu;
  
  m_qeueuLevel = 0;
  all_id( cubeID )
  {
    for ( Counter i = 0; i < m_numberOfCachedCubes; ++ i )
    {
      m_selectedCubes[i].rot = cubeID;
    }
    addToQeueu();
  }
  for ( Counter i = 0; i < m_numberOfCachedCubes; ++ i )
  {
    m_selectedCubes[i].rot = 0;
    addCube( m_selectedCubes + i );
  }
  clog( "Done." );
}

template<const unsigned int N>
void Engine<N>::setFromQeueu()
{
  m_qeueuLevel = m_cacheLevel[ *m_qeuOut ] + 1; 
  for ( Counter x = *( m_qeuOut ++ ), i = 0; i < m_numberOfCachedCubes; ++ i, x /= 24 )
  {
    delCube( m_selectedCubes + i );
    m_selectedCubes[i].rot = x % 24;
    addCube( m_selectedCubes + i );
  }
}

template<const unsigned int N>
void Engine<N>::addToQeueu()
{
  static Counter counter = 0;
  static Counter max = 0;
  const CacheID cacheID = getCacheID();
  if ( m_cacheCounter[ cacheID ] == 0 )
  {
    m_cacheLevel[ cacheID ] = m_qeueuLevel;
    *( m_qeuIn ++ ) = cacheID;
    ++ counter;
  }
  if ( m_cacheLevel[ cacheID ] == m_qeueuLevel )
  {
    if ( ++ m_cacheCounter[ cacheID ] > max )
      max = m_cacheCounter[ cacheID ];
    clog_( "\r                              \r", counter, " --> ", m_cacheLevel[ cacheID ], max, (int)( m_qeuIn - m_qeuOut ) );
  }
}

 // Cache
//  -----
template<const unsigned int N>
void Engine<N>::allocateCache()
{
  clog( "Allocate cache size:", pow24[ m_numberOfCachedCubes ] );
  m_cache        = new CubeID  [ pow24[ m_numberOfCachedCubes ] * CacheBufferSize ] ();
  m_cacheLevel   = new Counter [ pow24[ m_numberOfCachedCubes ] ] ();
  m_cacheCounter = new Counter [ pow24[ m_numberOfCachedCubes ] ] ();
  clog( "Done." );
}

template<const unsigned int N>
void Engine<N>::initCache( )
{
  CubeSlot * pCubes = m_selectedCubes;
  for ( PosID pos: m_cachePositions )
  {
    pCubes -> rot = 0;
    pCubes -> pos = pos;
    addCube( pCubes ++ );
  }

  allocateCache();
  initQeueu();
  buildCache();
  
  m_numberOfCubes += m_numberOfCachedCubes;
  clog( "number of cubes", m_numberOfCubes, m_numberOfCachedCubes );
}

template<const unsigned int N>
void Engine<N>::buildCache()
{
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
      turnLayer( axis, layer ); // turn back to original position
    }  
  }
}

template<const unsigned int N>
CacheID Engine<N>::getCacheID() const
{
  switch( m_numberOfCachedCubes )
  {
    case 1:
      return m_selectedCubes[0].rot;
    case 2:
      return m_selectedCubes[0].rot + pow24[1] * m_selectedCubes[1].rot;
    case 3:
      return m_selectedCubes[0].rot + pow24[1] * m_selectedCubes[1].rot + pow24[2] * m_selectedCubes[2].rot;
    case 4:
      return m_selectedCubes[0].rot + pow24[1] * m_selectedCubes[1].rot + pow24[2] * m_selectedCubes[2].rot + pow24[3] * m_selectedCubes[3].rot;
    case 5:
      return m_selectedCubes[0].rot + pow24[1] * m_selectedCubes[1].rot + pow24[2] * m_selectedCubes[2].rot + pow24[3] * m_selectedCubes[3].rot + pow24[4] * m_selectedCubes[4].rot;
    case 6:
      return m_selectedCubes[0].rot + pow24[1] * m_selectedCubes[1].rot + pow24[2] * m_selectedCubes[2].rot + pow24[3] * m_selectedCubes[3].rot + pow24[4] * m_selectedCubes[4].rot + pow24[5] * m_selectedCubes[5].rot;
    default:
      return 0;
  }
}

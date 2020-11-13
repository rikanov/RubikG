/********** Template definitions, do not add this source file to the project build tasks **********/

#include <engine.h>

#define all_turns( axis, layer, turn )      \
  for ( Axis axis : { _X, _Y, _Z } )          \
    for ( Layer layer = 0; layer < N; ++layer ) \
      for ( Turn turn: { 1, 2, 3 } ) 

static constexpr int CacheBufferSize = 8;

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
  m_qeueuLevel = 0;
  all_id( cubeID )
  {
    for ( auto pSlot = m_sentinel.start(); pSlot; pSlot = m_sentinel.next())
    {
      pSlot -> rot = cubeID;
    }
    m_cacheQeueu << m_sentinel.getCacheID();
  }
  for ( auto pSlot = m_sentinel.start(); pSlot; pSlot = m_sentinel.next())
  {
    pSlot -> rot = 0;
  }
  clog( "Done." );
}

template<const unsigned int N>
void Engine<N>::setFromQeueu()
{
  m_qeueuLevel = m_cubeCache.level( m_cacheQeueu.head() ) + 1; 
  Counter x;
  m_cacheQeueu >> x;
  for ( auto pSlot = m_sentinel.start(); pSlot; pSlot = m_sentinel.next() )
  {
    m_sentinel.delCube( pSlot );
    pSlot -> rot = x % 24;
    m_sentinel.addCube( pSlot );
    x /= 24;
  }
}

template<const unsigned int N>
void Engine<N>::addToQeueu()
{
  static Counter counter = 0;
  static Counter max = 0;
  const CacheID cacheID = m_sentinel.getCacheID();
  if ( m_cubeCache.counter( cacheID ) == 0 )
  {
    m_cubeCache.counter( cacheID ) = m_qeueuLevel;
    m_cacheQeueu << cacheID;
    ++ counter;
  }
  if ( m_cacheLevel[ cacheID ] == m_qeueuLevel )
  {
    if ( ++ m_cubeCache.counter( cacheID ) > max )
      max = m_cubeCache.counter( cacheID )
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

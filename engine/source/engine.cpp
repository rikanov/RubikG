/********** Template definitions, do not add this source file to the project build tasks **********/


#include <engine.h>


template<unsigned int N> 
Engine<N>::Engine( const std::initializer_list<PosID> & P, const bool solidColor )
 : m_numberOfCubes       ( 0 )
 , m_sentinel            ( P )
 , m_numberOfCachedCubes ( P.size() )
 , m_solidColor          ( solidColor )
 , m_cachePositions( P )
 , m_selectedCubes ( new CubeSlot [ CPositions<N>::GetSize() ] )
 , m_CFramework    ( nullptr )
 , m_qeueu         ( nullptr )
 , m_cache         ( nullptr )
 , m_cacheCounter  ( nullptr )
 , m_cacheLevel    ( nullptr )
{
  initCache();
}

template<unsigned int N> 
void Engine<N>::toSolve( CFramework<N> * C )
{
  m_CFramework = C;
  updateCubes();
}

template<unsigned int N> 
void Engine<N>::updateCubes()
{
  for ( Counter i = 0; i < m_numberOfCachedCubes; ++ i )
  {
    delCube( m_selectedCubes + i );
    m_selectedCubes[i].facet = CPositions<N>::Side( m_selectedCubes[i].pos );
    m_selectedCubes[i].rot   = m_CFramework -> getCubeID( m_CFramework -> whereIs( m_selectedCubes[i].pos ) );
    addCube( m_selectedCubes + i );
  }
}

template<unsigned int N> 
void Engine<N>::constrain( const std::initializer_list<PosID> & P )
{
  CubeSlot * slotPointer = m_selectedCubes + m_numberOfCubes;
  m_numberOfCubes += P.size();
  for ( PosID pos: P )
  {
    slotPointer -> facet = CPositions<N>::Side( pos );
    slotPointer -> pos   = pos;
    slotPointer -> rot   = m_CFramework ? m_CFramework -> getCubeID( m_CFramework -> whereIs( pos ) ) : 0;
    addCube( slotPointer ++ );    
  }
}

template<unsigned int N> 
void Engine<N>::turnLayer( const Axis axis, const Layer layer )
{
  int parts = m_counter[ axis ][ layer ];
  for( CubeSlot * slotPointer = m_selectedCubes; parts > 0; ++ slotPointer )
  {
    if ( CPositions<N>::GetCoord( slotPointer -> pos, slotPointer -> rot, axis ) == layer )
    {
      delCube( slotPointer );        
      slotPointer -> rot = Simplex::Composition( slotPointer -> rot, Simplex::Tilt( axis ) );
      addCube( slotPointer );
      -- parts;
    }
  }
}

template<unsigned int N> Engine<N>::~Engine()
{
  delete[] m_selectedCubes;
  delete[] m_cache;
  delete[] m_cacheLevel;
  delete[] m_cacheCounter;

  m_selectedCubes = nullptr;
  m_cache         = nullptr;
  m_cacheLevel    = nullptr;
  m_cacheCounter  = nullptr;
}


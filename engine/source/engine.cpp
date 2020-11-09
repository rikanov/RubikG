/********** Template definitions, do not add this source file to the project build tasks **********/


#include <engine.h>


template<unsigned int N> 
Engine<N>::Engine( CFramework<N>& C, const bool solidColor )
 : m_CFramework    ( C )
 , m_selectedCubes ( nullptr )
 , m_numberOfCubes ( 0 )
 , m_solidColor    ( solidColor )
 , m_cache         ( nullptr )
{
}

template<unsigned int N> 
void Engine<N>::constrain( const std::initializer_list<PosID> & P )
{
  m_numberOfCubes = P.size();
  delete[] m_selectedCubes;
  m_selectedCubes = new Slot [ m_numberOfCubes ];
  Slot * slotPointer = m_selectedCubes;
  for ( PosID pos: P )
  {
    slotPointer -> facet = CPositions<N>::Side( pos );
    slotPointer -> pos   = pos;
    slotPointer -> rot   = m_CFramework.getCubeID( m_CFramework.whereIs( pos ) );
    addCube( slotPointer ++ );    
  }
}

template<unsigned int N> Engine<N>::~Engine()
{
  delete[] m_selectedCubes;
  delete[] m_cache;
  m_selectedCubes = nullptr;
  m_cache         = nullptr;
}

template<unsigned int N> 
bool Engine<N>::isTwistedOnAxis( Axis axis ) const
{
  for ( Layer layer = 0; layer < N; ++ layer )
  {
    const PosID first = CPositions<N>::GetLayer( axis, layer, 0 );
    for ( PosID index = 1; index <= CPositions<N>::LayerSize( layer ); ++ index )
    {
      if ( m_CFramework.getCubeID( CPositions<N>::GetLayer( axis, layer, index ) ) != m_CFramework.getCubeID( first ) )
      {
        return false;
      }
    }
  }
  return true;
}

template<unsigned int N> 
void Engine<N>::turnLayer( const Axis axis, const Layer layer )
{
  int parts = m_counter[ axis ][ layer ];
  for( Slot * slotPointer = m_selectedCubes; parts > 0; ++ slotPointer )
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


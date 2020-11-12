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
  m_selectedCubes = new CubeSlot [ m_numberOfCubes ];
  CubeSlot * slotPointer = m_selectedCubes;
  for ( PosID pos: P )
  {
    slotPointer -> facet = CPositions<N>::Side( pos );
    slotPointer -> pos   = pos;
    slotPointer -> rot   = m_CFramework.getCubeID( m_CFramework.whereIs( pos ) );
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
  m_selectedCubes = nullptr;
  m_cache         = nullptr;
}


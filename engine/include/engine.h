#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include <cube_node.h>

struct Slot
{
  PosID pos = 0;
  RotID rot = 0;
};

 // Querry 
template<unsigned int N> 
class Engine
{
  CFramework<N> & m_CFramework;
  
  Slot * m_selectedCubes;
  byte   m_counter[ 3 ][ N ];
  
  void turnLayer( Axis axis, Layer layer );
public:
  Engine ( CFramework<N> & C, PosID * P );
  bool isTwistedOnAxis( Axis A );
};

template<unsigned int N> Engine<N>::Engine(CFramework<N>& C, PosID * P)
 : m_CFramework   ( C )
{
  Slot * slotPointer = m_selectedCubes;
  while ( *P != 0xFF )
  {
    slotPointer->pos = m_CFramework.whereIs( *P );
    slotPointer->rot = m_CFramework.getCubeID( slotPointer->pos );
    
    for ( Axis axis : { _X, _Y, _Z } )
      ++ m_counter[ axis ][ CPositions<N>::GetCoord( slotPointer->pos, axis ) ];
  }
}

template<unsigned int N> 
bool Engine<N>::isTwistedOnAxis(Axis A)
{
  for ( Layer layer = 0; layer < N; ++layer )
  {
    for ( PosID index = 1; index <= CPositions<N>::LayerSize( layer ); ++index )
    {
      if ( m_CFramework.getCubeID( index ) != m_CFramework.getCubeID( 0 ) )
      {
        return false;
      }
    }
  }
  return true;
}

template<unsigned int N> void Engine<N>::turnLayer(Axis axis, Layer layer)
{
  int parts = m_counter[ axis ][ layer ];
  for( Slot * slotPointer = m_selectedCubes; parts > 0; ++ slotPointer )
  {
    if ( CPositions<N>::GetCoord( slotPointer->pos, slotPointer->rot, axis ) == layer )
    {
      for ( Axis toDelete : { _X, _Y, _Z} )
        -- m_counter[ toDelete ][ CPositions<N>::GetCoord( slotPointer->pos, toDelete ) ];
        
      slotPointer->rot = Simplex::Composition( slotPointer->rot, Simplex::Tilt( axis ) );
      
      for ( Axis toAdd : { _X, _Y, _Z} )
        ++ m_counter[ toAdd ][ CPositions<N>::GetCoord( slotPointer->pos, toAdd ) ];
        
      -- parts;
    }
  }
}

#endif // !ENGINE_HEADER

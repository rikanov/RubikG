#ifndef SENTINEL__H
#define SENTINEL__H

#include <slot.h>

template<unsigned int N>
class Sentinel
{
  static constexpr int pow24[] = { 1, 24, 576, 13824, 331776, 7962624 };

  CFramework<N> * m_CFramework;
  CubeSlot      * m_container;
  CubeSlot      * m_containerEnd;
  CubeSlot      * m_slotPointer;
  const Counter m_cacheSize;
  Counter       m_size;
  Counter       m_counter[ 3 ][ N ] = {};
  public:

  Sentinel( CubeList );
  ~Sentinel();

  // inline functions to maintain counter data
  void addCube ( CubeSlot * S ) { for ( Axis axis : { _X, _Y, _Z } ) ++ m_counter[ axis ][ CPositions<N>::GetCoord( S -> pos, S -> rot, axis ) ]; }
  void delCube ( CubeSlot * S ) { for ( Axis axis : { _X, _Y, _Z } ) -- m_counter[ axis ][ CPositions<N>::GetCoord( S -> pos, S -> rot, axis ) ]; }
  
  Counter    id () const        { return m_slotPointer - m_container; }
  CubeSlot & id ( Counter & id) { return m_container[id];             }
  
  CubeSlot * start() { m_slotPointer = m_container; return next();          }
  CubeSlot * next () { return m_slotPointer == m_containerEnd ? nullptr : m_slotPointer ++ ; }

  void update();
  void toSolve( CFramework<N> * CF );
  void addConstrain( CubeList );
  CacheID getCacheID() const;

  void turnLayer( Axis, Layer );
};

template<unsigned int N>
Sentinel<N>::Sentinel( CubeList P )
 : m_CFramework ( nullptr )
 , m_container  ( new CubeSlot [ CPositions<N>::GetSize() ] )
 , m_size       ( 0 )
 , m_cacheSize  ( P.size() )
{
  m_slotPointer  = m_container;
  m_containerEnd = m_container;
  addConstrain( P );
}

template<unsigned int N>
void Sentinel<N>::addConstrain( CubeList P )
{
  m_size += P.size();
  for ( PosID pos: P )
  {
    ( m_containerEnd ++ ) -> pos = pos;
  }
  update();
}

template<unsigned int N>
void Sentinel<N>::update()
{
  for ( Axis axis: { _X, _Y, _Z } )
    for ( Layer layer = 0; layer < N; ++ layer )
    {
      m_counter[ axis ][ layer ] = 0;
    }
  for ( auto pSlot = start(); pSlot; pSlot = next() )
  {
    pSlot -> facet = CPositions<N>::Side( pSlot -> pos );
    if ( m_CFramework )
      pSlot -> rot   = m_CFramework -> getCubeID( m_CFramework -> whereIs( pSlot -> pos ) );
    addCube( pSlot );
  }
}

template<unsigned int N>
void Sentinel<N>::toSolve( CFramework<N> * CF )
{
  m_CFramework = CF;
  update();
}

template<unsigned int N> 
void Sentinel<N>::turnLayer( const Axis axis, const Layer layer )
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

template<unsigned int N>
CacheID Sentinel<N>::getCacheID() const
{
  // not nice but fast
  switch( m_cacheSize )
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

template<unsigned int N>
Sentinel<N>::~Sentinel()
{
  delete[] m_container;
}

#endif // ! SENTINEL__H
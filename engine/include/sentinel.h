#ifndef SENTINEL__H
#define SENTINEL__H

#include <slot.h>

template<unsigned int N>
class Sentinel
{
  CFramework<N>  * m_CFramework;
  CubeSlot       * m_container;
  CubeSlot       * m_containerEnd;
  CubeSlot       * m_slotPointer;
  const Counter  m_cacheSize;
  const bool     m_solidColor;
  Counter        m_size;
  Counter        m_counter[ 3 ][ N ] = {};
  
  mutable const CubeSlot * m_cSlotPointer;
  
  public:

  Sentinel( CubeList, const bool );
  ~Sentinel();

  // inline functions to maintain counter data
  void addCube ( CubeSlot * S ) 
  { 
    for ( Axis axis : { _X, _Y, _Z } ) 
      ++ m_counter[ axis ][ CPositions<N>::GetCoord( S -> pos, S -> rot, axis ) ]; 
  }

  void delCube ( CubeSlot * S ) 
  { 
    for ( Axis axis : { _X, _Y, _Z } ) 
      -- m_counter[ axis ][ CPositions<N>::GetCoord( S -> pos, S -> rot, axis ) ]; 
  }
  
  Counter id () const        
  { 
    return m_slotPointer - m_container; 
  }
  
  CubeSlot & id ( Counter & id) 
  { 
    return m_container[id];             
  }
  
  CubeSlot * start() 
  { 
    m_slotPointer = m_container; return next();          
  }
  
  const CubeSlot * start() const
  { 
    m_cSlotPointer = m_container; return next();          
  }
  
  CubeSlot * next () 
  { 
    return m_slotPointer == m_containerEnd ? nullptr : m_slotPointer ++ ; 
  }
  
  const CubeSlot * next () const
  { 
    return m_cSlotPointer == m_containerEnd ? nullptr : m_cSlotPointer ++ ; 
  }
  
  Counter count( Axis a, Layer l )
  {
    return m_counter[a][l];
  }

  void turnLayer( Axis, Layer );
  void update();
  void toSolve( CFramework<N> * CF );
  bool isSolved() const;
  void addConstrain( CubeList );

  CacheID getCacheID() const;
  void    setCacheID( CacheID );

};

template<unsigned int N>
Sentinel<N>::Sentinel( CubeList P, const bool solidColor )
 : m_CFramework ( nullptr )
 , m_container  ( new CubeSlot [ CPositions<N>::GetSize() ] )
 , m_solidColor ( solidColor )
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
  for( CubeSlot * slotPointer = m_container; parts > 0; ++ slotPointer )
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
      return m_container[0].rot;
    case 2:
      return m_container[0].rot + pow24[1] * m_container[1].rot;
    case 3:
      return m_container[0].rot + pow24[1] * m_container[1].rot + pow24[2] * m_container[2].rot;
    case 4:
      return m_container[0].rot + pow24[1] * m_container[1].rot + pow24[2] * m_container[2].rot + pow24[3] * m_container[3].rot;
    case 5:
      return m_container[0].rot + pow24[1] * m_container[1].rot + pow24[2] * m_container[2].rot + pow24[3] * m_container[3].rot + pow24[4] * m_container[4].rot;
    case 6:
      return m_container[0].rot + pow24[1] * m_container[1].rot + pow24[2] * m_container[2].rot + pow24[3] * m_container[3].rot + pow24[4] * m_container[4].rot + pow24[5] * m_container[5].rot;
    default:
      return 0;
  }
}

template<unsigned int N>
void Sentinel<N>::setCacheID( CacheID cID )
{
  for ( Counter i = 0; i < m_cacheSize; ++ i )
    m_container[i].rot = 0;
  
  for ( int slotID = 0; cID != 0; ++ slotID, cID /= 24 )
  {
    m_container[slotID].rot = cID % 24;
  }
  update();
}

template<unsigned int N>
bool Sentinel<N>::isSolved() const
{
  bool solved = true;
  const CubeID& orient = m_container -> rot;
  for ( auto pSlot = start(); pSlot && solved; pSlot = next() )
  {
    solved  = ( orient ==  pSlot -> rot );
    if ( m_solidColor )
    {
      solved |= pSlot -> facet != _NF && Simplex::GetCube( orient ).whatIs( CPositions<N>::Side( pSlot -> pos, pSlot -> rot ) ) == pSlot -> facet; 
    }
  }
  for ( auto pSlot = start(); pSlot && solved; pSlot = next() )
  {
    clog( CPositions<N>::GetCoord( pSlot -> pos ).toString(), Simplex::GetCube( pSlot -> rot ).toString() );
  }
  return solved;
}

template<unsigned int N>
Sentinel<N>::~Sentinel()
{
  delete[] m_container;
}

#endif // ! SENTINEL__H

#include <sentinel2.h>
#include <lcache_generator.h>

template<unsigned int N>
Sentinel2<N>::Sentinel2()
 : m_CFramework  ( nullptr )
 , m_containerBeg( new CubeSlot [ CPositions<N>::GetSize() ] )
 , m_sequancesBeg( new Sequence [ 10 ] )
{
  m_containerEnd = m_containerBeg;
  m_sequancesEnd = m_sequancesBeg;
  allSequence();
}

template<unsigned int N>
void Sentinel2<N>::update()
{
  for ( Axis axis: { _X, _Y, _Z } )
  {
    for ( Layer layer = 0; layer < N; ++ layer )
    {
      m_counter[ axis ][ layer ] = 0;
    }
  }
  for ( auto pSlot = m_operationBeg; pSlot != m_operationEnd; ++ pSlot )
  {
    pSlot -> facet = CPositions<N>::Side( pSlot -> pos );
    if ( m_CFramework )
    {
      pSlot -> rot   = m_CFramework -> getCubeID( m_CFramework -> whereIs( pSlot -> pos ) );
    }
    addCube( pSlot );
  }
}

template<unsigned int N>
void Sentinel2<N>::toSolve( CFramework<N> * CF )
{
  m_CFramework = CF;
  allSequence();
}

template<unsigned int N> 
void Sentinel2<N>::turnLayer( const Axis axis, const Layer layer, const Turn turn )
{
  int parts = m_counter[ axis ][ layer ];
  for( CubeSlot * slotPointer = m_operationBeg; parts > 0; ++ slotPointer )
  {
    if ( CPositions<N>::GetCoord( slotPointer -> pos, slotPointer -> rot, axis ) == layer )
    {
      delCube( slotPointer );        
      slotPointer -> rot = Simplex::Composition( slotPointer -> rot, Simplex::Tilt( axis, turn ) );
      addCube( slotPointer );
      -- parts;
    }
  }
}

template<unsigned int N> 
void Sentinel2<N>::addSequence(CubeList P)
{
  m_operationBeg = m_containerEnd;
  for ( auto p: P )
  {
    m_containerEnd -> pos = p;
    ++ m_containerEnd;
  }
  m_operationEnd = m_containerEnd;
  
  *m_sequancesEnd = Sequence( m_operationBeg, m_operationEnd );
   m_operationSeq = m_sequancesEnd ++ ;

  LGenerator<N> LGen( this );
  LGen.generate();
  m_operationSeq -> setCache( LGen.getCache() );

  allSequence();
}

template<unsigned int N>
bool Sentinel2<N>::isSolved() const
{
  CubeID fid;
  if ( m_sequancesBeg -> isSolved( fid ) == false )
  {
    return false;
  } 
  clog( "test end" );
  for ( Sequence * pSq = m_sequancesBeg + 1; pSq != m_sequancesEnd; ++ pSq )
  {
    if ( pSq -> level( fid ) > 0 )
    {
      return false;
    }
  }
  return true;
}

template<unsigned int N>
Level Sentinel2<N>::level( CubeID id ) const
{
  Level result = 0;
  for ( Sequence * pSq = m_sequancesBeg; pSq != m_sequancesEnd; ++pSq )
  {
    const Level lid = pSq -> level( id );
    if ( lid > result )
    {
      result = lid;
    }
  }
  return result;
}

template<unsigned int N>
Level Sentinel2<N>::level() const
{
  Level result = level( 0 );
  all_id ( id )
  {
    const Level lid = level( id );
    if ( lid < result )
    {
      result = lid;
    }
  }
  return result;
}

template<unsigned int N>
Sentinel2<N>::~Sentinel2()
{
  delete[] m_containerBeg;
  delete[] m_sequancesBeg;
}

#ifndef INSIGHT__H
#define INSIGHT__H

#include <cache_generator.h>
#include <rubik.h>

template< size_t N >
class Insight
{
  using _crot = CRotations< 2 * N - 3 >;

  CacheID       m_stateID;
  CubeID        m_prior;

  const size_t  m_size;
  const CubeID  m_base;
  const PosID   * m_pos;
  const CacheID * m_baseRotation;
  const CacheID * m_invBaseRotation;
  const CacheIDmap<N> * m_map;

public:
  Insight( SubSpace P );
  Insight( const Insight<N> &, const CubeID );
  ~Insight();

  void set( const Rubik<N> & );

  int rotate( Axis axis, Layer layer, Turn turn );

  void step( const size_t id )
  {
    rotate( m_map -> router( m_stateID, id ) );
  }
  
  CacheID state() const
  {
    return m_stateID;
  }

  int distance() const
  {
    return m_map -> distance( m_stateID );
  }

  void print() const;
};

template< size_t N >
Insight<N>::Insight( SubSpace P )
  : m_size  ( P.size() )
  , m_base  ( 0 )
  , m_baseRotation   ( nullptr )
  , m_invBaseRotation( nullptr )
{
  CacheIDmapper<N> mapBuilder;
  CacheIDmap<N>    * map = new CacheIDmap<N>();
  PosID            * pos = new PosID [ P.size() ];

  mapBuilder.reset( P );
  mapBuilder.createMap( *map );
  
  std::copy( P.begin(), P.end(), pos );
  
  m_map = map;
  m_pos = pos;
}

template< size_t N >
Insight<N>::Insight( const Insight<N> & orig, const CubeID base )
  : m_size ( orig.m_size )
  , m_base ( base )
  , m_map  ( orig.m_map )
{
  PosID  * pos = new PosID [ m_size ];

  for ( size_t i = 0; i < m_size; ++i )
  {
    pos[i] = CPositions<N>::GetPosID( orig.m_pos[i], base );
  }
  
  m_pos = pos;

  CubeID transBase[24];
  all_cubeid( id )
  {
    transBase[id] = Simplex::Conjugation( m_base, id );
  }

  CacheID * baseRotation = new CacheID [ _pow24[ m_size - 1 ] ];
  CacheID * invRotation  = new CacheID [ _pow24[ m_size - 1 ] ];
  for( size_t state = 0; state < _pow24[ m_size - 1 ]; ++ state )
  {
    CacheID stateID  = orig.m_baseRotation ? orig.m_baseRotation[ state ] : state;
    CacheID resultID = 0;
    for( size_t radix = 1; stateID > 0; radix *= 24, stateID /= 24 )
    {
      resultID += transBase[ stateID % 24 ] * radix;
    }
    baseRotation [ state ]    = resultID;
    invRotation  [ resultID ] = state;
  }
  m_baseRotation    = baseRotation;
  m_invBaseRotation = invRotation;
}
template< size_t N >
void Insight<N>::print() const
 {
   PosID * pos = new PosID [ m_size ];
   SetCacheID( pos, m_stateID, m_size, m_prior ); 
   for ( size_t i = 0; i < m_size; ++i )
   {
     clog( CPositions<N>::GetCoord( m_pos[i] ).toString() ,"-->", CPositions<N>::GetCoord( CPositions<N>::GetPosID( m_pos[i], pos[i] ) ).toString() );
   }
   clog( "Order: ", distance(), "Prior: ", Simplex::GetCube( m_prior ).toString() );
   delete[] pos;
 }

template< size_t N >
void Insight<N>::set( const Rubik<N> & R )
{
  m_prior = R.getCubeID( m_pos[0] );
  CubeID * subset = new CubeID [ m_size ];
  for( size_t posIndex = 0; posIndex < m_size; ++ posIndex )
  {
    subset[ posIndex ] = R.getCubeID( m_pos[ posIndex ] );
  }
  m_stateID = GetCacheID( subset, m_size );
  delete[] subset;
}

template< size_t N >
int Insight<N>::rotate( Axis axis, Layer layer, Turn turn )
{
  clog( "state:", m_stateID );
  clog_( _crot::ToString( _crot::GetRotID( axis, layer, turn) ), '*', Simplex::GetCube(  m_base  ).toString(), "-->" );
  RotID rotID = CExtRotations<N>::GetRotID( axis, layer, turn, Simplex::Composition( Simplex::Inverse( m_base ), Simplex::Inverse( m_prior ) ) );
 // 
  if ( ( layer  < N && layer         == CPositions<N>::GetLayer( m_pos[0], m_prior, axis ) ) ||
       ( layer >= N && layer - N + 1 >= CPositions<N>::GetLayer( m_pos[0], m_prior, axis ) ) ) 
  {
    clog_( Simplex::GetCube( m_prior).toString(), "-->" );
    m_prior = Simplex::Tilt( m_prior, axis, turn ); clog( Simplex::GetCube( m_prior).toString() );
  }
  //rotID       = CExtRotations<N>::GetRotID( rotID, Simplex::Inverse( m_prior ) ) ;
  clog_( _crot::ToString( rotID ), ',' ) ;
  clog( (int) ( rotID ) );
  
  if( m_baseRotation )
  { 
    clog( "Before base rotation:\n\n", m_stateID, "-->", m_baseRotation[ m_stateID ]);
    print();
    m_stateID = m_baseRotation[ m_stateID ];
    clog( "After base rotation");
    print();
  }
    m_stateID = m_map -> getState( m_stateID, rotID ) ;

  if( m_invBaseRotation )
  {
    clog( "Before inverse rotation:\n\n", m_stateID, "-->", m_invBaseRotation[ m_stateID ] );
    print();
    m_stateID = m_invBaseRotation[ m_stateID ];
    clog( "After inverse rotation");
  }
    print();
  return distance();
}

template< size_t N >
Insight<N>::~Insight()
{
  delete[] m_pos;
  if ( m_baseRotation == nullptr )
  {
    delete m_map;
    m_map = nullptr;
  }
  else
  {
    delete[] m_baseRotation;
    delete[] m_invBaseRotation;
  }
}
#endif //  ! INSIGHT__H

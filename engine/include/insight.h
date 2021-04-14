#ifndef INSIGHT__H
#define INSIGHT__H

#include <cache_generator.h>
#include <rubik.h>

template<unsigned int N>
class Insight
{
  using _crot = CRotations< 2 * N - 3 >;

  CacheID       m_state;
  CubeID        m_prior;

  const size_t  m_size;
  const bool    m_isBasis;
  const CubeID  m_base;
  const PosID * m_pos;

  const CacheIDmap<N> * m_map;

public:
  Insight( SubSpace P );
  Insight( Insight<N> &, const CubeID );
  ~Insight();

  void set( const Rubik<N> & );

  int rotate( const Axis axis, const Layer layer, const Turn turn );
  int rotate( const RotID rotID  );

  void step( const unsigned int id )
  {
    rotate( m_map -> router( m_state, id ) );
  }
  
  CacheID state() const
  {
    return m_state;
  }

  int distance() const
  {
    return m_map -> distance( m_state );
  }

};

template<unsigned int N>
Insight<N>::Insight( SubSpace P )
  : m_size( P.size() )
  , m_isBasis( true )
  , m_base( 0 )
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

template<unsigned int N>
Insight<N>::Insight( Insight<N> & orig, const CubeID base )
  : m_size   ( orig.m_size )
  , m_isBasis( false )
  , m_base   ( base )
  , m_map    ( orig.m_map )
{
  PosID  * pos = new PosID [ m_size ];

  for ( size_t i = 0; i < m_size; ++i )
  {
    pos[i] = CPositions<N>::GetPosID( orig.m_pos[i], base );
    clog_( CPositions<N>::GetCoord( pos[i] ).toString() );
  }
  
  m_pos = pos;
}

template<unsigned int N>
void Insight<N>::set( const Rubik<N> & R )
{
  m_prior = R.getCubeID( m_pos[0] );
  CubeID * subset = new CubeID [ m_size ];
  for( unsigned int posIndex = 0; posIndex < m_size; ++ posIndex )
  {
    subset[ posIndex ] = R.getCubeID( m_pos[ posIndex ] );
  }
  m_state = GetCacheID( subset, m_size );
  delete[] subset;
}

template<unsigned int N>
int Insight<N>::rotate( const Axis axis, const Layer layer, const Turn turn )
{
 /* if ( m_base > 0 )
  {
    clog( "rotID:", _crot::ToString( _crot::GetRotID( axis, layer, turn, m_base ) ) );
    return rotate( _crot::GetRotID( axis, layer, turn, m_base ) );
  }*/
  m_state = m_map -> getState( m_state, _crot::GetRotID( axis, layer, turn, Simplex::Inverse( m_prior ) ) );

  if ( ( layer  < N && layer         == CPositions<N>::GetLayer( m_pos[0], m_prior, axis ) ) ||
       ( layer >= N && layer - N + 1 <= CPositions<N>::GetLayer( m_pos[0], m_prior, axis ) ) )
  {
    m_prior = Simplex::Tilt( m_prior, axis, turn );
  }

  return distance();
}

template<unsigned int N>
int Insight<N>::rotate( const RotID rotID  )
{
  m_state = m_map -> getState( m_state, _crot::GetRotID( rotID, Simplex::Inverse( m_prior ) ) );

  const Axis  axis  = _crot::GetAxis ( rotID );
  const Layer layer = _crot::GetLayer( rotID );
  if ( ( layer  < N && layer         == CPositions<N>::GetLayer( m_pos[0], m_prior, axis ) ) ||
       ( layer >= N && layer - N + 1 <= CPositions<N>::GetLayer( m_pos[0], m_prior, axis ) ) )
  {
    m_prior = Simplex::Tilt( m_prior, axis, _crot::GetTurn( rotID ) );
  }

  return distance();
}


template<unsigned int N>
Insight<N>::~Insight()
{
  delete[] m_pos;
  if ( m_isBasis )
  {
    delete m_map;
  }
}
#endif //  ! INSIGHT__H

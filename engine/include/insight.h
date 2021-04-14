#ifndef INSIGHT__H
#define INSIGHT__H

#include <cache_generator.h>
#include <rubik.h>

template<unsigned int N>
class Insight: public CacheIDmapper<N>
{

  using _crot = CRotations< 2 * N - 3 >;

  CacheID       m_state;
  CubeID        m_prior;
  CacheIDmap<N> m_map;

public:
  Insight( SubSpace P );
  void set( const Rubik<N> & );

  void rotate( const Axis axis, const Layer layer, const Turn turn )
  {
    m_state = m_map.getState( m_state, _crot::GetRotID( axis, layer, turn, Simplex::Inverse( m_prior ) ) );

    const RotID rotID = _crot::GetRotID( axis, layer, turn);
    if ( ( rotID  < N && layer         == CPositions<N>::GetLayer( this -> m_position[0], m_prior, axis ) ) ||
         ( rotID >= N && layer - N + 1 <= CPositions<N>::GetLayer( this -> m_position[0], m_prior, axis ) ) )
    {
      m_prior = Simplex::Tilt(m_prior, axis, turn );
    }
  }

  void rotate( const RotID rotID  )
  {
    m_state = m_map.getState( m_state, _crot::GetRotID( rotID, Simplex::Inverse( m_prior ) ) );

    const Axis  axis  = _crot::GetAxis ( rotID );
    const Layer layer = _crot::GetLayer( rotID );
    if ( ( rotID  < N && layer         == CPositions<N>::GetLayer( this -> m_position[0], m_prior, axis ) ) ||
         ( rotID >= N && layer - N + 1 <= CPositions<N>::GetLayer( this -> m_position[0], m_prior, axis ) ) )
    {
      m_prior = Simplex::Tilt( m_prior, axis, _crot::GetTurn( rotID ) );
    }
  }

  void step( const unsigned int id )
  {
    rotate( m_map.router( m_state, id ) );
  }
  
  CacheID state() const
  {
    return m_state;
  }
};

template<unsigned int N>
Insight<N>::Insight( SubSpace P )
{
  this -> reset( P );
  this -> createMap( m_map );
}

template<unsigned int N>
void Insight<N>::set( const Rubik<N> & R )
{
  m_prior = R.getCubeID( this -> m_position[0] );
  CubeID * subset = new CubeID [ this -> m_size ];
  for( unsigned int posIndex = 0; posIndex < this -> m_size; ++ posIndex )
  {
    subset[ posIndex ] = R.getCubeID( this -> m_position[ posIndex ] );
  }
  m_state = this -> getCacheID( subset );
  delete[] subset;
}

#endif //  ! INSIGHT__H

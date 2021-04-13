#ifndef INSIGHT__H
#define INSIGHT__H

#include <cache_generator.h>
#include <rubik.h>

template<unsigned int N>
class Insight: public CacheIDmapper<N>
{

  CacheID       m_state;
  CacheIDmap<N> m_map;

public:
  Insight( SubSpace P );
  void set( const Rubik<N> & );

  void rotate( const Axis axis, const Layer layer, const Turn turn, const CubeID cubeID = 0 )
  {
    rotate( _crot::GetRotID( axis, layer, turn ), cubeID );
  }

  void rotate( const RotID rotID, const CubeID cubeID = 0 )
  {
    m_state = m_map.getState( m_state, _crot::GetRotID( rotID, cubeID ) );
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
  reset( P );
  createMap( m_map );
}

template<unsigned int N>
void Insight<N>::set( const Rubik<N> & R )
{
  CubeID * subset = new CubeID [ m_size ];
  for( unsigned int posIndex = 0; posIndex < m_size; ++ posIndex )
  {
    subset[ posIndex ] = R.getCubeID( m_position[ posIndex ] );
  }
  m_state = getCacheID( subset );
  delete[] subset;
}

#endif //  ! INSIGHT__H
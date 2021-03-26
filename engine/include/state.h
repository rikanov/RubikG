#ifndef CUBE_STATE__H
#define CUBE_STATE__H

#include <rubik.h>
#include <slot.h>

template<unsigned int N>
class CState
{
  const unsigned int m_size;
  const CacheID m_cacheID;
  CSlot * m_pattern;
  
  public:
  CState( const CubeList & P );
  ~CState();

  void setCacheID    ( CacheID cacheID );
  void setCacheID    ( const Rubik<N> & R );
  void updateCacheID ();
  CacheID getCacheID () const  
  { 
    return m_cacheID; 
  };

};

template<unsigned int N>
CState<N>::CState( const CubeList & P )
 :  m_pattern( new CSlot[ P.size() ] )
 ,  m_size( P.size() )
 ,  m_cacheID( 0 )
{
  int index = 0;
  for( PosID pos : P )
  {
    m_pattern[ index ++ ].pos = pos;
  }
}

template<unsigned int N>
CState<N>::~CState()
{
  delete[] m_pattern;
}

template<unsigned int N>
void CState<N>::setCacheID( CacheID cacheID )
{
  m_cacheID = cacheID;
  m_pattern[0] = 0;
  for( int i = 1; i < m_size; ++i, cacheID /= 24 )
  {
    m_pattern[i] = cacheID % 24;
  }
}

template<unsigned int N>
void CState<N>::setCacheID( const Rubik<N> & R )
{
  const CubeID trans = Simplex::Inverse( R.getCubeState( m_pattern[0].pos ) );
  m_pattern[0].rot = 0;
  for( int i = 1; i < m_size; ++i )
  {
    m_pattern[i].rot = Simplex::Composition( R.getCubeState( m_pattern[i].pos ), trans );
  }
  updateCacheID();
}

template<unsigned int N>
void CState<N>::updateCacheID()
{

}

#endif // ! CUBE_STATE__H
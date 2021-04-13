#ifndef CACHE_GENERATOR__H
#define CACHE_GENERATOR__H

#include <rubik.h>
#include <containers.h>

template< unsigned int N >
class CacheIDmapper
{

  using _crot = CRotations< 2 * N - 3 >;

protected:
  size_t   m_size;     // number of cubies in the subspace
  PosID  * m_position; // positions of the cubies

private:
  Qeueu  * m_qeueu;

  CacheID  m_parentID;
  CubeID * m_parent;
  CubeID * m_child;

private:
  void clean();
  void setParent();
  void cloneParent();
  void nextChild( const Axis axis, const Layer layer, const Turn turn );

public:
  CacheIDmapper();
  ~CacheIDmapper();

  void reset( SubSpace & P );

  bool acceptID ( CacheID cacheID )   { return *m_qeueu << cacheID;         }
  bool accept   ( const CubeID * P )  { return *m_qeueu << getCacheID( P ); }
  void createMap( CacheIDmap<N> & result );
  
  CacheID getCacheID( const CubeID * P );

private:
  void addLayerRotations( CacheIDmap<N> & result );
  void addSliceRotations( CacheIDmap<N> & result );
};

template<unsigned int N> CacheIDmapper<N>::CacheIDmapper()
: m_size ( 0 )
, m_position ( nullptr )
, m_qeueu    ( nullptr )
, m_parent   ( nullptr )
, m_child    ( nullptr )
{
}

template<unsigned int N>
void CacheIDmapper<N>::reset(SubSpace& P)
{
  clean();

  m_size     = P.size();
  m_parent   = new CubeID [ P.size() ];
  m_child    = new CubeID [ P.size() ];
  m_position = new PosID  [ P.size() ];

  m_qeueu    = new Qeueu ( P.size() - 1 );

  std::copy( P.begin(), P.end(), m_position );

  acceptID( 0 ) ;
}


template<unsigned int N>
void CacheIDmapper<N>::createMap( CacheIDmap<N> & result )
{
  result.init( m_size );
  while( *m_qeueu >> m_parentID )
  {
    setParent();
    addLayerRotations( result );
    addSliceRotations( result );
  }
  clean();
}

template<unsigned int N>
void CacheIDmapper<N>::addLayerRotations( CacheIDmap<N> & result )
{
  all_rot( axis, layer, turn, N )
  {
    cloneParent();
    nextChild( axis, layer, turn );
    const CacheID nextID = getCacheID( m_child );
    result.connect( m_parentID, axis, layer, turn, nextID, *m_qeueu << nextID );
  }
}

template<unsigned int N>
void CacheIDmapper<N>::addSliceRotations( CacheIDmap<N> & result )
{
  for( Axis axis: { _X, _Y, _Z } )
  {
    for( Turn turn : { 1, 2, 3 } )
    {
      cloneParent();
      nextChild( axis, 0, turn );
      for( Layer layer = 1; layer < N - 2; ++layer )
      {
        nextChild( axis, layer, turn );
        const CacheID nextID = getCacheID( m_child );
        result.connect( m_parentID, axis, layer + N - 1, turn, nextID, *m_qeueu << nextID );
      }
    }
  }
}

template<unsigned int N>
CacheID CacheIDmapper<N>::getCacheID( const CubeID * P )
{
  CacheID result = 0;
  const CubeID inv0 = Simplex::Inverse( P[0] );
  for( unsigned int i = 1; i < m_size; ++i )
  {
    result += Simplex::Composition( P[i], inv0 ) * _pow24[ i - 1 ];
  }
  return result;
}

template<unsigned int N>
void CacheIDmapper<N>::nextChild ( const Axis axis, const Layer layer, const Turn turn )
{
  for( unsigned int posIndex = 0; posIndex < m_size; ++ posIndex )
  {
    if ( CPositions<N>::GetLayer( m_position[posIndex], m_child[posIndex], axis ) == layer )
    {
      m_child[posIndex] = Simplex::Composition( m_child[posIndex], Simplex::Tilt( axis, turn) );
    }
  }
}

template<unsigned int N>
void CacheIDmapper<N>::setParent()
{
  CacheID cacheID = m_parentID;
  m_parent[0] = 0;
  for( unsigned int i = 1; i < m_size; ++i, cacheID /= 24 )
  {
    m_parent[i] = cacheID % 24;
  }
}

template<unsigned int N>
void CacheIDmapper<N>::cloneParent()
{
  for( unsigned int i = 0; i < m_size; ++i )
  {
    m_child[i] = m_parent[i];
  }
}

template<unsigned int N>
void CacheIDmapper<N>::clean()
{
  delete[] m_parent;
  delete[] m_child;
  delete   m_qeueu;

  m_parent = nullptr;
  m_child  = nullptr;
  m_qeueu  = nullptr;
}

template<unsigned int N>
CacheIDmapper<N>::~CacheIDmapper()
{
  clean();
  delete[] m_position;
  m_position = nullptr;
}

#endif // ! CACHE_GENERATOR__H

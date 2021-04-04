#ifndef CACHE_GENERATOR__H
#define CACHE_GENERATOR__H

#include <rubik.h>
#include <containers.h>


template< unsigned int N >
class CacheIDmapper
{
  size_t m_size;  // number of cubies in the subspace
  Qeueu  m_qeueu;

  const PosID * m_position;

  CubeID * m_operations;

  CubeID & operation( int posIndex, CubeID rot, Axis axis, Layer layer )
  {
    return m_operations[ layer + N * ( axis + 3*rot + 72*posIndex ) ];
  }

  void initOperations();
  void extendNode( const CacheID parentID, CacheIDmap<N>& refMap );

public:
  CacheIDmapper( const size_t size )
  : m_size  ( size )
  , m_qeueu ( size )
  , m_position( nullptr )
  , m_operations( nullptr )
  {

  }

  ~CacheIDmapper()
  {
    delete[] m_operations;
  }

  void initialize( const PosID * P )
  {
    m_position = P;
    acceptID( 0 ) ;
    initOperations();
  }

  bool generateMap( CacheIDmap<N> & refMap );

  bool acceptID( CacheID cacheID )
  {
    return m_qeueu << cacheID;
  }

  bool accept( const CubeID * P )
  {
    return m_qeueu << getCacheID( P );
  }

  CacheID getCacheID( const CubeID * P );
  void    setCacheID( CacheID cacheID, CubeID * P );
};

template<unsigned int N>
void CacheIDmapper<N>::initOperations()
{
  delete[] m_operations;
  m_operations = new CubeID [ m_size * 24 * 3 * N ];
  for( int posIndex = 0; posIndex < m_size; ++ posIndex )
  {
    all_cubeid( cubeID )
    {
      all_layer( axis, layer, N )
      {
        if ( CPositions<N>::GetLayer( m_position[posIndex], cubeID, axis ) == layer )
        {
          operation( posIndex, cubeID, axis, layer ) = Simplex::Composition( cubeID, Simplex::Tilt( axis ) );
        }
        else
        {
          operation( posIndex, cubeID, axis, layer ) = cubeID;
        }
      }
    }
  }
}


template<unsigned int N>
bool CacheIDmapper<N>::generateMap(CacheIDmap<N>& refMap)
{
  CacheID nextParent;
  while( m_qeueu >> nextParent )
  {
    extendNode( nextParent, refMap );
  }
  return true;
}

template<unsigned int N>
void CacheIDmapper<N>::extendNode(const CacheID parentID, CacheIDmap<N>& refMap )
{
  CubeID child [ m_size ];
  CubeID parent[ m_size ];
  setCacheID( parentID, parent );
  for( int id = 0; id < m_size; ++id )
  {
    for( Axis axis: {_X, _Y, _Z} )
    {
      for( Turn turn : { 1, 2, 3} )
      {
        for( int nid = 0; nid < m_size; ++nid )
        {
          const Layer layer = CPositions<N>::GetLayer( m_position[id], parent[nid], axis );
          child[nid] = operation( id, parent[nid], axis, layer );
          const CacheID nextID = getCacheID( child );
          if ( m_qeueu << nextID )
          {
            refMap.distance( nextID ) = refMap.distance( parentID ) + 1;
          }
          refMap.map( parentID, axis, layer, turn ) = nextID;
        }
      }
    }
  }

}

template<unsigned int N>
CacheID CacheIDmapper<N>::getCacheID(const CubeID* P)
{
  CacheID result = 0;
  const CubeID inv0 = Simplex::Inverse( P[0] );
  for( int i = 1; i < m_size; ++i )
  {
    result += Simplex::Composition( P[i], inv0 ) * _pow24[ i - 1 ];
  }
  return result;
}

template<unsigned int N>
void CacheIDmapper<N>::setCacheID( CacheID cacheID, CubeID * P )
{
  P[0] = 0;
  for( int i = 1; i < m_size; ++i, cacheID /= 24 )
  {
    P[i] = cacheID % 24;
  }
}

#endif // ! CACHE_GENERATOR__H

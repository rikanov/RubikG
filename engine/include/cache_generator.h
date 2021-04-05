#ifndef CACHE_GENERATOR__H
#define CACHE_GENERATOR__H

#include <rubik.h>
#include <containers.h>


template< unsigned int N >
class CacheIDmapper
{
  size_t m_size;  // number of cubies in the subspace
  Qeueu  m_qeueu;

  // result map
  CacheIDmap<N> * m_resMap;

  const PosID * m_position;
  CubeID      * m_operations;

  CubeID & operation( int posIndex, CubeID rot, Axis axis, Layer layer )
  {
    return m_operations[ layer + N * ( axis + 3*rot + 72*posIndex ) ];
  }

  void initOperations();
  void extendNode( const CacheID parentID );

  CacheID getCacheID( const CubeID * P );
  void    setCacheID( CacheID cacheID, CubeID * P );

public:
  CacheIDmapper( const size_t size )
  : m_size  ( size )
  , m_qeueu ( size )
  , m_resMap( new CacheIDmap<N> ( size ) )
  , m_position  ( nullptr )
  , m_operations( nullptr )
  {

  }

  ~CacheIDmapper()
  {
    delete[] m_operations;
    delete   m_resMap;
  }

  void initialize( const PosID * P )
  {
    m_position = P;
    acceptID( 0 ) ;
    initOperations();
  }

  bool acceptID( CacheID cacheID )
  {
    return m_qeueu << cacheID;
  }

  bool accept( const CubeID * P )
  {
    return m_qeueu << getCacheID( P );
  }

  const CacheIDmap<N> * generateMap();
};

template<unsigned int N>
void CacheIDmapper<N>::initOperations()
{
  delete[] m_operations;
  delete   m_resMap;
  m_operations = new CubeID [ m_size * 24 * 3 * N ];
  m_resMap     = new CacheIDmap<N> ( m_size );
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
const CacheIDmap<N> * CacheIDmapper<N>::generateMap()
{
  CacheID nextParent;
  while( m_qeueu >> nextParent )
  {
    extendNode( nextParent );
  }
  return m_resMap;
}

template<unsigned int N>
void CacheIDmapper<N>::extendNode(const CacheID parentID )
{
  CubeID child [ m_size ];
  CubeID parent[ m_size ];
  setCacheID( parentID, parent );
  for( int id = 0; id < m_size; ++id )
  {
    for( Axis axis: {_X, _Y, _Z} )
    {
      const Layer layer = CPositions<N>::GetLayer( m_position[id], parent[id], axis );
      for( Turn turn : { 1, 2, 3} )
      {
        for( int nid = 0; nid < m_size; ++nid )
        {
          child[nid] = operation( id, parent[nid], axis, layer );
          const CacheID nextID = getCacheID( child );
          if ( m_qeueu << nextID )
          {
            m_resMap -> distance( nextID ) = m_resMap -> distance( parentID ) + 1;
          }
          m_resMap -> map( parentID, axis, layer, turn ) = nextID;
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

#ifndef CACHE_GENERATOR__H
#define CACHE_GENERATOR__H

#include <rubik.h>
#include <containers.h>

template< unsigned int N >
class CacheIDmapper
{
  const size_t m_size;  // number of cubies in the subspace
  Qeueu m_qeueu;

  CacheID  m_parentID;
  CubeID * m_parent;
  CubeID * m_child;

  const PosID * m_position;
  CubeID      * m_operations;

  // result map
  CacheIDmap<N> * m_resMap;

private:
  CubeID & operation( uint_fast8_t posIndex, CubeID rot, Axis axis, Layer layer )
  {
    return m_operations[ layer + N * ( axis + 3*rot + 72*posIndex ) ];
  }

  void cloneParent()
  {
    for( uint_fast8_t i = 0; i < m_size; ++ i)
    {
      m_child[i] = m_parent[i];
    }
  }

  void rotateLayer   ( const Axis axis, const Layer layer );
  void initOperations( const Axis axis, const Layer layer, const CubeID cubeID );
  void generateOperations();
  void extendNode();

  CacheID getCacheID( const CubeID * P );
  void    setParent();

public:
  CacheIDmapper( const size_t size )
  : m_size  ( size )
  , m_qeueu ( size )
  , m_parent( new CubeID[ size ] )
  , m_child ( new CubeID[ size ] )
  , m_position  ( nullptr )
  , m_operations( nullptr )
  , m_resMap    ( nullptr )
  {

  }

  ~CacheIDmapper()
  {
    delete[] m_parent;
    delete[] m_child;
    delete[] m_operations;
    delete   m_resMap;
  }

  void initialize( const PosID * P )
  {
    m_position = P;
    acceptID( 0 ) ;
    generateOperations();
  }

  bool acceptID( CacheID cacheID )
  {
    return m_qeueu << cacheID;
  }

  bool accept( const CubeID * P )
  {
    return m_qeueu << getCacheID( P );
  }

  const CacheIDmap<N> * getMap();
};

template<unsigned int N>
void CacheIDmapper<N>::generateOperations()
{
  m_operations = new CubeID [ m_size * 24 * 3 * N ];
  all_cubeid( cubeID )
  {
    all_layer( axis, layer, N )
    {
      initOperations( axis, layer, cubeID );
    }
  }
}

template<unsigned int N>
void CacheIDmapper<N>::initOperations( const Axis axis, const Layer layer, const CubeID cubeID )
{
  for( uint_fast8_t posIndex = 0; posIndex < m_size; ++ posIndex )
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

template<unsigned int N>
const CacheIDmap<N> * CacheIDmapper<N>::getMap()
{
  if ( m_resMap == nullptr )
  {
    m_resMap = new CacheIDmap<N> ( m_size );
    while( m_qeueu >> m_parentID )
    {
      extendNode();
    }
  }
  return m_resMap;
}

template<unsigned int N>
void CacheIDmapper<N>::extendNode()
{
  setParent();
  for( uint_fast8_t id = 0; id < m_size; ++id )
  {
    for( Axis axis: {_X, _Y, _Z} )
    {
      const Layer layer = CPositions<N>::GetLayer( m_position[id], m_parent[id], axis );
      rotateLayer( axis, layer );
    }
  }
}

template<unsigned int N>
void CacheIDmapper<N>::rotateLayer( const Axis axis, const Layer layer )
{
  cloneParent(); 
  for( Turn turn : { 1, 2, 3} )
  {
    for( uint_fast8_t cube = 0; cube < m_size; ++cube )
    {
      m_child[cube] = operation( cube, m_child[cube], axis, layer );
    }
    const CacheID nextID = getCacheID( m_child );
    if ( m_qeueu << nextID )
    {
      m_resMap -> distance( nextID ) = m_resMap -> distance( m_parentID ) + 1;
    }
    m_resMap -> map( m_parentID, axis, layer, turn ) = nextID;
  }
}

template<unsigned int N>
CacheID CacheIDmapper<N>::getCacheID(const CubeID* P)
{
  CacheID result = 0;
  const CubeID inv0 = Simplex::Inverse( P[0] );
  for( uint_fast8_t i = 1; i < m_size; ++i )
  {
    result += Simplex::Composition( P[i], inv0 ) * _pow24[ i - 1 ];
  }
  return result;
}

template<unsigned int N>
void CacheIDmapper<N>::setParent()
{
  CacheID cacheID = m_parentID;
  m_parent[0] = 0;
  for( uint_fast8_t i = 1; i < m_size; ++i, cacheID /= 24 )
  {
    m_parent[i] = cacheID % 24;
  }
}

#endif // ! CACHE_GENERATOR__H

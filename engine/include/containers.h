#ifndef CONTAINERS__H
#define CONTAINERS__H


constexpr unsigned int _pow24[] = { 1, 24, 576, 13824, 331776, 7962624 };

// an excluding FIFO object: any CacheID value can be pushed in only once a life time
class Qeueu
{
  const size_t m_size;

  CacheID * m_qeueudCubes ;
  CacheID * m_qeuIn ;
  CacheID * m_qeuOut;
  bool    * m_used;
public:
  
  Qeueu( const int& size )
  : m_size( size )
  , m_qeueudCubes( new CacheID [ _pow24[ size ] ] )
  {
    m_qeuIn = m_qeuOut = m_qeueudCubes;
    m_used = new bool [ _pow24[ m_size ] ] { false };
  }

  bool operator << ( const CacheID& id )
  {
    if ( m_used[ id ] == false )
    {
      m_used[ id ] = true;
      *( m_qeuIn ++ ) = id;
      return true;
    }
    return false;
  }

  bool operator >> ( CacheID& id ) 
  {
      id = *( m_qeuOut ++ );
      return m_qeuOut <= m_qeuIn;
  }

  int count() const
  {
    return m_qeuIn - m_qeuOut;
  }

  ~Qeueu()
  {
    delete[] m_qeueudCubes;
    delete[] m_used;
  }
};

template<unsigned int N>
class CacheIDmap
{
  CacheID * m_map;
  byte    * m_dist;

public:
  CacheIDmap( size_t size )
  :  m_map ( new CacheID[ _pow24[ size - 1 ] * 3 * N * 3 ] )
  ,  m_dist( new byte   [ _pow24[ size - 1 ] ] )
  {
    for( CacheID cacheID = 0; cacheID < _pow24[ size - 1 ]; ++ cacheID )
    {
      all_rot( axis, layer, turn, N )
      {
        map( cacheID, axis, layer, turn ) = cacheID;
      }
    }
  }

  ~CacheIDmap()
  {
    delete[] m_map;
    delete[] m_dist;
  }

  CacheID & map( CacheID cacheID, Axis axis, Layer layer, Turn turn )
  {
    return m_map[ turn + 3 * layer + 3 * N * axis + 9 * N * cacheID ];
  }

  CacheID map( CacheID cacheID, Axis axis, Layer layer, Turn turn ) const
  {
    return m_map[ -1 + turn + 3 * layer + 3 * N * axis + 9 * N * cacheID ];
  }

  byte & distance( CacheID cacheID )
  {
    return m_dist[ cacheID ];
  }

  byte distance( CacheID cacheID ) const
  {
    return m_dist[ cacheID ];
  }

};

#endif // ! CONTAINERS__H

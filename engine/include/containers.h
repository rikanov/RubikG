#ifndef CONTAINERS__H
#define CONTAINERS__H

#include <initializer_list>

constexpr unsigned int _pow24[] = { 1, 24, 576, 13824, 331776, 7962624 };

using SubSpace = const std::initializer_list <PosID>;

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
  using _crot = CRotations< N >;

  CacheID * m_map;
  DistID  * m_dist;

  RotID  * m_cachedStep;
  DistID * m_complexity;

  void clean()
  {
    delete[] m_map;
    delete[] m_dist;
    delete[] m_cachedStep;
  }

public:
  CacheIDmap()
  :  m_map  ( nullptr )
  ,  m_dist ( nullptr )
  ,  m_cachedStep ( nullptr )
  ,  m_complexity ( nullptr )
  {
    _crot::Instance();
  }

  void init ( const size_t size )
  {
    clean();
    m_map  = new CacheID [ _pow24[ size - 1 ] * _crot::AllRotIDs ];
    m_dist = new DistID  [ _pow24[ size - 1 ] ] {};

    m_cachedStep = new RotID  [ _pow24[ size - 1 ] * _crot::AllRotIDs ];
    m_complexity = new DistID [ _pow24[ size - 1 ] ] {};
  }

  void connect( const CacheID start, const Axis axis, const Layer layer, const Turn turn, const CacheID result, const bool first )
  {
    if ( first )
    {
      m_dist[ result ] = m_dist[ start ] + 1;
    }
    
    const bool closer = m_dist[ result ] == m_dist[ start ] + 1;
    if ( turn == 1 || ( turn == 2 && closer ) ) // no duplicate by inverses
    {
      m_map[ start  * _crot::AllRotIDs + _crot::GetRotID( axis, layer,   turn ) ] = result;
      m_map[ result * _crot::AllRotIDs + _crot::GetRotID( axis, layer, 4-turn ) ] = start;
    }

    if ( closer )
    {
      m_cachedStep[ m_complexity[ result ] ++ ] = _crot::GetRotID( axis, layer, 4-turn );
    }
  }

  CacheID getNext( CacheID cacheID, Axis axis, Layer layer, Turn turn ) const
  {
    return m_map[ _crot::GetRotID( axis, layer, turn) + _crot::AllRotIDs * cacheID ];
  }

  int complexity( const CacheID id ) const
  {
    return m_complexity[ id ];
  }

  int distance( CacheID cacheID ) const
  {
    return m_dist[ cacheID ];
  }

  ~CacheIDmap()
  {
    clean();
  }

};

#endif // ! CONTAINERS__H

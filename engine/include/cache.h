#ifndef CACHE__H
#define CACHE__H

#include <slot.h>

class CCache
{
  RotID       * m_cachedMoves ;
  Counter     * m_cacheCounter;
  Counter     * m_cacheLevel;
  const Counter m_bufferSize;

  mutable 
  const RotID * m_readMoves;

public:
  CCache( const int& size, const int& buf_size )
   : m_cachedMoves ( new RotID   [ pow24[size] * buf_size ] () )
   , m_cacheCounter( new Counter [ pow24[size]            ] () )
   , m_cacheLevel  ( new Counter [ pow24[size]            ] () )
   , m_bufferSize  ( buf_size )
   , m_readMoves   ( nullptr )
  {
  }

  void set( CacheID cacheID, RotID rotID )
  {
    m_cachedMoves[ cacheID * m_bufferSize + m_cacheCounter[cacheID]++ ] = rotID;
  }

  Counter count( CacheID cacheID ) const
  {
    return m_cacheCounter[cacheID];
  }

  Counter & level( CacheID level )
  {
    return m_cacheLevel[level];
  }
  
  const Counter & level ( CacheID level ) const
  {
    return m_cacheLevel[level];
  }
  
  bool suggested ( CacheID cid, RotID rid ) const
  {
    for ( RotID rotID = start( cid ); rotID; rotID = next() )
    {
      if ( rid == rotID )
      {
        return true;
      }
    }
    return false;
  }
  
  const RotID & start( CacheID id ) const
  {
    m_readMoves = m_cachedMoves + ( id * m_bufferSize );
    return next();
  }

  const RotID & next() const
  {
    return *( m_readMoves ++ );
  }
};

#endif // ! CACHE__H

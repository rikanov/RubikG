#ifndef CACHE__H
#define CACHE__H

#include <slot.h>

class CCache
{
  RotID       * m_cachedMoves ;
  Counter     * m_cacheCounter;
  Counter     * m_cacheLevel;
  const Counter m_bufferSize;
public:
  CCache( const int& size, const int& buf_size )
   : m_cachedMoves ( new RotID [ pow24[size] * buf_size ] () )
   , m_cacheCounter( new Counter [ size ] () )
   , m_cacheLevel  ( new Counter [ size ] () )
   , m_bufferSize  ( buf_size )
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
};

#endif // ! CACHE__H

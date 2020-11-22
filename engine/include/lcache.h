#ifndef LEVEL_CACHE__H
#define LEVEL_CACHE__H

#include <slot.h>

using Level = unsigned char;

class LCache
{
  Level ** m_level;
public:
  LCache( const int );
  Level level( CubeID id, CacheID cid ) const
  {
    return m_level[id][cid];
  }
  Level & level( CubeID id, CacheID cid )
  {
    return m_level[id][cid];
  }
};

LCache::LCache( const int size )
 : m_level ( new Level* [24] )
{
  all_id( i )
  {
    m_level[i] = new Level [ pow24[ size ] () ];
  }
}


#endif // ! LEVEL_CACHE__H

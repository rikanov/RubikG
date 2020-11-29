#ifndef LEVEL_CACHE__H
#define LEVEL_CACHE__H

#include <slot.h>

using Level = unsigned char;

class LCache
{
  Level  ** m_level;
  CubeID  * m_fastestWay; 
public:
  LCache( const int );
  ~LCache();
  
  Level level( CubeID id, CacheID cid ) const
  {
    return m_level[id][cid];
  }
  
  void setLevel( CubeID id, CacheID cid, Level lid )
  {
    m_level[id][cid] = lid;
    if ( m_fastestWay[cid] == 0xFF || m_level[ m_fastestWay[cid] ][cid] > lid )
    {
      m_fastestWay[cid] = id;
    }
  }
  
  CubeID getFastest( CacheID cid ) const
  {
    return m_fastestWay[cid];
  }
};

LCache::LCache( const int size )
 : m_level ( new Level* [24] )
 , m_fastestWay ( new CubeID [ pow24[ size ] ] )
{
  all_id( i )
  {
    m_level[i] = new Level [ pow24[ size ] ] ();
  }
  for( CacheID cid = 0; cid < pow24[ size ]; ++ cid )
  {
    m_fastestWay[cid] = 0xFF;
  }
}


LCache::~LCache()
{
  all_id( i )
  {
    delete[] m_level[i];
  }
  delete[] m_level;
  delete[] m_fastestWay;
}

#endif // ! LEVEL_CACHE__H

#ifndef SEQUENCE__H
#define SEQUENCE__H

#include <slot.h>
#include <lcache.h>
#include <cache_generator.h>

class Sequence
{
  CubeSlot * m_sequenceBeg;
  CubeSlot * m_sequenceEnd;
  Counter  m_size;
  
  std::shared_ptr< const LCache  > m_cachedRotations;
  
public:
  Sequence();
  Sequence( CubeSlot * beg, CubeSlot * end );
  
  void setCache( const std::shared_ptr< const LCache >  );
  
  CacheID getCacheID() const;
  void    setCacheID( CacheID );
  
  Counter level( CubeID id ) const
  {
    m_cachedRotations -> level( id, getCacheID() );
  }
};

Sequence::Sequence()
 : m_sequenceBeg( nullptr )
 , m_sequenceEnd( nullptr )
 , m_size       ( 0 )
{
}

Sequence::Sequence( CubeSlot * beg, CubeSlot * end)
 : m_sequenceBeg( beg )
 , m_sequenceEnd( end )
 , m_size       ( end - beg )
{  
}

void Sequence::setCache( const std::shared_ptr< const LCache > cid )
{
  m_cachedRotations = cid;
}

CacheID Sequence::getCacheID() const
{
  // not nice but fast
  switch( m_size )
  {
    case 1:
      return m_sequenceBeg[0].rot;
    case 2:
      return m_sequenceBeg[0].rot + pow24[1] * m_sequenceBeg[1].rot;
    case 3:
      return m_sequenceBeg[0].rot + pow24[1] * m_sequenceBeg[1].rot + pow24[2] * m_sequenceBeg[2].rot;
    case 4:
      return m_sequenceBeg[0].rot + pow24[1] * m_sequenceBeg[1].rot + pow24[2] * m_sequenceBeg[2].rot + pow24[3] * m_sequenceBeg[3].rot;
    case 5:
      return m_sequenceBeg[0].rot + pow24[1] * m_sequenceBeg[1].rot + pow24[2] * m_sequenceBeg[2].rot + pow24[3] * m_sequenceBeg[3].rot + pow24[4] * m_sequenceBeg[4].rot;
    case 6:
      return m_sequenceBeg[0].rot + pow24[1] * m_sequenceBeg[1].rot + pow24[2] * m_sequenceBeg[2].rot + pow24[3] * m_sequenceBeg[3].rot + pow24[4] * m_sequenceBeg[4].rot + pow24[5] * m_sequenceBeg[5].rot;
    default:
      return 0;
  }
}

void Sequence::setCacheID( CacheID cID )
{
  for ( Counter slotID = 0; slotID < m_size; ++ slotID, cID /= 24 )
  {
    m_sequenceBeg[slotID].rot = cID % 24;
  }
}

#endif // ! SEQUENCE__H

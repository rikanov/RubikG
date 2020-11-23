#ifndef LEVEL_CACHE_GENERATOR__H
#define LEVEL_CACHE_GENERATOR__H

#include <sentinel2.h>
#include <qeueu.h>
#include <lcache.h>
#include <memory>

template<unsigned int N>
class LGenerator
{
  Sentinel2<N> * m_sentinel;
  LCache       * m_cachedRotations;
  Qeueu        * m_qeueudCacheIDs;
  Counter        m_qeueuLevel;
  
public:
  LGenerator( Sentinel2 <N> * );
  void generate();
  void initQeueu( CubeID );
};

template<unsigned int N> 
LGenerator<N>::LGenerator( Sentinel2<N> * S)
 : m_sentinel( S )
 , m_cachedRotations( new LCache( S -> size() ) ) /*OWNERSHIP*/
 , m_qeueudCacheIDs ( new Qeueu ( S -> size() ) ) // ToDO
 , m_qeueuLevel( 0 )
{
}

template<unsigned int N> 
void LGenerator<N>::initQeueu( CubeID id )
{
  m_qeueudCacheIDs -> reset();
  for( auto pSlot = m_sentinel -> start(); pSlot; pSlot = m_sentinel -> next() )
  {
    *pSlot = id;
  }
  m_cachedRotations -> level( id, m_sentinel -> getCacheID() ) = 1;
  m_qeueudCacheIDs -> qeuIn( m_sentinel -> getCahceID() );
}

template<unsigned int N> 
void LGenerator<N>::generate()
{
  all_id( id )
  {
    initQeueu( id );
    while( m_qeueudCacheIDs -> count() > 0 )
    {
      const Counter parent = m_qeueudCacheIDs -> qeuOut();
      const Counter nLevel = m_cachedRotations -> level( id, parent ) + 1;
      // generate child nodes
      m_sentinel -> setCacheID( parent );
      for ( Axis axis: { _X, _Y, _Z } )
      {
        for ( Layer layer = 0; layer < N; ++ layer )
        {
          if ( m_sentinel->count( axis, layer ) == 0 )
          {
            continue;
          }
          for( Turn turn: { 1, 2, 3 } )
          {
            m_sentinel->turnLayer( axis, layer );
            const CacheID cid = m_sentinel -> getCacheID();
            if ( m_cachedRotations -> level( id, cid ) == 0 )
            {
              m_cachedRotations -> level( id, cid ) = nLevel;
              m_qeueudCacheIDs  -> qeuIn( cid );
            }
          }
          m_sentinel -> turnLayer( axis, layer );
        } // Layer end
      } // Axis end
    } // Qeueu is empty
  } // Finish 
}

#endif // ! LEVEL_CACHE_GENERATOR__H

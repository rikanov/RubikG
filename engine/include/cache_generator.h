#ifndef CACHE_GENERATOR__H
#define CACHE_GENERATOR__H

#include <sentinel.h>
#include <qeueu.h>
#include <cache.h>

template<unsigned int N>
class CGenerator
{
  Sentinel<N> * m_sentinel;
  Counter     m_qeueuLevel;
  Qeueu       * m_qeueudCacheIDs;
  CCache      * m_cachedRotations;

  void initQeueu();
  void generate();
  
public:
  CGenerator( CubeList P, const bool& solidColor );
  ~CGenerator();
  
  const CCache * getCache() const
  {
    return m_cachedRotations;
  }
};


template<unsigned int N> 
CGenerator<N>::CGenerator( CubeList P, const bool& solidColor )
 : m_sentinel        ( new Sentinel<N>( P, solidColor ) )
 , m_qeueuLevel      ( 0 )
 , m_qeueudCacheIDs  ( new Qeueu ( pow24[ P.size() ] ) )
 , m_cachedRotations ( new CCache( P.size(), 9 * N ) ) /* OWNERSHIP */
{
  generate();
}

template<unsigned int N> 
void CGenerator<N>::initQeueu()
{
  clog( "init qeueu" );
  all_id( id )
  {clog_( (int) id );
    for ( auto pSlot = m_sentinel -> start(); pSlot; pSlot = m_sentinel -> next() )
    {
      pSlot -> rot = id;
    }
    const CacheID next = m_sentinel -> getCacheID();;
    *m_qeueudCacheIDs << next;
    m_cachedRotations -> set( next, 0 );clog_( '_' );
  }
  clog( "\ndone." );
}

template<unsigned int N> 
void CGenerator<N>::generate()
{
  initQeueu();
  Counter counter  = 0;
  Counter maxLevel = 0;
  Counter maxCache = 0;
  while( m_qeueudCacheIDs -> count() > 0 )
  {
    clog_( '\r', counter++, " : ", m_qeueudCacheIDs->count(), " - maxLevel: ", maxLevel, " maxCache: ", maxCache );
    const Counter parent = m_qeueudCacheIDs -> qeuOut();
    const Counter nLevel = m_cachedRotations -> level( parent ) + 1;
    // generate child nodes
    m_sentinel -> setCacheID( parent );
    for ( Axis axis: { _X, _Y, _Z } )
    {
      for ( Layer layer = 0; layer < N; ++ layer )
      {
        for( Turn turn: { 1, 2, 3 } )
        {
          m_sentinel -> turnLayer( axis, layer );
          CacheID id =  m_sentinel -> getCacheID();
          if( m_cachedRotations -> count( id ) == 0 )
          {
            m_qeueudCacheIDs  -> qeuIn( id ); 
            m_cachedRotations -> level( id ) = nLevel;
            if( nLevel > maxLevel )
              maxLevel = nLevel;
          }
          if ( m_cachedRotations -> level( id ) == nLevel )
          {
            m_cachedRotations -> set  ( id, getRotID<N>( axis, layer, 4 - turn ) ); // add inverse
            if ( m_cachedRotations -> count( id ) > maxCache )
              maxCache = m_cachedRotations -> count( id );
          }
        } // turn
        m_sentinel->turnLayer( axis, layer ); // fourth turn: get back to the original position
      } // layer
    }// axis
  }
}

template<unsigned int N> 
CGenerator<N>::~CGenerator()
{
  delete m_sentinel;
  delete m_qeueudCacheIDs;
}


#endif // ! CACHE_GENERATOR__H

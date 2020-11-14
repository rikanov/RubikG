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
  Qeueu       * m_cacheQeueu;
  CCache      * m_cachedRotations;

  void initQeueu();
  
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
 , m_cacheQeueu      ( new Qeueu ( pow24[ P.size() ] ) )
 , m_cachedRotations ( new CCache( P.size(), 9 * N ) ) /* OWNERSHIP */
{
}

template<unsigned int N> 
CGenerator<N>::~CGenerator()
{
  delete m_sentinel;
  delete m_cacheQeueu;
}


#endif // ! CACHE_GENERATOR__H

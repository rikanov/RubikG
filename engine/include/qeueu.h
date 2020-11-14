#ifndef QEUEU__H
#define QEUEU__H

#include <slot.h>

class Qeueu
{
  CacheID * m_qeueudCubes ;
  CacheID * m_qeuIn ;
  CacheID * m_qeuOut;
  CacheID * m_qeuEnd;
public:
  
  Qeueu( const int& size ) : m_qeueudCubes( new CacheID [ size ] )
  {
    m_qeuEnd = m_qeueudCubes + size;
    reset();
  }

  void reset()
  {
    m_qeuIn = m_qeuOut = m_qeueudCubes;
  }

  bool operator << ( const CacheID& id )
  {
      *( m_qeuIn ++ ) = id;
      return true;
  }

  bool operator >> ( CacheID& id ) 
  {
      id = *( m_qeuOut ++ );
      return true;
  }
  
  bool skip ( void ) 
  {
    if ( m_qeuOut < m_qeuIn )
    {
      ++ m_qeuOut;
      return true;
    }
    return false;

  }

  CacheID head() const
  {
    return *m_qeuOut;
  }

  Counter size() const
  {
    return m_qeuEnd - m_qeueudCubes;
  }

  Counter count() const
  {
    return m_qeuIn - m_qeuOut;
  }

  ~Qeueu()
  {
    delete[] m_qeueudCubes;
  }
};


#endif // ! QEUEU__H

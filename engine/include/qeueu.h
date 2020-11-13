#ifndef QEUEU__H
#define QEUEU__H

#include <slot.h>

class Qeueu
{
  CacheID * m_qeueudCubes ;
  CacheID * m_qeuIn ;
  CacheID * m_qeuOut;

public:
  
  Qeueu( const int& size ) : m_qeueudCubes( new CacheID [ size ] )
  {
    m_qeuIn = m_qeuOut = m_qeueudCubes;
  }

  void in( const CacheID& id )
  {
    *( m_qeuIn ++ ) = id;
  }

  bool out( CacheID& id ) 
  {
    if ( m_qeuOut < m_qeuIn )
    {
      id = *( m_qeuOut ++ );
      return true;
    }
    return false;
  }
  
  ~Qeueu()
  {
    delete[] m_qeueudCubes;
  }
};


#endif // ! QEUEU__H
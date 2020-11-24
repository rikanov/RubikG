#ifndef SENTINEL2__H
#define SENTINEL2__H

#include <slot.h>
#include <sequence.h>

template<unsigned int N>
class LGenerator;

template<unsigned int N>
class Sentinel2
{
  CFramework<N>  * m_CFramework;
  
  CubeSlot  * m_containerBeg;
  CubeSlot  * m_containerEnd;
  
  CubeSlot  * m_operationBeg;
  CubeSlot  * m_operationEnd;
  CubeSlot  * m_operationPtr;
  mutable const CubeSlot  * m_operationCPtr;
  
  Sequence  * m_sequancesBeg;
  Sequence  * m_sequancesEnd;
  Sequence  * m_operationSeq;
    
  Counter   m_counter[ 3 ][ N ] = {};
  
  
  // inline functions to maintain counter data
  void addCube ( CubeSlot * S ) 
  { 
    for ( Axis axis : { _X, _Y, _Z } ) 
    {
      ++ m_counter[ axis ][ CPositions<N>::GetCoord( S -> pos, S -> rot, axis ) ]; 
    }
  }

  void delCube ( CubeSlot * S ) 
  { 
    for ( Axis axis : { _X, _Y, _Z } ) 
    {
      -- m_counter[ axis ][ CPositions<N>::GetCoord( S -> pos, S -> rot, axis ) ]; 
    }
  }
  
  void allSequence()
  {
    m_operationBeg = m_containerBeg;
    m_operationEnd = m_containerEnd;
    update();
  }
  public:

  Sentinel2();
  ~Sentinel2();
  
  Counter size() const
  {
    return m_operationEnd - m_operationBeg;
  }
  
  Counter count( Axis a, Layer l ) const
  {
    return m_counter[a][l];
  }

  CacheID getCacheID() const 
  { 
    return m_operationSeq -> getCacheID();
  }
  
  void setCacheID( CacheID id )
  { 
    m_operationSeq -> setCacheID(id); update(); 
  }
  
  const RotID & start() const
  { 
    m_operationCPtr = m_operationBeg;
    return m_operationCPtr ++; 
  }
  
  const RotID & next()  const 
  { 
    return m_operationCPtr != m_operationEnd ? m_operationCPtr ++ : nullptr;
  }
  
  void turnLayer( const Axis, const Layer, const Turn turn = 1 );
  void update();
  void toSolve( CFramework<N> * CF );
  void addSequence( CubeList P );
  
  Level level( CubeID ) const;
  Level level() const;
 
};

#include <../template_cpp/sentinel2.cpp>

#endif // ! SENTINEL2__H


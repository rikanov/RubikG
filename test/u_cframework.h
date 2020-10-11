#ifndef INTEGRITY_CUBE_FRAMEWORK__H
#define INTEGRITY_CUBE_FRAMEWORK__H

#include <cube_framework.h>
#include <cube_basic_rotations.h>

class IFramework_test
{
public:
  virtual bool shuffle() =0;
  virtual bool integrity() const =0;
};
template<unsigned int N>
class CFramework_test: public IFramework_test, public CFramework<N>
{
  
public:
  
  CFramework_test( void )
  : CFramework<N>::CFramework()
  {
    
  }
  CFramework_test( const CFramework<N>& a, const CFramework<N>& b )
  : CFramework<N>::CFramework( a, b )
  {
    
  }
  
  bool shuffle() override;
  bool integrity() const override;
};

template<unsigned int N> 
bool CFramework_test<N>::integrity() const
{
  bool integrity = true;
  // check permutations
  for ( int startIndex = 0; startIndex < CPositions<N>::GetSize(); ++startIndex )
  {
    CubeID testID = 0; // identity
    int nextIndex = startIndex;
    do
    {
      testID = Simplex::Composition( testID, CFramework<N>::getCubeID( nextIndex ) );
      nextIndex = CFramework<N>::whatIs( nextIndex );
    } while ( nextIndex != startIndex );
    if ( testID != 0 ) // not identity
    {
      clog( startIndex, (int) testID );
      integrity = false;
      break;
    }
  }
  return integrity;
}

template<unsigned int N> 
bool CFramework_test<N>::shuffle()
{
  static std::default_random_engine engine( time(0) );
  static std::uniform_int_distribution<int> dist( 2 * N * N, 3 * N * N);
  int counter = dist( engine ); 
  while ( 0 < counter-- )
  {
    CFramework<N>::rotate( randomRotID<N>() );
  }
}

#endif // INTEGRITY_CUBE_FRAMEWORK__H

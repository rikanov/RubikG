#ifndef INTEGRITY_CUBE_FRAMEWORK__H
#define INTEGRITY_CUBE_FRAMEWORK__H

#include <cube_framework.h>
#include <cube_basic_rotations.h>


template<unsigned int N>
class CFramework_test: public CFramework<N>
{
  
public:
  bool shuffle();
  bool integrity() const;
};

template<unsigned int N> 
bool CFramework_test<N>::integrity() const
{
  bool integrity = true;
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

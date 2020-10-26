#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include <cube_node.h>

 // Querry 
template<unsigned int N> 
class Engine
{
  
public:
  static void Path( const CNode<N>* C, RotID* P );
};

 // Querry 
template<unsigned int N> 
void Engine<N>::Path( const CNode<N>* C, RotID* P )
{
  while ( C != CNode<N>::getRoot() )
  {
    *( P++ ) = C -> rotID();
    C = C -> parent();
  }
  *P = 0;
}

#endif


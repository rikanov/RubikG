#include <cube_framework.h>

#ifndef CUBE_STACK_HEADER
#define CUBE_STACK_HEADER

static const int DefaultStackSize = 200;

template<unsigned int N>
class CStack
{
  CFramework<N>*  cubeStack;
  CFramework<N>*  stackPointer;
  
public:
  
  CStack()        : cubeStack( new CFramework<5> [DefaultStackSize] )
                  { stackPointer = cubeStack; }
  
  CStack( int S ) : cubeStack( new CFramework<N> [ S ] )
                  { stackPointer = cubeStack; }
                    
  void store() { *( stackPointer + 1 ) = *( stackPointer++ ); }
  void undo () { --stackPointer; }
  void redo () { ++stackPointer; }
  
  CFramework<N>& pop() 
  { 
    return *stackPointer; 
  } 
  
  operator CFramework<N>& () 
  { 
    return pop(); 
  }
};

#endif

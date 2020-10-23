#include "all_includes.h"
#include <cube_node.h>

int main()
{
  init();
  CNode<3>::Initialize( 5 );
  std::atexit( exit );
  
  UnitTests tests;
  tests.testCore();
  
  CNode<3>::OnExit();
  return 0;
}

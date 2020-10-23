#include "all_includes.h"
#include <cube_node.h>

int main()
{
  CNode<3>::Initialize( 5 );
  init();
  std::atexit( exit );
  
  UnitTests tests;
  tests.testCore();
  
  return 0;
}

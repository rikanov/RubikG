#include "all_includes.h"
#include <engine.h>


int main()
{
  init();
  CFramework<4> A;
  A.print(_L);
  UnitTests tests;
  tests.testCore();
  
  return 0;
}

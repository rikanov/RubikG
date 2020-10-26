#include "all_includes.h"
#include <engine.h>


int main()
{
  init();
  UnitTests tests;
  tests.testCore();
  
  return 0;
}

#include "all_includes.h"

int main()
{
  init();
  std::atexit( exit );
  
  UnitTests tests;
  tests.testCore();
  
  return 0;
}

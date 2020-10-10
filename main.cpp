
#include <iostream>
#include <cstdlib>

#include <test.h>
#include <cli.h>
#include <rubiks_cube.h>

int main()
{
  CLi::init();
  CLi::REPL();
  CLi::exit();
  
  UnitTests tests;
  tests.testCore();
  
  return 0;
}

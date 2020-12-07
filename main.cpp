#include "all_includes.h"
#include <engine.h>


int main()
{
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to start." ); 
  init();
  CRotations<5> rotationIDtest;
  std::cin.get();
  UnitTests tests;
  tests.testCore();
  clog( "Quit." );
  return 0;
}

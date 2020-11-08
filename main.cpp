#include "all_includes.h"
#include <engine.h>


int main()
{
  init();
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to start." ); 
  std::cin.get();
  UnitTests tests;
  tests.testCore();
  clog( "Quit." );
  return 0;
}

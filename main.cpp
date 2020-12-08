#include "all_includes.h"
#include <engine.h>


int main()
{
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to start." ); 
  init();
  clog( CRotations<5>::ToString( CRotations<5>::GetRotID( _X, 3, 3 ) ) );
  clog( CRotations<5>::ToString( CRotations<5>::GetRotID( _Y, 0, 1 ) ) );
  clog( CRotations<5>::ToString( CRotations<5>::GetRotID( _Z, 4, 3 ) ) );
  std::cin.get();
  UnitTests tests;
  tests.testCore();
  clog( "Quit." );
  return 0;
}

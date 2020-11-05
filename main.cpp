#include "all_includes.h"
#include <engine.h>


int main()
{
  init();
  CFramework<4> A;
  A.rotate( _Y, 0 );
  A.rotate( _X, 0 );
  A.rotate( _Y, 0 );
  A.print(_L);
  Engine<4> test( A, { 0, CPositions<4>::GetIndex( 0, 0, 1), CPositions<4>::GetIndex( 0, 1, 0), CPositions<4>::GetIndex( 0, 1, 1) } );
  clog( test.solve( 3 ) );
  A.print(); 
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to end." ); 
  std::cin.get();
  UnitTests tests;
  tests.testCore();
  
  return 0;
}

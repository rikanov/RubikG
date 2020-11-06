#include "all_includes.h"
#include <engine.h>


int main()
{
  init();
  CFramework<4> A;
  A.shuffle( 17 );
  A.print();
  Engine<4> test( A, { 0, CPositions<4>::GetIndex( 1, 0, 0), CPositions<4>::GetIndex( 0, 1, 0), CPositions<4>::GetIndex( 1, 1, 0), CPositions<4>::GetIndex( 3, 3, 3) } );
  clog( Simplex::GetCube( test.solve( 8 ) ).toString() );
  A.print(); 
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to end." ); 
  std::cin.get();
  UnitTests tests;
  tests.testCore();
  
  return 0;
}

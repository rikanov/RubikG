#include "all_includes.h"
#include <engine.h>


int main()
{
  init();
  for ( int i = 0; i < 50; ++ i)
  {
    CFramework<4> A;
    A.shuffle( 5 );
    A.print();
    const std::initializer_list<PosID> middle = {
      CPositions<4>::GetIndex( 0, 1, 1),
      CPositions<4>::GetIndex( 0, 2, 1),
      CPositions<4>::GetIndex( 0, 1, 2),
      CPositions<4>::GetIndex( 0, 2, 2),
      CPositions<4>::GetIndex( 3, 1, 1),
      CPositions<4>::GetIndex( 3, 2, 1),
      CPositions<4>::GetIndex( 3, 1, 2),
      CPositions<4>::GetIndex( 3, 2, 2),
      CPositions<4>::GetIndex( 1, 0, 1),
      CPositions<4>::GetIndex( 2, 0, 1),
      CPositions<4>::GetIndex( 1, 0, 2),
      CPositions<4>::GetIndex( 2, 0, 2),
      CPositions<4>::GetIndex( 1, 3, 1),
      CPositions<4>::GetIndex( 2, 3, 1),
      CPositions<4>::GetIndex( 1, 3, 2),
      CPositions<4>::GetIndex( 2, 3, 2),
      CPositions<4>::GetIndex( 1, 1, 0),
      CPositions<4>::GetIndex( 2, 1, 0),
      CPositions<4>::GetIndex( 1, 2, 0),
      CPositions<4>::GetIndex( 2, 2, 0),
      CPositions<4>::GetIndex( 1, 1, 3),
      CPositions<4>::GetIndex( 2, 1, 3),
      CPositions<4>::GetIndex( 1, 2, 3),
      CPositions<4>::GetIndex( 2, 2, 3)
  };
  Engine<4> test( A, true );
  test.constrain( middle  );
  clog( " start ");
  clog( Simplex::GetCube( test.solve( 8 ) ).toString() );
  A.print(); 
  }
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to end." ); 
  std::cin.get();
  UnitTests tests;
  tests.testCore();
  return 0;
}

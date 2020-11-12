#include <test.h>
#include <engine.h>

bool UnitTests::unit_Engine() const
{
  bool success = true;
  head( "Engine" );

  clog_( "Start Simplex instance..." );
  Simplex::Instance();
  done();

  clog_( "Start 'cube positions' instances..." );
  CPositions<2>::Instance();
  CPositions<3>::Instance();
  CPositions<4>::Instance();
  CPositions<5>::Instance();
  done();

  for ( int i = 0; i < 10; ++ i)
  {
    CFramework<4> A;
    A.shuffle();
    A.print();
    const std::initializer_list<PosID> middle = {
      CPositions<4>::GetIndex( 0, 1, 1),
      CPositions<4>::GetIndex( 0, 2, 1),
      CPositions<4>::GetIndex( 0, 1, 2),
      CPositions<4>::GetIndex( 0, 2, 2) /*,
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
      CPositions<4>::GetIndex( 2, 2, 3) */
      };
    Engine<4> test( A, true );
    test.constrain( middle  );
    clog( " start ");
    clog( Simplex::GetCube( test.solve( 7 ) ).toString() );
    A.print(); 
  }
  /*
  for ( int i = 0; i < 20; ++ i)
  {
    CFramework<3> A;
    A.shuffle( 10 );
    A.print();
    const std::initializer_list<PosID> middle = {
      CPositions<3>::GetIndex( 1, 0, 0),
      CPositions<3>::GetIndex( 0, 1, 0),
      CPositions<3>::GetIndex( 1, 1, 0),
      CPositions<3>::GetIndex( 2, 1, 0),
      CPositions<3>::GetIndex( 1, 2, 0)
      };
    Engine<3> test( A, false );
    test.constrain( middle  );
    clog( " start ");
    clog( Simplex::GetCube( test.solve( 8 ) ).toString() );
    A.print(); 
  }
  */
  clog_( "Cube positions:", Color::bold, "onExit()", Color::off, ':' );
  CPositions<2>::OnExit();
  CPositions<3>::OnExit();
  CPositions<4>::OnExit();
  CPositions<5>::OnExit();
  done();
  
  clog_( "Simplex:", Color::bold, "onExit()", Color::off, ':' );
  Simplex::OnExit();
  done();

  finish( "Engine", success );
  return success;
}
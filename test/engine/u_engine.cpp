#include <test.h>
#include <engine.h>
#include <ctime>

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

  CubeList cache = {
    CPositions<4>::GetIndex( 0, 1, 1),
    CPositions<4>::GetIndex( 0, 2, 1),
    CPositions<4>::GetIndex( 0, 1, 2),
    CPositions<4>::GetIndex( 0, 2, 2),
    CPositions<4>::GetIndex( 3, 1, 1)
  };
  CubeList middle = {
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
  std::time_t start;
  std::time( &start );
  CGenerator<4> gen( cache );
  Engine<4> engine( gen, true );
  engine.addConstrain( middle  );
  std::time_t end;
  std::time( &end );
  NL();
  clog( cache.size(), "dimension cache has built in", end - start, "seconds." );

  for ( int i = 0; i < 10; ++ i )
  {
    CFramework<4> A;
    A.shuffle( 7 );
    A.print();
    engine.toSolve( A );
    clog( " start ", engine.isSolved() );
    clog( engine.solve( 8, true ) );
    A.print(); 
  }
  
  CubeList cross3 = {
    CPositions<3>::GetIndex( 1, 0, 0),
    CPositions<3>::GetIndex( 0, 1, 0),
    CPositions<3>::GetIndex( 1, 1, 0),
    CPositions<3>::GetIndex( 2, 1, 0),
    CPositions<3>::GetIndex( 1, 2, 0)
    };
  std::time( &start );
  Engine<3> engine3 ( cross3, true );
  std::time( &end );
  NL();
  clog( cross3.size(), "dimension cache has built in", end - start, "seconds." );
  for ( int i = 0; i < 10; ++ i)
  {
    CFramework<3> A;
    A.shuffle( 6 );
    A.print();
    engine3.toSolve( A );
    clog( " start ");
    clog( engine3.solve( 8, true ) );
    A.print(); 
  }
  
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

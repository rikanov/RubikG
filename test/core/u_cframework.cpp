#include "test.h"
#include "cube_framework.h"

bool UnitTests::unit_CFramework() const
{
  bool success = true;
  head( "cube framework" );

  clog_( Color::white, "Start Simplex instance..." );
  Simplex::instance();
  clog( Color::uline, "Done." );

  clog_( Color::white, "Start 'cube positions' instances..." );
  CPositions<2>::Instance();
  CPositions<3>::Instance();
  CPositions<4>::Instance();
  CPositions<5>::Instance();
  clog( Color::uline, "Done." );

  clog_( Color::white, "Cube framework test..." );
  CFramework<5> test1, test2;
  clog( Color::uline, "Done." );

  NL();
  NL();

  clog( Color::white, "Start testing turns" );
  clog(               "-------------------" );
  NL();
  clog( Color::white, Color::uline, "Test case 1", Color::off, ": X1, Y1" );
  test1.rot( _X, 1 );
  test1.rot( _Y, 1 );
  test1.print();
  
  clog( Color::white, Color::uline, "Test case 2", Color::off, ": Y1, Z4" );
  test2.rot( _Y, 1);
  test2.rot( _Z, 4);
  test2.print();
  
  clog( Color::white, Color::uline, "Test case 3", Color::off, ": combine of T1 + T2 --> X1, Y1, Y1, Z4" );
  CFramework<5> test3( test1, test2 );
  test3.print();
  
  clog( Color::white, Color::uline, "Test case 4", Color::off, ": inverse of T3" );
  CFramework<5> test4 = test3.inverse();
  test4.print();
  
  clog( Color::white, Color::uline, "Test case 5", Color::off, ": combine of T3 + T4 --> solved cube" );
  CFramework<5> test5(test3,test4);
  test5.print();
  
  clog_( "Cube positions:", Color::bold, "onExit()", Color::off, ':' );
  CPositions<2>::OnExit();
  CPositions<3>::OnExit();
  CPositions<4>::OnExit();
  CPositions<5>::OnExit();
  clog( "Done." ); //*/
  
  clog_( "Simplex:", Color::bold, "onExit()", Color::off, ':' );
  Simplex::onExit();
  clog( "Done." );

  finish( "Cube framework", success );
  return success;
}

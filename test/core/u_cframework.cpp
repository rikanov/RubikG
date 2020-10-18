#include <test.h>
#include <cube_framework.h>

bool UnitTests::unit_CFramework() const
{
  bool success = true;
  head( "cube framework" );

  clog_( "Start Simplex instance..." );
  Simplex::instance();
  done();

  clog_( "Start 'cube positions' instances..." );
  CPositions<2>::Instance();
  CPositions<3>::Instance();
  CPositions<4>::Instance();
  CPositions<5>::Instance();
  done();

  clog_( "Cube framework test..." );
  CFramework<5> test1, test2;
  done();

  NL();
  NL();

  clog( Color::white, "Start testing turns" );
  clog(               "-------------------" );
  NL();
  tcase( "Test case 1", " X1, Y1" );
  test1.rotate( _X, 1 );
  test1.rotate( _Y, 1 );
  test1.print();
  success &= test1.integrity();
  
  tcase( "Test case 2 :", " Y1, Z4" );
  test2.rotate( _Y, 1);
  test2.rotate( _Z, 4);
  test2.print();
  success &= test2.integrity();
  
  tcase( "Test case 3", " combine of T1 + T2", "X1, Y1, Y1, Z4" );
  CFramework<5> test3( test1, test2 );
  test3.print();
  success &= test3.integrity();
  
  tcase( "Test case 4", " inverse of T3" );
  CFramework<5> test4 = test3.inverse();
  test4.print();
  success &= test4.integrity();
  
  tcase( "Test case 5", " revert of T4", "Z4, Z4, Z4, Y1, Y1, X1, X1, X1" );
  CFramework<5> test5;
  test5.rotate( _Z, 4 );
  test5.rotate( _Z, 4 );
  test5.rotate( _Z, 4 );
  test5.rotate( _Y, 1 );
  test5.rotate( _Y, 1 );
  test5.rotate( _X, 1 );
  test5.rotate( _X, 1 );
  test5.rotate( _X, 1 );
  test5.print();
  success &= test5.integrity();
  
  tcase( "Test case 6", " combine of T3 + T4", "solved cube" );
  CFramework<5> test6(test3,test4);
  test6.print();
  success &= test6.integrity();
  success &= test6 == CFramework<5>(); // solved
  
  tcase( "Test case 7", " combine of T3 + T5", "solved cube" );
  CFramework<5> test7(test3,test5);
  test7.print();
  success &= test7.integrity();
  success &= test7 == CFramework<5>(); // solved
  
  tcase( "Test case 8", "integrity test of 4 x 4 cubes");
  CFramework<4> test8A, test8B, test8C;
  const int num = 1000;
  int counter = 0;
  for ( int s = 0; s < num; ++s )
  {
    bool integrity = true;
    test8A.shuffle();
    test8C.shuffle();
    test8B = CFramework<4>::Transform( test8A, test8C );
    
    integrity &= test8A.integrity();
    integrity &= test8B.integrity();
    integrity &= test8C.integrity();
    integrity &= ( test8B == test8C - test8A ) && ( test8C == test8A + test8B );
    
    counter += integrity;
  }
  const bool s = ( counter == num );
  tail( std::to_string( counter ) + " out of " + std::to_string( num ) + " executed", s );
  success &= s;
  clog_( "Cube positions:", Color::bold, "onExit()", Color::off, ':' );
  CPositions<2>::OnExit();
  CPositions<3>::OnExit();
  CPositions<4>::OnExit();
  CPositions<5>::OnExit();
  done();
  
  clog_( "Simplex:", Color::bold, "onExit()", Color::off, ':' );
  Simplex::onExit();
  done();

  finish( "Cube framework", success );
  return success;
}

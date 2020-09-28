
#include <iostream>
#include <cstdlib>

#include "test.h"
#include "cli.h"
#include "rubiks_cube.h"

int main()
{
  UnitTests tests;
  tests.run();
  
  initCLi();
  
  clog("Start Simplex instance..." );
  Simplex::instance();
  clog("Done." );

  clog("Start cube positions instances..." );
  CPositions<2>::Instance();
  CPositions<3>::Instance();
  CPositions<4>::Instance();
  CPositions<5>::Instance();
  clog("Done." );

  clog("Cube framework test..." );
  CFramework<5> test1, test2;
  clog("Done." );
  
  //test1.print(_R, _U );
  test1.rot( _X, 1);
  //test1.print(_R, _U );
  test1.rot( _Y, 1);
  //test1.print(_R, _U );
  
  test2.rot( _Y, 1);
  test2.print();
  test2.rot( _Z, 4);
  test2.print();
  
  CFramework<5> test3( test1, test2 );
  //test3.print();
  
  CFramework<5> test4 = test3.inverse();
  //test4.print();
  
  CFramework<5> test5(test3,test4);
  test5.print();
  
  clog( "Cube positions:", Color::bold, "onExit()", Color::off );
  CPositions<2>::OnExit();
  CPositions<3>::OnExit();
  CPositions<4>::OnExit();
  CPositions<5>::OnExit();
  clog( "Done." ); //*/
  
  clog( "Simplex onExit()" );
  Simplex::onExit();
  clog( "Done." );
  
  clog( "Press", Color::white, Color::uline, "ENTER", Color::off,"to end." ); 
  std::cin.get();
  return 0;
}

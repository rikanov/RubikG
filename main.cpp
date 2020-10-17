
#include <iostream>
#include <cstdlib>

#include <test.h>
#include <cli.h>
#include <rubiks_cube.h>

void init()
{
  Simplex::instance();
  CPositions<2>::Instance();
  CPositions<3>::Instance();
  CPositions<4>::Instance();
  CPositions<5>::Instance();
  CFramework<2>::InitializeBase();
  CFramework<3>::InitializeBase();
  CFramework<4>::InitializeBase();
  CFramework<5>::InitializeBase();
  CLi::init();
  CLi::REPL();
}

void exit()
{
  CLi::exit();
  CPositions<2>::OnExit();
  CPositions<3>::OnExit();
  CPositions<4>::OnExit();
  CPositions<5>::OnExit();
  CFramework<2>::DeleteBase();
  CFramework<3>::DeleteBase();
  CFramework<4>::DeleteBase();
  CFramework<5>::DeleteBase();
  Simplex::onExit();
}
int main()
{
  init();
  std::atexit( exit );
  
  UnitTests tests;
  tests.testCore();
  
  return 0;
}

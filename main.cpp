#include "all_includes.h"
#include <engine.h>

int main()
{
  init();
  CNode<4>::Initialize( 5 );
  
  byte *path = new byte [5];
  
  for( int i = 0; i < 40; ++i)
  {
    clog( i, " -- > ", toString<4>( i ) ) ;
  }
  NL();
  
  for( int i = 0; i < 100; ++i)
  {
    Engine<4>::Path( CNode<4>::getNode( i )  , path );
    std::string out = toString<4>( path );
    slog( "\t", i, out, toString<4>( CNode<4>::getNode( i ) -> getPath() ) );
  }
  for( int i = 1; i < 5; ++i)
  {
    clog( toString<4>( CNode<4>::StartNode( i ) -> getPath() ) );
  }
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to end." ); 
  std::cin.get();
  std::atexit( exit );
  
  UnitTests tests;
  tests.testCore();
  
  CNode<4>::OnExit();
  return 0;
}

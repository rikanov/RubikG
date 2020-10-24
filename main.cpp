#include "all_includes.h"
#include <engine.h>

int main()
{
  init();
  CNode<4>::Initialize( 4 );
  
  byte *path = new byte [5];
  
  for( int i = 0; i < 40; ++i)
  {
    clog( i, " -- > ", toString<4>( i ) ) ;
  }
  NL();
  for ( Axis axis : { _X, _Y, _Z } )
  {
    for ( byte slice = 0; slice < 4; ++slice )
    {
      for ( byte turn: { 1, 2, 3 } )
      {
        const byte i = getRotID<4>( axis, slice,turn);
        clog ( (int) i , toString<4>( i ) );
      }
    }
  }
  for( int i = 0; i < 300; ++i)
  {
    Engine<4>::Path( CNode<4>::getNode( i )  , path );
    std::string out = toString<4>( path );
    clog( i, '.', out );
  }
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to end." ); 
  std::cin.get();
  std::atexit( exit );
  
  UnitTests tests;
  tests.testCore();
  
  CNode<4>::OnExit();
  return 0;
}

#include <test.h>
#include <insight.h>


bool UnitTests::unit_CState() const
{

  bool success = true;
  head( "CacheIDmapper" );

  tcase( "Memory allocation" );

  SubSpace toSolve = {
                 CPositions<4>::GetPosID( 1, 1, 0 ),
                 CPositions<4>::GetPosID( 1, 2, 0 ),
                 CPositions<4>::GetPosID( 2, 1, 0 ),
                 CPositions<4>::GetPosID( 2, 2, 0 ),
                 CPositions<4>::GetPosID( 2, 0, 0 )
                };
; 
  Insight<4> baseInsight( toSolve );
  Insight<4> transInsight( baseInsight, Simplex::Tilt( _X, 1 ) );

  Rubik<4> test4;
  baseInsight.set( test4 );
  transInsight.set( test4 );
/*
for( int i = 0; i < 20; ++i)
{
  RotID stack[20];
  for( int t = 0; t < 20; ++t)
  {
    stack[t] = CRotations<5>::Random();
    baseInsight.rotate( stack[t] );
  }
  for( int t = 0; t < 20; ++t)
  {
    baseInsight.rotate( CExtRotations<4>::Inverse( stack[ 19 -t ] ) );
  }
}*/
  clog( "\nbase test:\n----------\n");
  
  
  clog( baseInsight.rotate( _Z, 0, 2 ) );
  
  clog( baseInsight.rotate( _Z, 1, 2 ) );
  
  clog( baseInsight.rotate( _X, 1, 1 ) );
  
  clog( baseInsight.rotate( _Z, 0, 2 ) );
  
  clog( baseInsight.rotate( _X, 4, 2 ) );
  
  clog( baseInsight.rotate( _Y, 4, 1 ) );
  
  clog( baseInsight.rotate( _Y, 4, 2 ) );
  
  clog( baseInsight.rotate( _Z, 4, 1 ) );
  
  clog( baseInsight.rotate( _Y, 3, 1 ) );
  
  clog( baseInsight.rotate( _Y, 3, 3 ) );
  
  clog( baseInsight.rotate( _X, 4, 3 ) );
  
  clog( baseInsight.rotate( _X, 4, 1 ) );
  
  clog( baseInsight.rotate( _Z, 4, 3 ) );
  
  clog( baseInsight.rotate( _Y, 4, 1 ) );
  
  clog( baseInsight.rotate( _X, 4, 2 ) );
  
  clog( baseInsight.rotate( _Z, 0, 2 ) );
  
  clog( baseInsight.rotate( _X, 1, 3 ) );
  
  clog( baseInsight.rotate( _Z, 4, 2 ) );
  

  clog( "\ntransformed test:\n-----------------\n" );

  
  clog( transInsight.rotate( _Y, 1, 2 ) );
  
  clog( transInsight.rotate( _Y, 0, 2 ) );
  
  clog( transInsight.rotate( _Y, 0, 1 ) );
  
  clog( transInsight.rotate( _Y, 0, 1 ) );
  
  clog( transInsight.rotate( _X, 4, 1 ) );
  
  clog( transInsight.rotate( _Y, 4, 2 ) );
  
  clog( transInsight.rotate( _Y, 4, 2 ) );
  
  clog( transInsight.rotate( _X, 4, 3 ) );
  
  
  tail( "Memory allocation", success );

  finish( "CacheIDmapper", success );
  return success;
}

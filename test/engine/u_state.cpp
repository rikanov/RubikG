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
  Insight<4> testInsight( toSolve );

  Rubik<4> test4;
  testInsight.set( test4 );
  clog( testInsight.state() );

  testInsight.rotate( _X, 0, 2 );
  testInsight.rotate( _X, 1, 2 );
  testInsight.rotate( _Z, 5, 1 );
  clog( testInsight.state() );

  tail( "Memory allocation", success );

  finish( "CacheIDmapper", success );
  return success;
}

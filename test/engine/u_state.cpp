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

  clog( "\nbase test:\n----------\n");
  
  baseInsight.print();
  clog( baseInsight.rotate( _Z, 0, 2 ) );
  baseInsight.print();
  clog( baseInsight.rotate( _X, 1, 2 ) );
  baseInsight.print();
  clog( baseInsight.rotate( _Z, 0, 2 ) );
  baseInsight.print();
  clog( baseInsight.rotate( _X, 0, 2 ) );
  baseInsight.print();
  clog( baseInsight.rotate( _Y, 1, 2 ) );
  baseInsight.print();
  clog( baseInsight.rotate( _Y, 3, 2 ) );
  baseInsight.print();
  clog( baseInsight.rotate( _X, 2, 1 ) );
  baseInsight.print();

  clog( "\ntransformed test:\n-----------------\n" );

  transInsight.print();
  clog( transInsight.rotate( _Y, 1, 2 ) );
  transInsight.print();
  clog( transInsight.rotate( _Y, 0, 2 ) );
  transInsight.print();
  clog( transInsight.rotate( _Y, 0, 1 ) );
  transInsight.print();
  clog( transInsight.rotate( _Y, 0, 1 ) );
  transInsight.print();
  clog( transInsight.rotate( _X, 4, 1 ) );
  transInsight.print();
  clog( transInsight.rotate( _Y, 4, 2 ) );
  transInsight.print();
  clog( transInsight.rotate( _Y, 4, 2 ) );
  transInsight.print();
  clog( transInsight.rotate( _X, 4, 3 ) );
  transInsight.print();
  
  tail( "Memory allocation", success );

  finish( "CacheIDmapper", success );
  return success;
}

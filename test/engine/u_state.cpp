#include <test.h>
#include <cache_generator.h>


bool UnitTests::unit_CState() const
{

  bool success = true;
  head( "CacheIDmapper" );

  tcase( "Memory allocation" );
  CacheIDmapper<4> test;
  CacheIDmap   <4> result;
  SubSpace toSolve = {
                 CPositions<4>::GetPosID( 1, 1, 0 ),
                 CPositions<4>::GetPosID( 1, 2, 0 ),
                 CPositions<4>::GetPosID( 2, 1, 0 ),
                 CPositions<4>::GetPosID( 2, 2, 0 ),
                 CPositions<4>::GetPosID( 2, 0, 0 )
                };

  test.reset( toSolve );
  test.createMap( result ); 

  for( int i = 200000, counter = 0; i < 200500; ++i )
  {
    if ( result.distance( i ) > 0 )
      clog( counter++, '\t', i, ":", result.distance( i ), result.complexity( i ) );
  }
  tail( "Memory allocation", success );

  finish( "CacheIDmapper", success );
  return success;
}

#include <test.h>
#include <cache_generator.h>


bool UnitTests::unit_CState() const
{

  bool success = true;
  head( "CacheIDmapper" );

  tcase( "Memory allocation" );
  CacheIDmap   <4> testMap( 4 );
  CacheIDmapper<4> test   ( 4 );
  const PosID toSolve[] = {
                            CPositions<4>::GetPosID( 1, 1, 0 ),
                            CPositions<4>::GetPosID( 1, 2, 0 ),
                            CPositions<4>::GetPosID( 2, 1, 0 ),
                            CPositions<4>::GetPosID( 2, 2, 0 )
                          };
  test.initialize( toSolve );
  success |= test.generateMap( testMap );

  tail( "Memory allocation", success );

  finish( "CacheIDmapper", success );
  return success;
}

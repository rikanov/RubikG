#include <test.h>
#include <cache_generator.h>


bool UnitTests::unit_CState() const
{

  bool success = true;
  head( "CacheIDmapper" );

  tcase( "Memory allocation" );
  CacheIDmapper<4> test;
  CacheIDmap   <4> sliceResult;
  CacheIDmap   <3> layerResult;
  SubSpace toSolve = {
                 CPositions<4>::GetPosID( 1, 1, 0 ),
                 CPositions<4>::GetPosID( 1, 2, 0 ),
                 CPositions<4>::GetPosID( 2, 1, 0 ),
                 CPositions<4>::GetPosID( 2, 2, 0 ),
                 CPositions<4>::GetPosID( 2, 0, 0 )
                };

  test.reset( toSolve );
  test.createSliceMap( sliceResult );       
  test.reset( toSolve );
  test.createLayerMap( layerResult );

  for( int i = 100000, counter = 0; i < 200500; ++i )
  {
    if ( sliceResult.distance( i ) < layerResult.distance( i ) )
      clog( counter++, '\t', i, "\t:", sliceResult.distance( i ), sliceResult.complexity( i ), "\t|", layerResult.distance( i ), layerResult.complexity( i ) );
  }
  tail( "Memory allocation", success );

  finish( "CacheIDmapper", success );
  return success;
}

#include "test.h"
#include "simplex.h"

bool UnitTests::unit_Simplex() const
{
  bool result = true;
  
  head( "Simplex" );
  clog( Color::gray, "Start Simplex instance..." );
  Simplex::instance();
  clog("Done." );
  NL();
  
  // test cases
  Facet testCases[10][3][2]  = 
                              {/*       A           B      expected AxB  */
                                 { { _R, _U }, { _R, _U }, { _R, _U } }
                                ,{ { _R, _U }, { _B, _D }, { _B, _D } }
                                ,{ { _F, _U }, { _R, _U }, { _F, _U } }
                                ,{ { _L, _D }, { _L, _D }, { _R, _U } }
                                ,{ { _B, _R }, { _L, _U }, { _F, _R } }
                                ,{ { _D, _F }, { _L, _F }, { _U, _L } }
                                ,{ { _L, _B }, { _L, _B }, { _R, _U } }
                                ,{ { _U, _L }, { _U, _B }, { _L, _B } }
                                ,{ { _F, _D }, { _D, _L }, { _U, _B } }
                                ,{ { _D, _F }, { _B, _L }, { _R, _U } }
                              };
  
  for( auto c: testCases )
  {
    const Facet rA = c[0][0], uA = c[0][1];
    const Facet rB = c[1][0], uB = c[1][1];
    const Facet rC = c[2][0], uC = c[2][1];
    CubeID A = Simplex::GetGroupID( rA , uA );
    CubeID B = Simplex::GetGroupID( rB , uB );
    CubeID C = Simplex::GetGroupID( rC , uC );
    CubeID T = Simplex::Composition( A , B );
    clog_( Color::blue, Simplex::GetCube( A ), 'X', Simplex::GetCube( B ), '=', Simplex::GetCube( C ), '\t' );
    if ( C == T )
    {
      clog( Color::green, "OK" );
    }
    else
    {
      clog( Color::red, "NO OK!" );
      result = false;
    }
  }
  
  NL();
  clog( Color::gray, Color::uline, "Simplex exit." );
  Simplex::onExit();
  clog("Done." );
  tail( "Simplex", result );
  return result;
}

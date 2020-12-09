#include <test.h>
#include <cube_rotations.h>

bool UnitTests::unit_CRotations() const
{
  bool success = true;
  head( "cube rotation" );

  clog_( "Start 'cube rotations' instances..." );
  CRotations<2>::Instance();
  CRotations<3>::Instance();
  CRotations<4>::Instance();
  CRotations<5>::Instance();
  done();

  tcase( "RotID conversion" );
  all_rot( axis, layer, turn, 5 )
  {
    const RotID rotID = CRotations<5>::GetRotID( axis, layer, turn );
    clog_( Color::gray, (int) rotID, Color::white, "-->\t", Color::bold, CRotations<5>::ToString( rotID ) );
    bool s = ( axis == CRotations<5>::GetAxis( rotID ) ) && ( layer == CRotations<5>::GetLayer( rotID ) ) && ( turn == CRotations<5>::GetTurn( rotID ) );
    stamp( s, success );
  }
  tail( "RotID conversion", success );

  clog_( "Cube rotations:", Color::bold, "onExit()", Color::off, ':' );
  CRotations<2>::OnExit();
  CRotations<3>::OnExit();
  CRotations<4>::OnExit();
  CRotations<5>::OnExit();
  done();
  
  finish( "Cube rotations", success );
  return success;
}
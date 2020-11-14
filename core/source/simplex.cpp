#include <simplex.h>

Simplex * Simplex::Singleton = nullptr;

Simplex::Simplex()
{
  // Constructor body
  Singleton = this;
  init();
  initGroup();
}

void Simplex::init()
{
  // determine group IDs. Identity element most have ID = 0
  CubeID lastRadix[6] = {};
  all_facet ( right )
  {
    all_facet ( up )
    {
      if ( Coaxial(right, up) )
      {
        simplexGroupID [ right ][ up ] = _NF;
        continue;
      }
      CubeID front = OCube::FrontSide( right, up );
      const int index = 4 * front + lastRadix [ front ]++;
      simplexGroupID [ right ][ up ] = index;
      simplexGroup [ index ].init ( right, up, index );
       // created all the 24 orientation of a simplex and store them in an array
      //  indexed by their group ID
    }
  }
}

void Simplex::initGroup()
{
  // init algebrian group
  all_id ( a )
    all_id ( b )
    {
      OCube A = GetCube ( a );
      OCube B = GetCube ( b );

      // create a composition of them:
      Facet cr = A.whatIs ( B.whatIs ( _R ) );
      Facet cu = A.whatIs ( B.whatIs ( _U ) );

      const CubeID c = GetGroupID ( cr, cu );

      composition [a][b] = c;
      transform   [a][c] = b;
    }

  // init tilts  
  tilt [ _X ] [1] = Simplex::GetGroupID( _R, _F );
  tilt [ _Y ] [1] = Simplex::GetGroupID( _B, _U );
  tilt [ _Z ] [1] = Simplex::GetGroupID( _U, _L );
  for ( Axis a: { _X, _Y, _Z } )
  {
    for ( int t: { 2, 3 } )
    {
      tilt [a][t] = Simplex::Composition( tilt [a][t-1], tilt [a][1] );
    }
  }
  all_id( id )
  {
    for( Facet F: { _F, _R, _U, _L, _D, _B } )
      align[ id ][F] = GetCube( id ).aligned( F ); 
  }
}

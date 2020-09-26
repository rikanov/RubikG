#ifndef AUXILIARY__H
#define AUXILIARY__H

#include "global.h"

 // Auxiliary types
// ----------------
typedef unsigned char CubeID;

enum Facet
{
  _F, _R, _U, _D, _L, _B, _NF
};

enum Axis 
{
  _X, _Y, _Z, _NA
};

 // Auxiliary inline functions
// ---------------------------
inline void operator++ ( Facet& id )
{
  id = Facet( int( id ) + 1 );
}

inline Facet SideOpposite( Facet S )
{
  return (S == _NF) ? _NF : Facet( 5 - S );
}

 // Auxiliary functions
// --------------------
Color::Modifier colorOf( Facet F );


 // Auxiliary macros
// -----------------
#define all_facet(i) for(Facet i = _F; i <= _B; ++i)
#define  all_id(i) for(CubeID i = 0; i < 24; ++i)
///--------------------------


#endif

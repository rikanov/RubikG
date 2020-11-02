/*
 * Simple, typed IDs of the Rubik's Cubes. Axis-Layer-Turn (A-L-T) to describe layer rotations. CubeID, PosID, RotID to store spatial operations 
 *
 * Copyright (C) 2020  Robert Ikanov <robert.ikanov@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BASE_TYPES__HEADER
#define BASE_TYPES__HEADER

#include <def_colors.h>

typedef unsigned char byte;

 // ID types
//  --------

typedef unsigned char CubeID;
typedef unsigned char PosID;
typedef unsigned char RotID;

 // Facets
//  ------

enum Facet
{
  _F, _R, _U, _D, _L, _B, _NF
};

 // Components
//  ----------

enum Axis 
{
  _X, _Y, _Z, _NA
};

typedef char Layer;
typedef char Turn;


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

inline Color::Modifier colorOf( Facet F )
{
  switch ( F )
  {
    case _F:
      return Color::red;
      
    case _R:
      return Color::yellow;
      
    case _U:
      return Color::blue;
      
    case _L:
      return Color::white;
      
    case _D:
      return Color::green;
      
    case _B:
      return Color::light;
      
    default:
      return Color::black;
  }
}



 // Auxiliary macros
// -----------------
#define all_facet(i) for(Facet i = _F; i <= _B; ++i)
#define  all_id(i) for(CubeID i = 0; i < 24; ++i)

#endif
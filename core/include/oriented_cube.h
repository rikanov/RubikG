/*
 * Oriented cube: desribe the rotational state of a (single) cube.
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

#ifndef ORIENTED_CUBE_HEADER
#define ORIENTED_CUBE_HEADER

#include <base_types.h>

// Oriented cube class, an auxiliary class for the sake of readable code. 
// It won't be used directly by algorithms but its mapping function is very useful for UI too.
// The basic idea: there are 24 different cube orientation, forming a non-Abelian group
// which is isomorph to P{4} permutation group. Mapping to integer: query the Right and Up
// facets current values. ( 6 possible 'Right' facets each of them has 4 possible 'Up' facets )
// Their properties and group operation stored and handled in a singleton Simplex class

class OCube
{
  static const char  Tokens [7];
  static const Facet FrontSides [ 6 /*Right*/ ][ 6 /*Up*/ ];

  Facet  m_whereIs [7];
  Facet  m_whatIs  [7];
  bool   m_aligned [7];
  CubeID m_groupID;

  std::string m_readable;

public:
  OCube( ) = default;
  void init ( Facet R, Facet U, CubeID groupID );

  Facet   whereIs ( Facet ID ) const { return m_whereIs[ID]; }
  Facet	  whatIs  ( Facet ID ) const { return m_whatIs[ID];  }
  bool    aligned ( Facet ID ) const { return m_aligned[ID]; }
  CubeID  groupID (   void   ) const { return m_groupID;     }

  const std::string& toString () const { return m_readable;         }
  operator        const char* () const { return m_readable.c_str(); }
  
  static char  Token     ( Facet F )          { return Tokens[ F ];      }
  static Facet FrontSide ( Facet r, Facet u ) { return FrontSides[r][u]; }
};

#endif // !ORIENTED_CUBE_HEADER

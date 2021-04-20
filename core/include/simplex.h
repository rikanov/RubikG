/*
 * Simplex: to handle (a single) cube rotation group and mapping them to integer (CubeID) values to make algorithms efficient
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

#ifndef SIMPLEX_HEADER
#define SIMPLEX_HEADER

#include <oriented_cube.h>

class Simplex
{
    static Simplex * Singleton;
  
private: 
  // functions
  Simplex();
  void init();
  void initGroup();

private: 
  // array datamembers
  OCube   m_simplexGroup     [ 24 /* ID */ ];
  CubeID  m_tilt             [ 3 ] [ 4 ] = {};
  CubeID  m_simplexGroupID   [ 6 /*Right*/ ][ 6 /*Up*/ ];
  CubeID  m_composition      [ 24 /* A */  ][ 24 /*B*/ ]; // X = AB
  CubeID  m_transform        [ 24 /* A */  ][ 24 /*B*/ ]; // AX = B => X = inv(A) B
  bool    m_align            [ 24 /* ID */ ][ 6  /*F*/ ]; // [ id ] [ Orient ]
  
  const OCube & getCube     ( CubeID ID )           const  { return m_simplexGroup [ID];                      }
  const OCube & getCube     ( Orient r, Orient u )  const  { return m_simplexGroup[ m_simplexGroupID[r][u] ]; }
  bool          aligned     ( CubeID a, Orient f )  const  { return m_align          [a][f];                  }
  CubeID        getGroupID  ( Orient r, Orient u )  const  { return m_simplexGroupID [r][u];                  }
  CubeID        composition ( CubeID a, CubeID b )  const  { return m_composition    [a][b];                  }
  CubeID        transform   ( CubeID a, CubeID b )  const  { return m_transform      [a][b];                  }
  CubeID        inverse     ( CubeID a )            const  { return m_transform      [a][0];                  } 
  CubeID        tilt        ( Axis a, Turn t = 1 )  const  { return m_tilt           [a][t];                  }

public:
  // create a singleton object
  static void            Instance    ( )                          { if ( Singleton == nullptr ) new Simplex;  }
  static void            OnExit      ( )                          { delete Singleton; Singleton = nullptr;    }
  static const OCube &   GetCube     ( CubeID ID )                { return Singleton -> getCube     ( ID );   }
  static const OCube &   GetCube     ( Orient r, Orient u )       { return Singleton -> getCube     ( r, u ); }
  static CubeID          GetGroupID  ( Orient r, Orient u )       { return Singleton -> getGroupID  ( r, u ); }
  static bool            Aligned     ( CubeID a, Orient f )       { return Singleton -> aligned     ( a, f ); }
  static CubeID          Composition ( CubeID a, CubeID b )       { return Singleton -> composition ( a, b ); }
  static CubeID          Transform   ( CubeID a, CubeID b )       { return Singleton -> transform   ( a, b ); }
  static CubeID          Inverse     ( CubeID a )                 { return Singleton -> transform   ( a, 0 ); } 
  static CubeID          Tilt        ( Axis a, Turn t = 1 )       { return Singleton -> tilt        ( a, t ); }
  static CubeID          Conjugation ( CubeID a, CubeID b )       { return Composition( a, Composition( b, Inverse(a) ) ); }
  
  // ToDo: use cached array for Tilt( CubeID, Axis, Turn )
  static CubeID          Tilt        ( CubeID c, Axis a, Turn t = 1 )   { return Singleton->Composition( c, Singleton -> tilt( a, t ) ); }

};

#endif //SIMPLEX_HEADER

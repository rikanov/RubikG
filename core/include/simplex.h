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
  
  CubeID tilt [ 3 ] [ 4 ] = {};

private: // functions
  Simplex();
  void init();
  void initGroup();
  void initMetric();

private: // array datamembers
  OCube                  simplexGroup     [ 24 /* ID */ ];
  CubeID                 simplexGroupID   [ 6 /*Right*/ ][ 6 /*Up*/ ];
  CubeID /* X: [0 23] */ composition      [ 24 /* A */  ][ 24 /*B*/ ]; // X = AB
  CubeID /* X: [0 23] */ transform        [ 24 /* A */  ][ 24 /*B*/ ]; // AX = B => X = inv(A) B
  bool                   align            [ 24 /* ID */ ][ 6  /*F*/ ]; // [ id ] [ facet ]
  byte                   metric           [ 24 /* A */  ][ 24 /*B*/ ];
  
public:

  // create a singleton object
  static void          Instance    ( )                          { if ( Singleton == nullptr ) new Simplex;   }
  static void          OnExit      ( )                          { delete Singleton; Singleton = nullptr;     }
  static const OCube & GetCube     ( CubeID ID )                { return Singleton -> simplexGroup [ID];     }
  static const OCube & GetCube     ( Facet r, Facet u )         { return GetCube( GetGroupID( r, u ) );      }
  static CubeID        GetGroupID  ( Facet r, Facet u )         { return Singleton -> simplexGroupID [r][u]; }
  static bool          Aligned     ( CubeID a, Facet f )        { return Singleton -> align[ a ][ f ];       }
  static CubeID        Composition ( CubeID a, CubeID b )       { return Singleton -> composition [a][b];    }
  static CubeID        Transform   ( CubeID a, CubeID b )       { return Singleton -> transform [a][b];      }
  static CubeID        Inverse     ( CubeID a )                 { return Singleton -> transform [a][0];      } 
  static CubeID        Tilt        ( Axis a, int t = 1 )        { return Singleton -> tilt [a][t];           }
  
  static bool          Coaxial     ( Facet a, Facet b )         { return  a == b || a == SideOpposite( b ) || a == _NF || b == _NF; }

};

#endif //SIMPLEX_HEADER

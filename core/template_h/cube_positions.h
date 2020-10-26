/*
 * Cube positions: this singleton class makes a graph to determine cublet positions in an N x N Rubics cube according to the rotation state of the cubes so each node has 24 branches
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

#ifndef CUBE_POSITIONS_H
#define CUBE_POSITIONS_H

#include <text_output.h>
#include <simplex.h>

static const bool SHOW_LOG = false; // setting true is only for debuging purposes

#define for_vector( x,y,z, N )           \
  for ( unsigned int x = 0; x < N; ++x )  \
    for ( unsigned int y = 0; y < N; ++y ) \
       for ( unsigned int z = 0; z < N; ++z )


struct Coord 
{
  unsigned int x = 0;
  unsigned int y = 0;
  unsigned int z = 0; 
  std::string toString() const 
  { 
    return "< " + std::to_string( x ) + ' ' + std::to_string( y ) + ' ' + std::to_string( z ) + " >";
  }
};

/// ----------------------------------- Template declarations starts here ------------------------------------- ///
template<unsigned int N>
class CPositions
{
  static constexpr unsigned int FrameworkSize [] = { 0, 1, 8 - 0, 27 - 1, 64 - 8, 125 - 27, 216 - 64, 341 - 125, 512 - 216, 721 - 341, 1000 - 512 }; //  N > 1 :  N ^ 3 - ( N - 2 ) ^ 3
  static CPositions * Singleton; 
  
  int   routerPositions [ FrameworkSize [ N ] ][ 24 ];
  int   frameworkSlice  [ 3 ] [ N ] [ N * N ]; // [axis] [slice index] [cubes] 
  int   coordToIndex    [ N ] [ N ] [ N ];
  Coord indexToCoord    [ FrameworkSize [ N ] ];

  CPositions();
  
  Coord rotate ( int x, int y, int z, CubeID rot ); // an auxiliary function for inner calculations
  Coord rotate ( const Coord C, CubeID rot )        { return rotate( C.x, C.y, C.z, rot ); }
  
  void  initPositions();
  void  initIndices  ();
  
  static int* GetNode  ( int id )                { return Singleton->routerPositions[ id ]; }
  static int* GetNode  ( int x, int y, int z )   { return GetNode( GetIndex ( x, y, z) );   }

public:
  static constexpr int GetSize ( ) { return Singleton->FrameworkSize [ N ]; }
  
  static void  Instance ( )                     { if ( Singleton == nullptr ) new CPositions<N>;  }
  static void  OnExit   ( )                     { delete Singleton; Singleton = nullptr;          }
  static bool  ValID    ( PosID id )            { return 0 <= id && id < GetSize();               }
  static Coord GetCoord ( PosID id )            { return Singleton->indexToCoord [ id ];          }
  static PosID GetIndex ( PosID id, CubeID rot) { return Singleton->routerPositions[ id ][ rot ]; }
  static PosID GetIndex ( int x, int y, int z ) { return Singleton->coordToIndex[ x ][ y ][ z ];  }
  static PosID GetIndex ( const Coord & C )     { return GetIndex( C.x, C.y, C.z);                }
  
  static PosID GetIndex ( int x, int y, int z, CubeID rot ) { return GetNode( x, y, z ) [ rot ];                  }
  static int   GetLayer ( Axis a, Layer sl, Turn id )       { return Singleton->frameworkSlice [ a ][ sl ][ id ]; }

};

/// ----------------------------------- Template definitions starts here ------------------------------------- ///
template<unsigned int N>
CPositions<N> * CPositions<N>::Singleton = nullptr;

template<unsigned int N>
CPositions<N>:: CPositions( )
{
  Singleton = this;
  initPositions();
}

template<unsigned int N> 
Coord CPositions<N>::rotate(int x, int y, int z, CubeID id)
{
  Coord result;
  OCube rot = Simplex::GetCube( id );
  unsigned int coord[6];
  coord[ _F ] = N - 1 - z;
  coord[ _R ] = N - 1 - x;
  coord[ _U ] = N - 1 - y;
  coord[ _B ] = z;
  coord[ _L ] = x;
  coord[ _D ] = y;

  result.x = coord[ rot.whatIs ( _L ) ];
  result.y = coord[ rot.whatIs ( _D ) ];
  result.z = coord[ rot.whatIs ( _B ) ];
  return result;
}

template<unsigned int N>
void CPositions<N>::initIndices()
{
  int index = 0,
  idX [N] = {}, idY [N] = {}, idZ [N] = {}; // slice indices
  for_vector ( x, y, z, N )
  {
    if ( x == 0 || x == N - 1 || y == 0 || y == N - 1 || z == 0 || z == N - 1 )
    {
      frameworkSlice [ _X ][ x ][ idX[x]++ ] = index;
      frameworkSlice [ _Y ][ y ][ idY[y]++ ] = index;
      frameworkSlice [ _Z ][ z ][ idZ[z]++ ] = index;
      
      indexToCoord [index] = { x, y, z };
      coordToIndex [ x ][ y ][ z ] = index++;
    }
    else
    {
      coordToIndex[ x ][ y ][ z ] = -1;
    }
  } // getIndex() is fully working now
}

template<unsigned int N>
void CPositions<N>::initPositions()
{
  initIndices();
  for_vector ( x, y, z, N )
  {
    if ( GetIndex( x, y, z ) != -1 )
    {
      if ( SHOW_LOG ) clog ( '\n', N, 'X', N,"  ... ", x,  y, z, "\t  ",  GetIndex( x, y, z) );
      all_id ( id )
      {
        const Coord C = rotate ( x, y, z, id );
        if ( SHOW_LOG ) clog ( Simplex::GetCube ( id ).toString(), " --> ", C.toString() , "\t| ",GetIndex( C ) );
        routerPositions[ GetIndex( x, y, z ) ][ id ] = GetIndex( C );
      }
    }
  }
}
#endif // CUBE_POSITIONS_H

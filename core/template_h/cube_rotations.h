/*
 * In brief: simple auxiliary template functions to handle basic rotation IDs in order to reach lower memory usage
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

#ifndef CUBE_ROTATIONS__H
#define CUBE_ROTATIONS__H

#include <oriented_cube.h>
#include <ctime>
#include <random>

// Mapping between all the basic rotations and the set of one-byte size IDs:
// Axis  --> X,Y or Z represented by integers [ 0..2 ]
// LAyer --> index of the turning slice [ 0..N ) where N isthe size of the cube
// Turn  --> 1: a simple clockwise turn  2: double turn 3: tripple (or inverse) turn
//
// to get ID we use a mixed radix system:
// ID = 0 --> no rotation
// ID = 3 * N * Axis + 3 * Layer + Turn 
// ID < 3 * N * 3

 // RotID from components
//  ---------------------


template<unsigned int N>
class CRotations
{
  static CRotations<N> * Singleton;
  static constexpr int AllRotIDs = 3 * N * 3 + 1;

  RotID  m_rotID [ 3 /*axes*/ ][ N /*layers*/ ][ 4 /*turns*/ ] = {};
  Axis   m_axis  [ AllRotIDs ] = {};
  Layer  m_layer [ AllRotIDs ] = {};
  Turn   m_turn  [ AllRotIDs ] = {};
  
  CRotations() = default;
  void init();

  public:

  static void Instance();
  static void OnExit();

  static  RotID   GetRotID ( Axis A, Layer L, Turn T ) { return Singleton -> m_rotID [A][L][T]; }
  static  Axis    GetAxis  ( RotID rotID )             { return Singleton -> m_axis  [rotID];   }
  static  Layer   GetLayer ( RotID rotID )             { return Singleton -> m_layer [rotID];   }
  static  Turn    GetTurn  ( RotID rotID )             { return Singleton -> m_turn  [rotID];   }

  static RotID random();
  static std::string ToString( Axis  );
  static std::string ToString( RotID );
};
template<unsigned int N>
CRotations<N> * CRotations<N>::Singleton = nullptr;

template<unsigned int N>
void CRotations<N>::Instance()
{
  if ( nullptr == Singleton )
  {
    Singleton = new CRotations<N>();
    Singleton -> init();
  }
}

template<unsigned int N>
void CRotations<N>::OnExit()
{
  delete Singleton;
  Singleton = nullptr;
}

template<unsigned int N>
void CRotations<N>::init()
{
  RotID rotID = 0;
  all_rot( axis, layer, turn, N )
  {
    m_rotID [axis][layer][turn] = ++ rotID;
    m_axis  [rotID] = axis;
    m_layer [rotID] = layer;
    m_turn  [rotID] = turn;
  }
}

template<unsigned int N>
RotID CRotations<N>::random()
{
  static std::default_random_engine engine( static_cast<unsigned int>( time( 0 ) ) );
  static std::uniform_int_distribution<int> dist( 1, (int) 3 * N * 3 ); 
  return dist( engine );
}

template<unsigned int N>
std::string CRotations<N>::ToString( Axis A )
{
  switch ( A )
  {
    case _X:
      return "X";
    case _Y:
      return "Y";
    case _Z:
      return "Z";
    default:
      return "#ERR";
  }
}

template<unsigned int N>
std::string CRotations<N>::ToString( RotID rotID )  
{
  return std::string( "{ _" + ToString ( GetAxis ( rotID ) ) + ", " + std::to_string( GetLayer ( rotID ) ) + ", " + std::to_string( GetTurn ( rotID ) ) + " }" );
}
#endif // ! CUBE_ROTATIONS__H
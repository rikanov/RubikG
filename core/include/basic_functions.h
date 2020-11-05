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

#ifndef BASIC_ROTATIONS__H
#define BASIC_ROTATIONS__H

#include <base_types.h>
#include <ctime>
#include <random>

// Mapping between all the basic rotations and the set of one-byte size IDs:
// Axis  --> X,Y or Z represented by integers [ 0..2 ]
// LAyer --> index of the turning slice [ 0..N ) where N isthe size of the cube
// Turn  --> 1: a simple clockwise turn  2: double turn 3: tripple (or inverse) turn
//
// to get ID we use a mixed radix system:
// ID = 3 * N * Axis + 3 * Layer + Turn 
// ID < 3 * N * 3

 // RotID from components
//  ---------------------

template<unsigned int N>
inline RotID getRotID( Axis A, Layer L, Turn T )
{
  return ( 3 * N * A ) + ( 3 * L ) + T;
}

 // Get components from RotID
//  -------------------------

template<unsigned int N>
inline Axis getAxis( RotID id )
{
  switch ( --id / ( 3 * N ) )
  {
    case 0:
      return _X;
    case 1:
      return _Y;
    case 2:
      return _Z;
    default:
      return _NA;
  }
}

template<unsigned int N>
inline Layer getLayer( RotID id )
{
  return ( --id % ( 3 * N ) ) / 3;
}

template<unsigned int N>
inline Turn getTurn( RotID id )
{
  return --id % 3 + 1;
}

 // Random RotID
//  ------------
 
template<unsigned int N>
RotID randomRotID()
{
  static std::default_random_engine engine( static_cast<unsigned int>( time( 0 ) ) );
  static std::uniform_int_distribution<int> dist( 1, (int) 3 * N * 3 ); 
  return dist( engine );
}
 
 // Readable output 
//  ---------------

template<unsigned int N>
std::string toString( Axis A )
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
std::string toString( RotID B )  
{
  return std::string( "{ _" + toString<N> ( getAxis<N> (B) ) + ", " + std::to_string( getLayer<N> (B) ) + ", " + std::to_string( getTurn<N> (B) ) + " }" );
}

template<unsigned int N>
std::string toString( const RotID* B )  
{
  std::string ret;
  while ( *B != 0 )
  {
    ret += toString<N> ( *( B++ ) );
  }
  return ret;
}
#endif

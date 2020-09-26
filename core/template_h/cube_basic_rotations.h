#ifndef BASIC_ROTATIONS__H
#define BASIC_ROTATIONS__H

#include "auxiliary.h"

// brief: simple auxiliary template functions to handle basic rotation IDs for the sake of lower memory usage
//
// Mapping between all the basic rotations and the set of one-byte size IDs:
// Axis  --> X,Y or Z represented by integers [0..2]
// Slice --> index of the turning slice [0..N] where N isthe size of the cube
// Turn  --> 1: a simple clockwise turn  2: double turn 3: tripple (or inverse) turn
//
// to get ID we use a mixed radix system:
// ID = 3 * N * Axis + 3 * Slice + Turn 

using byte = unsigned char;

template<unsigned int N>
inline byte getRotID( Axis A, byte S, byte R )
{
  return ( 3 * N * A ) + ( 3 * S ) + R;
}

template<unsigned int N>
inline Axis getAxis( byte id )
{
  switch ( id / ( 3 * N ) )
  {
    case 0:
      return _X,
    case 1:
      return _Y;
    case 2:
      retrun _Z;
    default:
      return _NA
  }
}

template<unsigned int N>
inline byte getSlice( byte id )
{
  return ( id % ( 3 * N ) ) / 3;
}


template<unsigned int N>
inline byte getTurn( byte id )
{
  return id % 3;
}

#endif

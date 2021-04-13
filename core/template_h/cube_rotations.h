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

#include <simplex.h>
#include <random>

// Mapping between all the basic rotations and the set of one-byte size IDs:
// Axis  --> X,Y or Z represented by integers [ 0..2 ]
// Layer --> index of a single turning layer [ 0..N ) or the last layer of a slice [ 0..N-1 )
// Turn  --> 1: a simple clockwise turn  2: double turn 3: tripple (or inverse) turn
//
// to get ID we use a mixed radix system:
// ID = 0 --> no rotation
// ID = 3 * N * Axis + 3 * Layer + Turn 
// ID < 3 * N * 3

static inline Orient GetBaseOrient( Axis axis )
{
  switch( axis )
  {
    case _X:
      return _L;
    case _Y:
      return _D;
    case _Z:
      return _B;
    default:
      return _NF;
  }
}

template<unsigned int N>
class CRotations
{

public:
  static constexpr int AllRotIDs = 3 * N * 3 + 1;

private:
  static CRotations<N> * Singleton;

  std::random_device                 m_randomDevice;
  std::default_random_engine         m_randomEngine;
  std::uniform_int_distribution<int> m_distribution; 

  RotID  m_rotID  [ 3 /*axes*/][ N /*layers*/ ][ 4 /*turns*/ ] = {};
  RotID  m_tRotID [ AllRotIDs ][ 24 ] = {};
  Axis   m_axis   [ AllRotIDs ]       = {};
  Layer  m_layer  [ AllRotIDs ]       = {};
  Turn   m_turn   [ AllRotIDs ]       = {};
  
  CRotations()
    : m_randomEngine( m_randomDevice() )
    , m_distribution( 1, (int) 3 * N * 3 )
  {}

  void  init();
  void  transformRotIDs();
  RotID transformedRotID( Orient trans, Layer layer, Turn turn );

  RotID   getRotID ( Axis A, Layer L, Turn T ) const  { return m_rotID [A][L][T]; }

  Axis    getAxis  ( RotID rotID ) const  { return m_axis  [ rotID ]; }
  Layer   getLayer ( RotID rotID ) const  { return m_layer [ rotID ]; }
  Turn    getTurn  ( RotID rotID ) const  { return m_turn  [ rotID ]; }
  
  public:

  static void Instance();
  static void OnExit();

  static RotID GetInvRotID ( Axis A, Layer L, Turn T ) { return Singleton -> getRotID( A, L, 4 - T ); }
  static RotID GetRotID    ( Axis A, Layer L, Turn T ) { return Singleton -> getRotID( A, L, T );     }

  static Axis  GetAxis  ( RotID rotID )  { return Singleton -> getAxis  ( rotID ); }
  static Layer GetLayer ( RotID rotID )  { return Singleton -> getLayer ( rotID ); }
  static Turn  GetTurn  ( RotID rotID )  { return Singleton -> getTurn  ( rotID ); }

  static RotID Random();
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
    transformRotIDs();
  }
}

template<unsigned int N>
RotID CRotations<N>::transformedRotID( Orient trans, Layer layer, Turn turn )
{
  switch( trans )
  {
    case _L:
      return GetRotID( _X, layer, turn );
    case _D:
      return GetRotID( _Y, layer, turn );
    case _B:
      return GetRotID( _Z, layer, turn );
    case _R:
      return GetRotID( _X, N - 1 - layer, 4 - turn );
    case _U:
      return GetRotID( _Y, N - 1 - layer, 4 - turn );
    case _F:
      return GetRotID( _Z, N - 1 - layer, 4 - turn );
    default:
      clog( "invalid Orient to get axis" );
      return 0;
  }
}
template<unsigned int N>
void CRotations<N>::transformRotIDs()
{
  all_rot( axis, layer, turn, N )
  {
    const RotID rotID = GetRotID( axis, layer, turn);
    all_cubeid( cubeID )
    {
      Orient base  = GetBaseOrient( axis );
      Orient trans = Simplex::GetCube( cubeID ).whereIs( base );
      m_tRotID[ rotID ][ cubeID ] = transformedRotID( trans, layer, turn );
    }
  }
}

template<unsigned int N>
RotID CRotations<N>::Random()
{
  return CRotations<N>::Singleton -> m_distribution( CRotations<N>::Singleton -> m_randomEngine );
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

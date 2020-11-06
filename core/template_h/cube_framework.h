/*
 * Cube frameworks: store only the rotational state of all cublets of an N x N Rubics cube. (without their position in the Rubics cube)
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

#ifndef CUBE_FRAMEWORK_HEADER
#define CUBE_FRAMEWORK_HEADER

#include <basic_functions.h>
#include <cube_positions.h>
#include <cstring> // memcmp, memcpy

/// ----------------------------------- Template declarations starts here ------------------------------------- ///
template<unsigned int N>
class CFramework
{
  static constexpr size_t Fsize = CPositions<N>::GetSize();
  CubeID * frameworkSpace;

public:
  
  static void InitializeBase();
  static void DeleteBase();

  // Constructors
  CFramework( void );
  CFramework( const CFramework<N>&, const CFramework<N>& );
  CFramework( const CFramework<N>&, RotID );
  CFramework( const CFramework<N>& );
  CFramework( CFramework<N>&& f );
  
  // Operations
  CFramework<N> inverse    ( void ) const;
  void          rotate     ( const Axis, const Layer, const Turn turn = 1 );
  void          rotate     ( const RotID rotID );
  void          shuffle    ( int depth = 0 );
  
  static CFramework<N> Transform  ( const CFramework<N>& A, const CFramework<N>& C ) { return CFramework<N>( A.inverse(), C ); } // transform( A, C ) returns with B where A + B = C
  
  // Operators
  constexpr CFramework<N>& operator=  ( const CFramework<N>& C ) { std::memcpy( frameworkSpace, C.frameworkSpace, Fsize ); return *this; }
  bool                     operator== ( const CFramework<N>& X ) { return std::memcmp( frameworkSpace, X.frameworkSpace, Fsize ) == 0;   }
  const     CFramework<N>  operator+  ( const CFramework<N>& B ) { return CFramework<N> ( *this, B ); }
  const     CFramework<N>  operator-  ( const CFramework<N>& B ) { return Transform( B, *this );      }
  // Destructor
  ~CFramework( );
  
  // Query functions
  CubeID       getCubeID ( PosID id ) const { return frameworkSpace[id];                   }
  OCube        getCube   ( PosID id ) const { return Simplex::GetCube( getCubeID ( id ) ); }
  inline PosID whatIs    ( PosID id ) const ;
  inline PosID whereIs   ( PosID id ) const ;
  Coord        whatIs    ( Coord C )  const { return CPositions<N>::getCoord( whatIs ( CPositions<N>::getIndex( C ) ) ); }
  Coord        whereIs   ( Coord C )  const { return CPositions<N>::getCoord( whereIs( CPositions<N>::getIndex( C ) ) ); }
  bool         integrity ( void    )  const ;
  Facet        getFacet  ( const Facet front, const Facet up, int x, int y) const; // left-bottom corner: x = 0, y = 0
  
  // Printer
  void show ( Facet                  ) const;
  void show ( Facet, Facet, int, int ) const;
  void print( Facet, Facet           ) const;
  void print( bool separator = true  ) const;
};

// print a single facet on different operating systems
#ifdef _WIN32
  static const char FChar[] = { (char) 254u, '\0' };
#else
  static const char FChar[] = "■";
#endif

/// ----------------------------------- Template definitions starts here ------------------------------------- ///

 // Constructors
//  ------------

// default
template<unsigned int N>
CFramework<N>::CFramework( )
 : frameworkSpace( new CubeID [ Fsize ] () )
{
  
}

// Conmposition of two cubes --> construct a new one
template<unsigned int N>
CFramework<N>::CFramework( const CFramework<N> & cf1, const CFramework<N> & cf2 )
 : frameworkSpace( new CubeID [ Fsize ] )
{
  for ( PosID id = 0; id < Fsize; ++id )
  {
    const PosID position = cf2.whatIs( id );
    frameworkSpace[ id ] = Simplex::Composition( cf1.getCubeID( position ), cf2.getCubeID( id ) );
  }
}

// copy
template<unsigned int N> 
CFramework<N>::CFramework( const CFramework<N>& C )
 : frameworkSpace( new CubeID [ Fsize ] )
{
  std::memcpy( frameworkSpace, C.frameworkSpace, Fsize );
}

template<unsigned int N> 
CFramework<N>::CFramework( const CFramework<N>& C, RotID rotID )
 : frameworkSpace( new CubeID [ Fsize ] )
{
  std::memcpy( frameworkSpace, C.frameworkSpace, Fsize );
  rotate( rotID );
}

template<unsigned int N>
CFramework<N>::CFramework( CFramework<N> && f )
{ 
  frameworkSpace = f.frameworkSpace;
  f.frameworkSpace = nullptr;
}

 // Operations
//  ----------

// inverse
template<unsigned int N> 
CFramework<N> CFramework<N>::inverse() const
{
  CFramework<N> inv;
  for ( PosID id = 0; id < Fsize; ++id )
  {
    const CubeID rotinv = Simplex::Inverse( frameworkSpace[ id ] );
    const int position = CPositions<N>::GetIndex( id, rotinv );
    inv.frameworkSpace[ position ] = rotinv;
  }
  return inv;
}

// clockwise rotation one layer (side) with 90 degree turns
template<unsigned int N> 
void CFramework<N>::rotate( const Axis axis, const Layer layer, const Turn turn )
{
  const int cubes = CPositions<N>::LayerSize( layer );
  CubeID state  [ N * N ]; // to store rotational states temporarily.
  
  const CubeID twist = Simplex::Tilt( axis, turn );
  for ( int index = 0; index < cubes; ++index )
  {
    const CubeID & f = frameworkSpace [ CPositions<N>::GetLayer( axis, layer, index ) ];
    state[index]     = Simplex::Composition( f, twist );
  }
  for ( int index = 0; index < cubes; ++index ) // apply stored assignments
  {
    frameworkSpace[ CPositions<N>::GetIndex( CPositions<N>::GetLayer( axis, layer, index ), twist ) ] = state[ index ];
  }
}

// rotation by using RotID
template<unsigned int N> void CFramework<N>::rotate( const RotID rotID )
{
  const Axis  axis  = getAxis  <N> ( rotID );
  const Layer layer = getLayer <N> ( rotID );
  const Turn  turn  = getTurn  <N> ( rotID );
  rotate( axis, layer, turn ); 
}

template<unsigned int N> 
void CFramework<N>::shuffle( int depth )
{
  static std::default_random_engine engine( static_cast<unsigned int>( time( 0 ) ) );
  static std::uniform_int_distribution<int> dist( 2 * N * N, 3 * N * N);
  int counter = depth == 0 ? dist( engine ) : depth; 
  while ( 0 < counter-- )
  {
    CFramework<N>::rotate( randomRotID<N>() );
  }
}

 // Query functions
//  ---------------
template<unsigned int N>
PosID CFramework<N>::whatIs( PosID id ) const
{ 
  return CPositions<N>::GetIndex( id, Simplex::Inverse( frameworkSpace [ id ] ) ); 
}

template<unsigned int N>
PosID CFramework<N>::whereIs( PosID id ) const
{ 
  CubeID rot = 0; 
  while ( frameworkSpace[ CPositions<N>::GetIndex( id, rot ) ] != rot )
  {
    ++ rot;
  }
  return CPositions<N>::GetIndex( id, rot );
}

template<unsigned int N>
Facet CFramework<N>::getFacet ( const Facet right, const Facet up, int x, int y ) const
{
  if ( Simplex::Coaxial ( right, up ) || x < 0 || x >= N || y < 0 || y >= N ) // invalid setting
  {
    return _NF;
  }
  
  const CubeID trans  = Simplex::GetGroupID   ( right, up ); 
  const CubeID inv    = Simplex::Inverse      ( trans ); 
  const Facet  facet  = OCube::FrontSide      ( right, up ); // = Simplex::GetCube( trans ).whatIs( _F );
  const int    index  = CPositions<N>::GetIndex ( x, y, N - 1, inv );
  
  return getCube ( index ).whatIs( facet );
}

template<unsigned int N> 
bool CFramework<N>::integrity() const
{
  static const Facet orientations [6][2] = { { _F, _U }, { _R, _U }, { _B, _U }, { _L, _U }, { _R, _B }, { _R, _F } };
  
  int counter[6] = {};
  for ( auto side : orientations )
  {
    for ( Layer x = 0; x < N; ++x )
    {
      for ( Layer y = 0; y < N; ++y )
      {
        ++counter[ CFramework<N>::getFacet( side[0], side[1], x, y ) ];
      }
    }
  }
  int idx = 0;
  while ( idx < 6 && counter[idx] == N * N )
  {
    ++idx;
  }
  return idx == 6;
}

 // Printer
//  -------
template<unsigned int N>
void CFramework<N>::show( Facet F ) const
{
  std::cout << colorOf( F ) << FChar << ' ';
}

template<unsigned int N>
void CFramework<N>::show( Facet right, Facet up, int x, int y ) const
{
  const Facet F = getFacet( right, up, x, y );
  show( F );
}

template<unsigned int N>
void CFramework<N>::print( Facet right, Facet up ) const
{
  std::cout << std::endl;
  for ( Layer y = N - 1; y >= 0; --y )
  {
    for ( Layer x = 0; x < N; ++x )
    {
      show( right, up, x, y );
    }
    NL();
  }        
  NL();
}

template<unsigned int N>
void CFramework<N>::print( bool separator ) const
{
  const int SideSize = separator ? N + 1 : N; 
  // print UP side
  for ( Layer y = SideSize - 1; y >= 0; --y )
  {
    for ( Layer x = 0; x < SideSize; ++x )
    {
      show( _NF );
    }
    for ( Layer x = 0; x < SideSize; ++x )
    {
      show( _R, _B, x, y );
    }
    NL();
  }
  // print middle sides Left - Front - Right - Back
  Facet orientations [] = { _F, _R, _B, _L };
  for ( Layer y = SideSize - 1; y >= 0; --y )
  {
    for ( Facet right: orientations )
    {
      for ( Layer x = 0; x < SideSize; ++x )
      {
        show( right, _U, x, y );
      }
	}
	NL();
  }
  // print DOWN side
  for ( Layer y = SideSize - 1; y >= 0; --y )
  {
    for ( Layer x = 0; x < SideSize; ++x )
    {
      show( _NF );
    }
    for ( Layer x = 0; x < SideSize; ++x )
    {
      show( _R, _F, x, y );
    }
    NL();
  }
  NL( Color::gray );
}

 // Destructor
//  ----------
template<unsigned int N>
CFramework<N>::~CFramework()
{
  delete[] frameworkSpace;
  frameworkSpace = nullptr;
}

#endif

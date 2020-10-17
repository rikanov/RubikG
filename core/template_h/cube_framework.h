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

#include <cube_basic_rotations.h>
#include <cube_positions.h>
#include <cstring> // memcmp, memcpy

/// ----------------------------------- Template declarations starts here ------------------------------------- ///
template<unsigned int N>
class CFramework
{
  static CFramework<N> * BaseRotations;
  static void            InitializeBase();

  static inline CFramework<N> BaseRotation   ( int rotID );
  static inline CFramework<N> BaseRotation   ( Axis A, byte slice, byte turn );
  static inline CFramework<N> RandomRotation ();
  
  static constexpr size_t _mem_size = CPositions<N>::GetSize() * sizeof( CubeID );
  CubeID * frameworkSpace;

public:

  // Constructors
  CFramework( void );
  CFramework( const CFramework<N>&, const CFramework<N>& );
  CFramework( CFramework<N>&& f );
  
  // Operations
  CFramework<N> inverse    ( void ) const;
  void          rotate     ( Axis, int  );
  void          rotate     ( byte rotID );
  void          shuffle    ( void );
  
  static CFramework<N> Transform  ( const CFramework<N>& A, const CFramework<N>& C ) { return CFramework<N>( A.inverse(), C ); } // transform( A, C ) returns with B where A + B = C
  
  // Operators
  constexpr CFramework<N>& operator=  ( const CFramework<N>& C ) { std::memcpy( frameworkSpace, C.frameworkSpace, _mem_size ); return *this; }
  bool                     operator== ( const CFramework<N>& X ) { return std::memcmp( frameworkSpace, X.frameworkSpace, _mem_size ) == 0;   }
  const     CFramework<N>  operator+  ( const CFramework<N>& B ) { return CFramework<N> ( *this, B ); }
  const     CFramework<N>  operator-  ( const CFramework<N>& B ) { return Transform( B, *this );      }
  // Destructor
  ~CFramework( );
  
  // Query functions
  CubeID     getCubeID ( int id  ) const { return frameworkSpace[id];                   }
  OCube      getCube   ( int id  ) const { return Simplex::GetCube( getCubeID ( id ) ); }
  inline int whatIs    ( int id  ) const ;
  inline int whereIs   ( int id  ) const ;
  Coord      whatIs    ( Coord C ) const { return CPositions<N>::getCoord( whatIs ( CPositions<N>::getIndex( C ) ) ); }
  Coord      whereIs   ( Coord C ) const { return CPositions<N>::getCoord( whereIs( CPositions<N>::getIndex( C ) ) ); }
  bool       integrity ( void    ) const ;
  Facet      getFacet  ( const Facet front, const Facet up, int x, int y) const; // left-bottom corner: x = 0, y = 0
  
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

 // Non-operational static functions
// ---------------------------------
template<unsigned int N> 
CFramework<N> CFramework<N>::BaseRotation( int rotID )
{
  return BaseRotations[rotID];
}

template<unsigned int N> 
CFramework<N> CFramework<N>::BaseRotation( Axis A, byte slice, byte turn )
{
  return BaseRotations[ getRotID<N>( A, slice, turn ) ];
}

template<unsigned int N> CFramework<N> CFramework<N>::RandomRotation()
{
  return BaseRotations[ randomRotID<N>() ];
}

template<unsigned int N> 
void CFramework<N>::InitializeBase()
{
  if ( BaseRotations )
  {
    return;
  }
  const int numberOfRotations = 3 * N * 3;
  BaseRotations = new CFramework<N> [ numberOfRotations ];
  for ( byte rotID = 0; rotID < numberOfRotations; ++rotID )
  {
    BaseRotations[rotID].rotate( rotID );
  }
}

  
 // Constructors
//  ------------
template<unsigned int N>
CFramework<N> * CFramework<N>::BaseRotations = nullptr;

template<unsigned int N>
CFramework<N>::CFramework( )
{
  frameworkSpace = new CubeID [ CPositions<N>::GetSize() ] ();
}

// Conmposition of two cubes --> construct a new one
template<unsigned int N>
CFramework<N>::CFramework( const CFramework<N> & cf1, const CFramework<N> & cf2 )
{
  frameworkSpace = new CubeID [ CPositions<N>::GetSize() ];
  for ( int id = 0; id < CPositions<N>::GetSize(); ++id )
  {
    const int position = cf2.whatIs( id );
    frameworkSpace[ id ] = Simplex::Composition( cf1.getCubeID( position ), cf2.getCubeID( id ) );
  }
}

template<unsigned int N>
CFramework<N>::CFramework( CFramework<N> && f )
{ 
  frameworkSpace = f.frameworkSpace;
  f.frameworkSpace = nullptr; clog_ ( "called" );
}

 // Operations
//  ----------

// inverse
template<unsigned int N> 
CFramework<N> CFramework<N>::inverse() const
{
  CFramework<N> inv;
  for ( int id = 0; id < CPositions<N>::GetSize(); ++id )
  {
    const CubeID rotinv = Simplex::Inverse( frameworkSpace[ id ] );
    const int position = CPositions<N>::GetIndex( id, rotinv );
    inv.frameworkSpace[ position ] = rotinv;
  }
  return inv;
}

// clockwise rotation one slice (side) with 90 degree
template<unsigned int N> 
void CFramework<N>::rotate( Axis axis, int slice )
{
  const int cubes = ( slice == 0 || slice == N - 1 ) ? N * N : 4 * ( N - 1 );
  int socket [ N * N ];
  int state  [ N * N ]; // to store indices and values temporarily. Rotations must be atomic operations
  for ( int index = 0; index < cubes; ++index )
  {
    socket[index]    = CPositions<N>::GetSlice( axis, slice, index );
    const CubeID & f = frameworkSpace [ socket[index] ];
    state[index]     = Simplex::Composition( f, Simplex::Tilt( axis ) );
  }
  for ( int index = 0; index < cubes; ++index ) // apply stored assignments
  {
    frameworkSpace[ CPositions<N>::GetIndex( socket[index], Simplex::Tilt( axis ) ) ] = state[ index ];
  }
}

// rotation by rotat IDs
template<unsigned int N> void CFramework<N>::rotate( byte rotID )
{
  Axis axis  = getAxis  <N> ( rotID );;
  byte slice = getSlice <N> ( rotID );
  byte turn  = getTurn  <N> ( rotID );
  
  while ( 0 < turn-- )
  {
    rotate( axis, slice ); 
  }
}

template<unsigned int N> 
void CFramework<N>::shuffle()
{
  static std::default_random_engine engine( time(0) );
  static std::uniform_int_distribution<int> dist( 2 * N * N, 3 * N * N);
  int counter = dist( engine ); 
  while ( 0 < counter-- )
  {
    CFramework<N>::rotate( randomRotID<N>() );
  }
}

 // Query functions
//  ---------------
template<unsigned int N>
int CFramework<N>::whatIs( int id ) const
{ 
  return CPositions<N>::GetIndex( id, Simplex::Inverse( frameworkSpace [ id ] ) ); 
}

template<unsigned int N>
int CFramework<N>::whereIs( int id ) const
{ 
  int orbit = id;
  while ( whatIs( orbit ) != id )
  { 
    orbit = whatIs( orbit ); // go to the next in the cycle
  }
  return orbit; // found: whereIs( orbit ) = id;
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
    for ( int x = 0; x < N; ++x )
    {
      for ( int y = 0; y < N; ++y )
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
  for ( int y = N - 1; y >= 0; --y )
  {
    for ( int x = 0; x < N; ++x )
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
  for ( int y = SideSize - 1; y >= 0; --y )
  {
    for ( int x = 0; x < SideSize; ++x )
    {
      show( _NF );
    }
    for ( int x = 0; x < SideSize; ++x )
    {
      show( _R, _B, x, y );
    }
    NL();
  }
  // print middle sides Left - Front - Right - Back
  Facet orientations [] = { _F, _R, _B, _L };
  for ( int y = SideSize - 1; y >= 0; --y )
  {
    for ( Facet right: orientations )
    {
      for ( int x = 0; x < SideSize; ++x )
      {
        show( right, _U, x, y );
      }
	}
	NL();
  }
  // print DOWN side
  for ( int y = SideSize - 1; y >= 0; --y )
  {
    for ( int x = 0; x < SideSize; ++x )
    {
      show( _NF );
    }
    for ( int x = 0; x < SideSize; ++x )
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

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

#include "simplex.h"
#include "cube_positions.h"

/// ----------------------------------- Template declarations starts here ------------------------------------- ///
template<unsigned int N>
class CFramework
{
  static  CFramework<N> * BasicMoves;

  CubeID * frameworkSpace;

public:

  // Constructors
  CFramework( void );
  CFramework( const CFramework<N>&, const CFramework<N>& );
  CFramework( CFramework<N>&& f );
  
  // Operations
  CFramework<N> inverse (void);
  void rot( Axis, int );
  
  // Destructor
  ~CFramework( );
  
  // Query functions
  CubeID     getCubeID ( int id  ) const { return frameworkSpace[id];                   }
  OCube      getCube   ( int id  ) const { return Simplex::GetCube( getCubeID ( id ) ); }
  inline int whatIs    ( int id  ) const ;
  inline int whereIs   ( int id  ) const ;
  Coord      whatIs    ( Coord C ) const { return CPositions<N>::getCoord( whatIs ( CPositions<N>::getIndex( C ) ) ); }
  Coord      whereIs   ( Coord C ) const { return CPositions<N>::getCoord( whereIs( CPositions<N>::getIndex( C ) ) ); }
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
  static const char FChar = (char) 254u;
#endif

/// ----------------------------------- Template definitions starts here ------------------------------------- ///

 // Constructors
//  ------------
template<unsigned int N>
CFramework<N> * CFramework<N>::BasicMoves = nullptr;

template<unsigned int N>
CFramework<N>::CFramework( )
{
  frameworkSpace = new CubeID [ CPositions<N>::GetSize() ] ();
}

template<unsigned int N>
CFramework<N>::CFramework( const CFramework<N> & cf1, const CFramework<N> & cf2 )
{
  frameworkSpace = new CubeID [ CPositions<N>::GetSize() ];
  for ( int id = 0; id < CPositions<N>::GetSize(); ++id )
  {
    frameworkSpace[id] = Simplex::Composition( cf2.getCubeID( id ), cf1.getCubeID( id ) );
  }    
}

template<unsigned int N>
CFramework<N>::CFramework( CFramework<N> && f )
{ 
  frameworkSpace = f.frameworkSpace;
  f.frameworkSpace = nullptr;
}

 // Operations
//  ----------
template<unsigned int N> 
CFramework<N> CFramework<N>::inverse()
{
 CFramework<N> inv;
 for ( int id = 0; id < CPositions<N>::GetSize(); ++id )
 {
   inv.frameworkSpace[ id ] = Simplex::Inverse( frameworkSpace[ id ] );
 }
 return inv;
}

template<unsigned int N> 
void CFramework<N>::rot( Axis axis, int slice )
{
  const int cubes = ( slice == 0 || slice == N - 1 ) ? N * N : 4 * ( N - 1 );
  int socket [ N * N ];
  int state  [ N * N ]; // to store indices and values temporarily. Rotations must be atomic operations
  for ( int index = 0; index < cubes; ++index)
  {
    socket[index]    = CPositions<N>::GetSlice( axis, slice, index );
    const CubeID & f = frameworkSpace [ socket[index] ];
    state[index]     = Simplex::Composition( f, Simplex::Tilt( axis ) );
  }
  for ( int index = 0; index < cubes; ++index ) // apply stored assignments
  {
    frameworkSpace[ CPositions<N>::GetPlace( socket[index], Simplex::Tilt( axis ) ) ] = state[ index ];
  }
}

 // Query functions
//  ---------------
template<unsigned int N>
int CFramework<N>::whatIs( int id ) const
{ 
  return CPositions<N>::GetPlace( id, Simplex::Inverse( frameworkSpace [ id ] ) ); 
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
  const Facet  facet  = OCube::FrontSide    ( right, up ); // = Simplex::GetCube( trans ).whatIs( _F );
  const int    index  = CPositions<N>::GetIndex ( x, y, N - 1, inv );
  
  return getCube ( index ).whatIs( facet );
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
  coff();
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
  coff();
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

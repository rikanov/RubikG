/*
 * Node objects to store evolutioned rotational states in the static member tree structures
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
 
#ifndef CUBE_NODE__H
#define CUBE_NODE__H

#include <cube_framework.h>

template<unsigned int N>
class CNode
{
  static CNode<N>* Tree;
  static byte      Depth;
  static long*     IndexOFLevel;
  
  const CNode<N>*      parent;
  const byte           rotID;
  const CFramework<N>  data;
  
  // create root
  CNode( );
  
  // create a new node from a parent node by a rotational operation
  CNode( CNode<N>* aParent, byte aRotID ); 
  
  // calculate the number of nodes ( in a tree of any size )
  static void initIndices();
  
  // cube frameworks generated by n rotations starts from here:
    static const CNode< N >* StartNode( int level = 0 ); 
  
public:
  
  // build a new tree of a given size
  static void Initialize( int depth );
};

 // set default values to static members
//  ------------------------------------

template<unsigned int N>
CNode<N>* CNode<N>::Tree = nullptr;

template<unsigned int N>
long* CNode<N>::IndexOFLevel = nullptr;
 
template<unsigned int N>
byte CNode<N>::Depth = 0;

 // Constructors
//  ------------

// root
template<unsigned int N>
CNode<N>::CNode()
  : parent( nullptr )
  , rotID ( 9 * N /* not exist */ )
  , data  ( CFramework<N>() )
{
}

// inner node (having a parent node)
template<unsigned int N>
CNode<N>::CNode( CNode<N>* aParent, byte aRotID )
  : parent( aParent )
  , rotID ( aRotID )
  , data  ( parent->data, rotID )
{
}

 // Building trees
//  -------------- 

template<unsigned int N> 
void CNode<N>::Initialize( int depth )
{
  if ( depth == Depth )
  {
    // nothing to do
    return;
  }
  Depth = depth;
  
  if ( IndexOFLevel == nullptr )
  {
    // build a new tree
    initIndices();
  }
  
  // allocate a new tree
  Tree = new CNode<N> [ IndexOFLevel[ Depth + 1 ] ];
}

// calculate the width of levels
template<unsigned int N> void CNode<N>::initIndices()
{
  // create a new array
    IndexOFLevel = new long [9];
  
  // calculate the constant ratio of the iteration
    auto sum = []( int b ) { return ( b * ( b - 1 ) ) / 2; };
    const int iter = ( 9 * N ) * ( 9 * N ) - 3 * sum( 3 * N );
  
  // Initialize the recursion
    IndexOFLevel[0] = 0;         // level 0 : empty tree
    IndexOFLevel[1] = 1;         // level 1 : root
    IndexOFLevel[2] = 3 * N * 3; // level 2 : (3 axes) x (N slices) x (3 possible turns)
  
  // calculate the number of nodes in a given level recursively
    for ( int dep = 3; dep < 9; ++dep )
        IndexOFLevel[dep] = IndexOFLevel[dep-2] * iter;
  
  // number of nodes in a tree = sum of levels
    for ( int dep = 3; dep < 9; ++dep )
        IndexOFLevel[dep] += IndexOFLevel[dep-1];
}

template<unsigned int N> 
const CNode<N> * CNode<N>::StartNode( int level )
{
  return Tree + IndexOFLevel[ level ]; 
}

#endif

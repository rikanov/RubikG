/*
 * Node objects to store evolutioned rotational states in tree structures
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

#include <CFramework>

template<unsigned int N>
class CNode
{
  const CNode<N>*      parent;
  const byte           rotID;
  const byte           depth;
  const CFramework<N>  data;
  
public:
  CNode( /* root */ );
  CNode( CNode<N>* aParent, byte aRotID );
};

template<unsigned int N>
CFramework<N>::CNode( /* root */ )
  , parent( nullptr )
  , rotID ( 9 * N /* not exist */ )
  , depth ( 0 )
  , data  ( CFramework<N>() )
{
}

template<unsigned int N>
CFramework<N>::CNode( CNode<N>* aParent, byte aRotID )
  , parent( aParent )
  , rotID ( aRotID )
  , depth ( parent->depth + 1 )
  , data  ( parent->data, rotID )
{
}
#endif

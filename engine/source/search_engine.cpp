/********** Template definitions, do not add this source file to the project build tasks **********/


#include <engine.h>


template<unsigned int N> 
bool Engine<N>::isSolved() const
{
  bool solved = true;
  const CubeID& orient = m_selectedCubes[ 0 ].rot;
  for ( unsigned t = 1; solved && t < m_numberOfCubes; ++ t )
  {
    const Slot& next = m_selectedCubes[ t ];
    solved  = ( orient ==  next.rot );
    if ( m_solidColor )
    {
      solved |= next.facet != _NF && Simplex::GetCube( orient ).whatIs( CPositions<N>::Side( next.pos, next.rot ) ) == next.facet; 
    }
  }
  for ( unsigned t = 0; solved && t < m_numberOfCubes; ++ t )
  {
    clog( CPositions<N>::GetCoord( m_selectedCubes[ t ].pos ).toString(), Simplex::GetCube( m_selectedCubes[ t ].rot ).toString() );
  }
  return solved;
}

template<unsigned int N> 
bool  Engine<N>::testLayer( const Axis axis, const Layer layer )
{
  ++ m_depth;
  if ( m_counter[ axis ][ layer ] > 0 )
  {
    for ( Turn turn : { 1, 2, 3 } )
    {
      turnLayer( axis, layer );
      if ( extend( axis, layer + 1 ) )
      {
        m_solution.push_back( getRotID <N> ( axis, layer, turn ) );
        return true;
      }
    }
    turnLayer( axis, layer ); // turn back to original position
  }
  -- m_depth;
  return false; // solution not found on this branch
}

template<unsigned int N> 
bool Engine<N>::extend( const Axis axis, const Layer layer )
{
  if ( m_depth == m_maxDepth )
  {
    return isSolved();
  }
  for ( Axis A: { _X, _Y, _Z } )
  {
    for ( Layer L = ( A == axis ? layer : 0 ); L < N; ++ L )
    {
      if ( testLayer( A, L ) )
      {
        return true;
      }
    }
  }
  return false;
}

template<unsigned int N> 
CubeID Engine<N>::solve( const int& depth )
{
  for ( int test = 0; !isSolved() && test < depth; ++ test )
  {
    m_solution.clear();
    m_depth = 0;
    m_maxDepth = test;
  
    if ( extend( _NA, 0 ) )
    {
      std::reverse( m_solution.begin(), m_solution.end() );
      clog( m_solution.size(), "rotations" );
      for ( RotID rotID: m_solution )
      {
        clog ( toString<N>( rotID ) );
        m_CFramework.rotate( rotID );
      }
      return m_selectedCubes[ 0 ].rot;
    }
  }
  return 0; // solution not found
}

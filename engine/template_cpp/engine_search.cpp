/******* Template definitions: do not add this source file to CMake directly ! *******/

#include <engine.h>

template<unsigned int N>
Counter Engine<N>::solve( Counter depth, bool exec )
{
  m_depth = 0;
  m_maxDepth = depth;
  m_solution.clear();
  const bool success = m_sentinel -> isSolved() || extend( _NA, 0 );
  std::reverse( m_solution.begin(), m_solution.end() );
  if ( exec )
  {
    for ( RotID r: m_solution )
    {clog( toString<N>( r ));
      m_CFramework -> rotate( r );
    }
  }clog( "success:", success, "exec:", exec);
  return success ? m_depth : -1 ;
}

template<unsigned int N> 
bool Engine<N>::testLayer( const Axis axis, const Layer layer )
{
  ++ m_depth;
  if ( m_sentinel -> count( axis, layer ) > 0 )
  {
    for ( Turn turn : { 1, 2, 3 } )
    {
      m_sentinel -> turnLayer( axis, layer );
      if ( m_sentinel -> isSolved() || ( m_depth < m_maxDepth && extend( axis, layer + 1 ) ) )
      {
        m_solution.push_back( getRotID <N> ( axis, layer, turn ) );
        return true;
      }
    }
    m_sentinel -> turnLayer( axis, layer ); // turn back to original position
  }
  -- m_depth;
  return false; // solution not found on this branch
}

template<unsigned int N>
bool Engine<N>::extend( const Axis axis, const Layer layer )
{
  for ( Axis A: { _X, _Y, _Z } )
  {
    for ( Layer L = ( A == axis ? layer : 0 ); L < N; ++L )
    {
      if ( testLayer( A, L ) )
      {
        return true;
      }
    }
  }
  return false;
}

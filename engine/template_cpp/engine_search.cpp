/******* Template definitions: do not add this source file to CMake directly ! *******/

#include <engine.h>

template<unsigned int N>
Counter Engine<N>::solve( Counter depth, bool exec )
{
  if ( m_sentinel -> isSolved() )
  {
    return 0;
  }
  m_depth = 0;
  m_maxDepth = m_cachedRotations -> level( cacheID() );
  m_solution.clear();
  if ( speedSolver() )
  {
    std::reverse( m_solution.begin(), m_solution.end() );
    applySolution( exec );
    return m_depth;
  }
  if ( m_cachedRotations -> level( cacheID() ) > depth )
  {
    return 0xFF;
  }
  for( m_maxDepth = 1; m_maxDepth <= depth; ++ m_maxDepth )
  {
    m_depth = 0;
    m_solution.clear();
    clog( m_maxDepth );
    if ( extend( _NA, 0 ) )
    {
      std::reverse( m_solution.begin(), m_solution.end() );
      applySolution( exec );
      break;
    }
  }
  return m_maxDepth <= depth ? m_depth : 0xFF ;
}

template<unsigned int N> 
bool Engine<N>::testLayer( const Axis axis, const Layer layer )
{
  if ( m_depth < m_maxDepth && m_sentinel -> count( axis, layer ) > 0 )
  {
    ++ m_depth;
    for ( Turn turn : { 1, 2, 3 } )
    {
      m_sentinel -> turnLayer( axis, layer );
      if ( extend( axis, layer + 1 ) )
      {
        m_solution.push_back( getRotID <N> ( axis, layer, turn ) );
        return true;
      }
    }
    m_sentinel -> turnLayer( axis, layer ); // turn back to original position
    -- m_depth;
  }
  return false; //  evaluate leaf nodes
}

template<unsigned int N> 
bool Engine<N>::speedSolverStep( const Axis axis, const Layer layer, const Turn turn )
{
  m_sentinel -> turnLayer( axis, layer, turn );
  if ( speedSolver() )
  {
    m_solution.push_back( getRotID <N> ( axis, layer, turn ) );
    return true;
  }
  m_sentinel -> turnLayer( axis, layer, 4 - turn ); // turn back to original position
  return false;
}

template<unsigned int N>
bool Engine<N>::speedSolver()
{
  if ( m_depth == m_maxDepth )
  {
    return m_sentinel -> isSolved( true );
  }
  for ( RotID rotID = m_cachedRotations -> start( cacheID() ); rotID; rotID = m_cachedRotations -> next() )
  {
    ++ m_depth;
    if ( speedSolverStep( getAxis <N> ( rotID ), getLayer <N> ( rotID ), getTurn <N> ( rotID ) ) )
    {
      return true; 
    }
    -- m_depth;
  }
  return false;
}

template<unsigned int N>
bool Engine<N>::extend( const Axis axis, const Layer layer )
{
  if ( m_cachedRotations -> level( cacheID() ) > m_maxDepth - m_depth )
  {
    return false;
  }
  if ( m_cachedRotations -> level( cacheID() ) == m_maxDepth - m_depth )
  {
    return speedSolver();
  }
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

template<unsigned int N>
void Engine<N>::applySolution( const bool & exec )
{
  if ( exec )
  {
    for ( RotID r: m_solution )
    {
      clog( toString<N>( r ));
      m_CFramework -> rotate( r );
    }
  }
}

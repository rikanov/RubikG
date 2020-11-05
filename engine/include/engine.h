#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include <cube_node.h>
#include <algorithm>
#include <vector>
#include <initializer_list>

struct Slot
{
  PosID pos = 0;
  RotID rot = 0;
};

template<unsigned int N> 
class Engine
{
  CFramework<N> & m_CFramework;
  
  Slot *  m_selectedCubes;          // selected cubes to solve: pos = original position  rot = current rotational state
  size_t  m_numberOfCubes;          // number of cubes > 1
  byte    m_counter[ 3 ][ N ] = {}; // the number of selected cubes on a layer [ Axis ][ Layer index ]
  byte    m_depth;                  // depth of the current search
  byte    m_maxDepth;               //
  std::vector<RotID> m_solution;    // path to solution
  
  // inline functions to maintain counter data
  void addCube( Slot * S )  { for ( Axis axis : { _X, _Y, _Z } ) ++ m_counter[ axis ][ CPositions<N>::GetCoord( S -> pos, S -> rot, axis ) ]; }
  void delCube( Slot * S )  { for ( Axis axis : { _X, _Y, _Z } ) -- m_counter[ axis ][ CPositions<N>::GetCoord( S -> pos, S -> rot, axis ) ]; }
  
  // Query functions
  bool isTwistedOnAxis( Axis A ) const;
  bool isSolved() const;
  
  // search engines
  bool  extend    ( const Axis, const Layer );
  bool  testLayer ( const Axis, const Layer );
  void  turnLayer ( const Axis, const Layer );
  
public:
  Engine ( CFramework<N> & C, const std::initializer_list<PosID> & P );
  ~Engine();
  
  int solve( const byte& depth );
  const std::vector< RotID >& getSolution( void ) const;
};

template<unsigned int N> Engine<N>::Engine( CFramework<N>& C, const std::initializer_list<PosID> & P )
 : m_CFramework    ( C )
 , m_selectedCubes ( new Slot [ P.size() ] )
 , m_numberOfCubes ( P.size() )
{
  Slot * slotPointer = m_selectedCubes;
  for ( PosID pos: P )
  {
    slotPointer -> pos = pos;
    slotPointer -> rot = m_CFramework.getCubeID( m_CFramework.whereIs( pos ) );
    addCube( slotPointer ++ );    
  }
}

template<unsigned int N> Engine<N>::~Engine()
{
  delete[] m_selectedCubes;
}

template<unsigned int N> bool Engine<N>::isSolved() const
{
  for ( int t = 0; t < m_numberOfCubes; ++ t )
  {
    if ( 0 !=  m_selectedCubes[ t ].rot )
    {
      return false;
    }
  }clog( "depth:", (int)m_depth);
  return true;
}

template<unsigned int N> 
bool Engine<N>::isTwistedOnAxis( Axis axis ) const
{
  for ( Layer layer = 0; layer < N; ++ layer )
  {
    const PosID first = CPositions<N>::GetLayer( axis, layer, 0 );
    for ( PosID index = 1; index <= CPositions<N>::LayerSize( layer ); ++ index )
    {
      if ( m_CFramework.getCubeID( CPositions<N>::GetLayer( axis, layer, index ) ) != m_CFramework.getCubeID( first ) )
      {
        return false;
      }
    }
  }
  return true;
}

template<unsigned int N> 
void Engine<N>::turnLayer( const Axis axis, const Layer layer )
{
  int parts = m_counter[ axis ][ layer ];
  for( Slot * slotPointer = m_selectedCubes; parts > 0; ++ slotPointer )
  {
    if ( CPositions<N>::GetCoord( slotPointer -> pos, slotPointer -> rot, axis ) == layer )
    {
      delCube( slotPointer );        
      slotPointer -> rot = Simplex::Composition( slotPointer -> rot, Simplex::Tilt( axis ) );
      addCube( slotPointer );
      -- parts;
    }
  }
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
      if ( isSolved() || ( m_depth < m_maxDepth && extend( axis, layer + 1 ) ) )
      {
        m_solution.push_back( getRotID <N> ( axis, layer, turn ) ); clog( toString<N>(getRotID <N> ( axis, layer, turn ) ), m_solution.size() );
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
  for ( Axis A: { _X, _Y, _Z } )
  {
    for ( Layer L = ( A == axis ? layer : 0 ); L < N; ++L )
    {
      if ( testLayer( A, L ) )
      {
        return true; clog("S", m_solution.size() );
      }
    }
  }
  return false;
}

template<unsigned int N> 
int Engine<N>::solve( const byte& depth )
{
  m_solution.clear();
  m_depth = 0;
  m_maxDepth = depth;
  const bool success = extend( _NA, 0 );
  std::reverse( m_solution.begin(), m_solution.end() );
  clog( m_solution.size(), "rotations" );
  for ( RotID rotID: m_solution )
  {
    clog ( toString<N>( rotID ) );
    m_CFramework.rotate( rotID );
  }
  return success ? m_depth : -1 ;
}

#endif // !ENGINE_HEADER

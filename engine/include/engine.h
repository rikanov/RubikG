#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include <cube_node.h>
#include <algorithm>
#include <vector>
#include <initializer_list>

struct Slot
{
  PosID  pos = 0;
  CubeID rot = 0;
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
  
  CubeID solve( const int& depth );
  const  std::vector< RotID >& getSolution( void ) const;
};

template<unsigned int N> Engine<N>::Engine( CFramework<N>& C, const std::initializer_list<PosID> & P )
 : m_CFramework    ( C )
 , m_selectedCubes ( new Slot [ P.size() ] )
 , m_numberOfCubes ( P.size() )
{
  Slot * slotPointer = m_selectedCubes;
  for ( PosID pos: P )
  {
    if( pos == 0xFF )
      continue;
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
  for ( unsigned t = 1; t < m_numberOfCubes; ++ t )
  {
    if ( m_selectedCubes[ 0 ].rot !=  m_selectedCubes[ t ].rot )
    {
      return false;
    }
  }
  for ( unsigned t = 0; t < m_numberOfCubes; ++ t )
  {
    clog( CPositions<N>::GetCoord( m_selectedCubes[ t ].pos ).toString(), Simplex::GetCube( m_selectedCubes[ t ].rot ).toString() );
  }
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
  for ( int test = 0; test < depth; ++ test )
  {
    m_solution.clear();
    m_depth = 0;
    m_maxDepth = test;

    const bool success = extend( _NA, 0 );
  
    if ( success )
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

#endif // !ENGINE_HEADER

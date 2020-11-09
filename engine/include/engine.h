#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include <cube_node.h>
#include <algorithm>
#include <vector>
#include <initializer_list>

struct Slot
{
  Facet  facet = _NF;
  PosID  pos   = 0;
  CubeID rot   = 0;
};

template<unsigned int N> 
class Engine
{
  CFramework<N> & m_CFramework;
  
   // Searching parameters
  //  --------------------
  int           m_maxDepth;         // depth of the searching tree
  const bool    m_solidColor;       // it is true if the sides colored with solid pattern
  
   // Inner states
  //  ------------
  Slot *  m_selectedCubes;          // selected cubes to solve: pos = original position  rot = current rotational state
  size_t  m_numberOfCubes;          // number of cubes > 1
  int     m_counter[ 3 ][ N ] = {}; // the number of selected cubes on a layer [ Axis ][ Layer index ]
  int     m_depth;                  // current level of the search
  
    // Cache
   //  -----
   CubeID * m_cache;
   
  
   // Results
  //  -------
  std::vector<RotID> m_solution;    // path to solution
  // Query functions
  bool isTwistedOnAxis( Axis A ) const;
  bool isSolved() const;
  
   // search engines
  //  --------------
  bool  extend    ( const Axis, const Layer );
  bool  testLayer ( const Axis, const Layer );
  void  turnLayer ( const Axis, const Layer );
  // inline functions to maintain counter data
  void  addCube   ( Slot * S )  { for ( Axis axis : { _X, _Y, _Z } ) ++ m_counter[ axis ][ CPositions<N>::GetCoord( S -> pos, S -> rot, axis ) ]; }
  void  delCube   ( Slot * S )  { for ( Axis axis : { _X, _Y, _Z } ) -- m_counter[ axis ][ CPositions<N>::GetCoord( S -> pos, S -> rot, axis ) ]; }
  
   // cache functions
  //  ---------------
  void initCache();
  
public:
  
  // constructor & destructor
  Engine ( CFramework<N> & C, const bool solidColor = true );
  ~Engine();
  
  // search engine functions
  void   constrain( const std::initializer_list<PosID>& );
  CubeID solve( const int& depth );
  const  std::vector< RotID >& getSolution( void ) const;
};

#include <../source/engine.cpp>        // template engine definitions
#include <../source/search_engine.cpp> // search engine definitions

#endif // !ENGINE_HEADER

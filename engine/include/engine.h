#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include <sentinel.h>
#include <qeueu.h>
#include <cache.h>

#include <algorithm>
#include <vector>
#include <initializer_list>

template<unsigned int N> 
class Engine
{
  CFramework<N> * m_CFramework;
  
   // Searching parameters
  //  --------------------
  int           m_maxDepth;         // depth of the searching tree
  const bool    m_solidColor;       // it is true if the sides colored with solid pattern
  
   // Inner states
  //  ------------
  CubeSlot *  m_selectedCubes;          // selected cubes to solve: pos = original position  rot = current rotational state
  Counter     m_numberOfCubes;          // number of cubes > 1
  Counter     m_counter[ 3 ][ N ] = {}; // the number of selected cubes on a layer [ Axis ][ Layer index ]
  Counter     m_depth;                  // current level of the search
  Sentinel<N> m_sentinel;
   // Cache
  //  -----
  const Counter   m_numberOfCachedCubes;
  CubeID  * m_cache ;
  CacheID * m_qeueu ;
  CacheID * m_qeuIn ;
  CacheID * m_qeuOut;
  Counter * m_cacheCounter;
  Counter * m_cacheLevel;
  Counter   m_qeueuLevel;
  const std::initializer_list<PosID>& m_cachePositions;
  
   // Results
  //  -------
  std::vector<RotID> m_solution;    // path to solution
  // Query functions
  bool isSolved() const;
  
   // search engines
  //  --------------
  bool  extend    ( const Axis, const Layer );
  bool  testLayer ( const Axis, const Layer );
  void  turnLayer ( const Axis, const Layer );
  // inline functions to maintain counter data
  void  addCube   ( CubeSlot * S )  { for ( Axis axis : { _X, _Y, _Z } ) ++ m_counter[ axis ][ CPositions<N>::GetCoord( S -> pos, S -> rot, axis ) ]; }
  void  delCube   ( CubeSlot * S )  { for ( Axis axis : { _X, _Y, _Z } ) -- m_counter[ axis ][ CPositions<N>::GetCoord( S -> pos, S -> rot, axis ) ]; }
  void  updateCubes();
   // cache functions
  //  ---------------
  void allocateCache();
  void initCache();
  void buildCache();
  void initQeueu();
  void addToQeueu();
  void setFromQeueu();
  CacheID getCacheID() const;
public:
  
  // constructor & destructor
  Engine ( const std::initializer_list<PosID>&, const bool solidColor = true );
  ~Engine();
  
  // search engine functions
  void   toSolve( CFramework<N> * );
  void   constrain( const std::initializer_list<PosID>& );
  CubeID solve( const int& depth );
  const  std::vector< RotID >& getSolution( void ) const;
};

#include <../source/engine.cpp>        // template engine definitions
#include <../source/search_engine.cpp> // search engine definitions
#include <../source/cache.cpp>         // cache engine definitions
#endif // !ENGINE_HEADER

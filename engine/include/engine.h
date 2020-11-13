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
  
  Sentinel<N> m_sentinel;
  Qeueu       m_cacheQeueu;
  CCache      m_cubeCache;

   // Results
  std::vector<RotID> m_solution;    // path to solution
  // Query functions
  bool isSolved() const;
  
   // search engine
  bool  extend    ( const Axis, const Layer );
  bool  testLayer ( const Axis, const Layer );
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

/******* Template definitions: do not add this source file to CMake directly ! *******/

#include <engine.h>

template<unsigned int N>
Engine<N>::Engine( CubeList P, const bool& solidColor )
  : m_CFramework      ( nullptr )
  , m_sentinel        ( new Sentinel<N>( P, solidColor ) )
  , m_cachedRotations ( CGenerator<N>  ( P ).getCache() )
{

}

template<unsigned int N>
Engine<N>::Engine( const CGenerator<N>& CGen, const bool& solidColor )
  : m_CFramework      ( nullptr )
  , m_sentinel        ( new Sentinel<N> ( CGen.getGenerators(), solidColor ) )
  , m_cachedRotations ( CGen.getCache() )
{

}

template<unsigned int N>
void Engine<N>::toSolve( CFramework<N> & CF)
{
  m_CFramework = &CF;
  m_sentinel -> toSolve( &CF );
}

template<unsigned int N> 
Engine<N>::~Engine()
{
  delete m_sentinel;
}


#include <engine.h>


static constexpr int CacheBufferSize = 8;

using CacheID = unsigned int;

static inline constexpr int pow24[] = { 1, 24, 576, 13824, 331776 };

static CacheID getCacheID (const CubeID x0, const CubeID x1, const CubeID x2, const CubeID x3 )
{
  return 0 * pow24[0] + x1 * pow24[1] + x2 * pow24[2] + x3 * pow24[3];
}
template<const unsigned int N>
void Engine<N>::initCache()
{
  m_cache = new CubeID [ CacheBufferSize * pow24[4] ];
}

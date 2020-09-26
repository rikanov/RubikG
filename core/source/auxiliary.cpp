#include "auxiliary.h"


 // Auxiliary functions
// --------------------

Color::Modifier colorOf( Facet F )
{
  Color::Modifier ret;
  switch ( F )
  {
    case _F:
      ret = Color::red;
      break;
    case _R:
      ret = Color::yellow;
      break;
    case _U:
      ret = Color::blue;
      break;
    case _L:
      ret = Color::white;
      break;
    case _D:
      ret = Color::green;
      break;
    case _B:
      ret = Color::light;
      break;
    default:
      ret = Color::black;
  }
  return ret;
}

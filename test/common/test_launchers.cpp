#include <test.h>

bool UnitTests::testCore() const
{
  bool result = true;
  result &= unit_Simplex();
  result &= unit_CFramework();
  result &= unit_Engine();
  return result;
}

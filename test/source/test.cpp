#include "test.h"
#include "global.h"

void UnitTests::head(const std::string& unit)
{
  clog( Color::white, Color::uline, unit, "unit test" );
}

void UnitTests::tail(const std::string& unit, bool success)
{
  NL();
  clog_( Color::white, unit, "unit test result:" );
  if ( success )
  {
    clog( Color::green, "SUCCESS" );
  }
  else
  {
    clog( Color::red, "FAILED!" );
  }
  clog( Color::gray, std::string( unit.length() + 26, '-' ) );
  NL();
}

bool UnitTests::run() const
{
  bool result = true;
  result &= unit_Simplex();
  return result;
}

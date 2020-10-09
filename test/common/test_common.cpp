#include "test.h"
#include "global.h"

void UnitTests::head(const std::string& unit)
{
  CLS();
  clog( Color::light, ">>>>>  ", Color::gray, "Starting unit test of", Color::white,  unit);
  clog(               "       ", std::string( 22 + unit.length(), '-' ) );
  NL();
}

void UnitTests::tail(const std::string& unit, bool success)
{
  NL();
  clog_( Color::white, unit, "test result:" );
  if ( success )
  {
    clog( Color::green, "SUCCEED" );
  }
  else
  {
    clog( Color::red, "FAILED!" );
  }
  NL();
}

void UnitTests::stamp( bool success, bool& result )
{
  if ( success )
  {
    clog( Color::green, "OK" );
  }
  else
  {
    clog( Color::red, "NO OK!" );
  }
  result = success;
}

void UnitTests::finish(const std::string& unit, bool success)
{
  NL();
  clog_( Color::gray, "Unit test of", Color::white,  unit, Color::off, ':');
  if ( success )
  {
    clog_( Color::green, "SUCCEED" );
  }
  else
  {
    clog_( Color::red, "FAILED!" );
  }
  clog( Color::light, "  <<<<<" );
  clog( std::string( 23 + unit.length(), '-' ) );
  NL();
  NL();  
  clog( Color::off, "Press", Color::white, "ENTER", Color::off,"to end." ); 
  std::cin.get();
}

void UnitTests::done()
{ 
  clog( Color::white, Color::uline, "Done.", Color::off ); 
} 


#include "global.h"
#include "cli.h"


#include <iostream>
#include <locale>

int initCLi( void )
{
  clog( "User-preferred locale setting is" , std::locale("").name().c_str() );
  
  // replace the C++ global locale as well as the C locale with the user-preferred locale
  std::locale::global( std::locale( "" ) );
  
  // use the new global locale for future special character output
  std::cout.imbue( std::locale( "" ) );
  
  return 0; // ToDo: error handling
}


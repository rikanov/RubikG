#ifndef TEXT_OUTPUT__H
#define TEXT_OUTPUT__H


#include <cstdlib>
#include <iostream>
#include <string>

#include "def_colors.h"

// clear screen ToDo
inline void CLS()
{
  if ( system( "CLS" ) ) system( "clear" );
}

// new line
inline void NL()
{
  std::cout << std::endl;
}

// color off & new line --> writing color will turn gray
inline void coff()
{
	std::cout << Color::gray << std::endl;
}

  // basic characters print out
 // ---------------------------
template <typename T>
void clog_ (T t) 
{
  if ( typeid( t ) != typeid( Color::Modifier ) )
  {
    std::cout << t << ' ';
  }
  else
  {
    std::cout << t;
  }
}

template <typename T, typename... Args>
void clog_ (T t, Args... args) 
{
  clog_ ( t );
  clog_ ( args... );
}

template <typename T>
void clog (T t) 
{
  std::cout << t << Color::off << std::endl ;
}

template<typename T, typename... Args>
void clog (T t, Args... args) // recursive variadic function
{
  clog_ ( t );
  clog  ( args... ) ;
}

 // wide characters print out
// --------------------------
template <typename T>
void wlog_ (T t) 
{
  std::wcout << t << ' ';
}

template <typename T>
void wlog (T t) 
{
  std::wcout << t << std::endl ;
}

template<typename T, typename... Args>
void wlog (T t, Args... args) // recursive variadic function
{
  wlog_ ( t );
  wlog  ( args...) ;
}

// logger
inline void clogPos(const char * text)
{
  clog( text, std::string("\t@  ") + std::string(__FILE__) + std::to_string(__LINE__) );
}


#endif // ! TEXT_OUTPUT__H

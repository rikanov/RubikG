/*
 * Global header for logger and auxiliary functions.
 *
 * Copyright (C) 2020  Robert Ikanov <robert.ikanov@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GLOBAL_HEADER
#define GLOBAL_HEADER

#include <stdio.h>
#include <iostream>
#include <string>

#include "def_colors.h"

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
    std::cout << t << ' ';
}

template <typename T>
void clog (T t) 
{
    std::cout << t << std::endl ;
}

template<typename T, typename... Args>
void clog (T t, Args... args) // recursive variadic function
{
    clog_ ( t );
    clog  ( args...) ;
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

#endif

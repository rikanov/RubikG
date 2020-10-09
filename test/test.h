#ifndef UNIT_TEST__H
#define UNIT_TEST__H

#include <string>

class UnitTests
{
private:
  
  static void head   ( const std::string& unit );
  static void tail   ( const std::string& unit, bool success );
  static void finish ( const std::string& unit, bool success );
  static void stamp  ( bool success, bool& result );
  static void done   ();
  
   // Unit tests of base classes (core folder)
  // -----------------------------------------
  bool unit_Simplex()    const; // simplex.h
  bool unit_OCube()      const; // oriented_cube.h
  bool unit_CPositions() const; // cube_positions.h
  bool unit_CFramework() const; // cube_framework.h
  
public:
  bool testCore() const;
  
};

#endif // ! UNIT_TEST__H

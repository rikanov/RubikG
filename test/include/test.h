#ifndef UNIT_TEST__H
#define UNIT_TEST__H

#include <string>

class UnitTests
{
private:
  
  static void head( const std::string& unit );
  static void tail( const std::string& unit, bool success );
  
   // Unit tests of base classes (core folder)
  // -----------------------------------------
  bool unit_Simplex()    const; // simplex.h
  bool unit_OCube()      const; // oriented_cube.h
  bool unit_CPositions() const; // cube_positions.h
  bool unit_CFramework() const; // cube_framework.h
  
public:
  bool run() const;
  
};

#endif // ! UNIT_TEST__H

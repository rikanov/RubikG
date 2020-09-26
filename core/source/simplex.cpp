#include "simplex.h"

Simplex * Simplex::Singleton = nullptr;

 /**************************************************************************************************************************
  * 
  * All the 24 simplex cube orientations are represented as the indices of two given direction (Up and Right in our case).  
  *                                                                                                                      
  * The simplexFrontSide[][] array contains the Front side indices belonging to the given up and Right indices,     
  * so we can determine all the other indices for any direction. (by using SideOpposite() function)
  * Example: if the Left facet lies on upside and the Down facet on the right, the front facet is:
  * 
  * Facet front = simplexFrontSide[ Down = 3 ][ Left = 4 ] = Back (aka 5)
  * 
  *************************************************************************************************************************/

Simplex::Simplex() :
    simplexFrontSide
{

    // Front   Right     Up   Down    Left   Back
    // --------------------------------------------
    {  _NF,     _U,     _L,    _R,     _D,    _NF },  // Front
    {   _D,    _NF,     _F,    _B,    _NF,     _U },  // Right
    {   _R,     _B,    _NF,   _NF,     _F,     _L },  // Up
    {   _L,     _F,    _NF,   _NF,     _B,     _R },  // Down
    {   _U,    _NF,     _B,    _F,    _NF,     _D },  // Left
    {  _NF,     _D,     _R,    _L,     _U,    _NF }   // Back
    // --------------------------------------------

     /*******************************************************
      *                                                                                                                  
      *       Notice the function symmetries:                                                 
      *                                                                                                                  
      *   - Swapping directions gives opposite side index                          
      *   - Using opposite side for one of the directions                        
      *     gives the opposite side index                                                                   
      *   - Using opposite sides for both direction gives                            
      *     the same side index                                                                         
      *                                                                                                                  
      ********************************************************/

}
{
  // Constructor body
  Singleton = this;
  init();
  initGroup();
}

void Simplex::init()
{
  // determine group IDs. Identity element most have ID = 0
  CubeID lastRadix[6] = {};
  all_facet ( right )
  {
    all_facet ( up )
    {
      if ( Coaxial(right, up) )
      {
        simplexGroupID [ right ][ up ] = _NF;
        continue;
      }
      CubeID front = FrontSide( right, up );
      const int index = 4 * front + lastRadix [ front ]++;
      simplexGroupID [ right ][ up ] = index;
      simplexGroup [ index ].init ( right, up );
       // created all the 24 orientation of a simplex and store them in an array
      //  indexed by their group ID
    }
  }
  tilt [ _X ] = Simplex::GetGroupID( _R, _F );
  tilt [ _Y ] = Simplex::GetGroupID( _B, _U );
  tilt [ _Z ] = Simplex::GetGroupID( _U, _L );
}

void Simplex::initGroup()
{
  all_id ( a )
    all_id ( b )
    {
      OCube A = GetCube ( a );
      OCube B = GetCube ( b );

      // create a composition of them:
      Facet cr = A.whatIs ( B.whatIs ( _R ) );
      Facet cu = A.whatIs ( B.whatIs ( _U ) );

      const CubeID c = GetGroupID ( cr, cu );

      composition	[a][b] = c;
      transform	[a][c] = b;
    }
}

#include <oriented_cube.h>

const char OCube::Tokens [7] = "FRUDLB";

 /**************************************************************************************************************************
  * 
  * All the 24 cube orientations are represented as the indices of two given direction (Up and Right in our case).  
  *                                                                                                                      
  * The FrontSides[][] array contains the Front side indices belonging to the given up and Right indices,     
  * so we can determine all the other indices for any facets / directions. 
  *  ( by using SideOpposite() function as it happens in the init() method )
  *
  * An example: if the Left facet lies on upside and the Down facet on the right, the front facet is:
  * 
  * Facet front = FrontSides [ Down = 3 ][ Left = 4 ] = Back (aka 5)
  * 
  *************************************************************************************************************************/

const Facet OCube::FrontSides [ 6 /*Right*/ ][ 6 /*Up*/ ] = {

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
};

void OCube::init( Facet r, Facet u, CubeID groupID )
{
	m_whatIs[_R] = r;
	m_whatIs[_U] = u;
	m_whatIs[_F] = FrontSide( r, u );

	m_whatIs[_L] = SideOpposite( m_whatIs[_R] );
	m_whatIs[_D] = SideOpposite( m_whatIs[_U] );
	m_whatIs[_B] = SideOpposite( m_whatIs[_F] );

	all_facet( ID )
	{
		m_whereIs[m_whatIs[ID]] = ID;
		m_aligned[ID] = ( m_whatIs[ID] == ID );
	}

	m_groupID = groupID;

	// Set a readable name 
	m_readable.push_back( Token ( whatIs( _R ) ) );
	m_readable.push_back( Token ( whatIs( _U ) ) );
	m_readable.push_back( Token ( whatIs( _F ) ) );
	m_readable.push_back( '(' );
	if (m_groupID < 10)
	{
		m_readable.push_back( ' ' );
	}
	m_readable.append( std::to_string( m_groupID ) );
	m_readable.push_back( ')' );
}

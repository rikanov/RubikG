#include "simplex.h"

const char OCube::Token [7] = "FRUDLB";

void OCube::init( Facet r, Facet u )
{
	m_whatIs[_R] = r;
	m_whatIs[_U] = u;
	m_whatIs[_F] = Simplex::FrontSide( r, u );

	m_whatIs[_L] = SideOpposite( m_whatIs[_R] );
	m_whatIs[_D] = SideOpposite( m_whatIs[_U] );
	m_whatIs[_B] = SideOpposite( m_whatIs[_F] );

	all_facet( ID )
	{
		m_whereIs[m_whatIs[ID]] = ID;
		m_aligned[ID] = ( m_whatIs[ID] == ID );
	}

	m_groupID = Simplex::GetGroupID( r, u );

	// Set a readable name 
	m_readable.push_back( token ( whatIs( _R ) ) );
	m_readable.push_back( token ( whatIs( _U ) ) );
	m_readable.push_back( token ( whatIs( _F ) ) );
	m_readable.push_back( '(' );
	if (m_groupID < 10)
	{
		m_readable.push_back( ' ' );
	}
	m_readable.append( std::to_string( m_groupID ) );
	m_readable.push_back( ')' );
}

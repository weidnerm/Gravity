// {{{RME classifier 'Logical View::Gravity::InitMain::InitMain'

#if defined( PRAGMA ) && ! defined( PRAGMA_IMPLEMENTED )
#pragma implementation "InitMain.h"
#endif

#include <Gravity_lib.h>
#include <InitMain.h>
#include <GravityEngine.h>

// {{{RME tool 'OT::Cpp' property 'ImplementationPreface'
// {{{USR
#include "stdlib.h"
// }}}USR
// }}}RME

// {{{RME tool 'OT::Cpp' property 'GenerateDefaultConstructor'
InitMain::InitMain( void )
{
}
// }}}RME

// {{{RME tool 'OT::Cpp' property 'GenerateDestructor'
InitMain::~InitMain( void )
{
}
// }}}RME

// {{{RME tool 'OT::Cpp' property 'GenerateCopyConstructor'
InitMain::InitMain( const InitMain & rtg_arg )
{
}
// }}}RME

// {{{RME tool 'OT::Cpp' property 'GenerateAssignmentOperator'
InitMain & InitMain::operator=( const InitMain & rtg_arg )
{
	if( this != &rtg_arg )
	{
	}
	return *this;
}
// }}}RME

// {{{RME operation 'SoftwareInitialize()'
void SoftwareInitialize( void )
{
	// {{{USR

	GravityEngine myGravityEngine;

	myGravityEngine.run( 365*300, 60*60*24 );

	exit(0);

	// }}}USR
}
// }}}RME

// {{{RME tool 'OT::Cpp' property 'ImplementationEnding'
// {{{USR

// }}}USR
// }}}RME

// }}}RME

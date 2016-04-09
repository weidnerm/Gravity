// {{{RME classifier 'Logical View::Gravity::GravityEngine::GravityEngine'

#if defined( PRAGMA ) && ! defined( PRAGMA_IMPLEMENTED )
#pragma implementation "GravityEngine.h"
#endif

#include <Gravity_lib.h>
#include <GravityEngine.h>

// {{{RME tool 'OT::Cpp' property 'ImplementationPreface'
// {{{USR
#include "stdio.h"

// }}}USR
// }}}RME

// {{{RME tool 'OT::Cpp' property 'GenerateDestructor'
GravityEngine::~GravityEngine( void )
{
}
// }}}RME

// {{{RME tool 'OT::Cpp' property 'GenerateCopyConstructor'
GravityEngine::GravityEngine( const GravityEngine & rtg_arg )
	: m_G( rtg_arg.m_G )
	, m_system( rtg_arg.m_system )
{
}
// }}}RME

// {{{RME tool 'OT::Cpp' property 'GenerateAssignmentOperator'
GravityEngine & GravityEngine::operator=( const GravityEngine & rtg_arg )
{
	if( this != &rtg_arg )
	{
		m_G = rtg_arg.m_G;
		m_system = rtg_arg.m_system;
	}
	return *this;
}
// }}}RME

// {{{RME operation 'GravityEngine()'
GravityEngine::GravityEngine( void )
	// {{{RME tool 'OT::Cpp' property 'ConstructorInitializer'
	// {{{USR

	// }}}USR
	// }}}RME
{
	// {{{USR

	// }}}USR
}
// }}}RME

// {{{RME operation 'run(uint32_t,double)'
void GravityEngine::run( uint32_t numIterations, double tickDuration )
{
	// {{{USR
	uint32_t iteration;

	for(iteration=0; iteration<numIterations ; iteration++ )
	{
	   m_system.processTimeInterval(tickDuration);
	}

	m_system.dumpSystemVerbose();



	// }}}USR
}
// }}}RME

// {{{RME tool 'OT::Cpp' property 'ImplementationEnding'
// {{{USR

// }}}USR
// }}}RME

// }}}RME

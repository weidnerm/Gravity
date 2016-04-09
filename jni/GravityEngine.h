// {{{RME classifier 'Logical View::Gravity::GravityEngine::GravityEngine'

#ifndef GravityEngine_H
#define GravityEngine_H

#ifdef PRAGMA
#pragma interface "GravityEngine.h"
#endif

#include <Gravity_lib.h>
#include <SolarSystem.h>

// {{{RME tool 'OT::Cpp' property 'HeaderPreface'
// {{{USR

// }}}USR
// }}}RME

class GravityEngine
{
public:
	// {{{RME tool 'OT::Cpp' property 'PublicDeclarations'
	// {{{USR

	// }}}USR
	// }}}RME

protected:
	// {{{RME tool 'OT::Cpp' property 'ProtectedDeclarations'
	// {{{USR

	// }}}USR
	// }}}RME

private:
	// {{{RME tool 'OT::Cpp' property 'PrivateDeclarations'
	// {{{USR

	// }}}USR
	// }}}RME
	// {{{RME classAttribute 'm_G'
	double m_G;
	// }}}RME
	// {{{RME classAttribute 'm_system'
	SolarSystem m_system;
	// }}}RME

public:
	// {{{RME tool 'OT::Cpp' property 'GenerateDestructor'
	virtual ~GravityEngine( void );
	// }}}RME
	// {{{RME tool 'OT::Cpp' property 'GenerateCopyConstructor'
	GravityEngine( const GravityEngine & rtg_arg );
	// }}}RME
	// {{{RME tool 'OT::Cpp' property 'GenerateAssignmentOperator'
	GravityEngine & operator=( const GravityEngine & rtg_arg );
	// }}}RME
	// {{{RME operation 'GravityEngine()'
	GravityEngine( void );
	// }}}RME
	// {{{RME operation 'run(uint32_t,double)'
	void run( uint32_t numIterations = 1, double tickDuration = 1 );
	// }}}RME
};

// {{{RME tool 'OT::Cpp' property 'HeaderEnding'
// {{{USR

// }}}USR
// }}}RME

#endif /* GravityEngine_H */

// }}}RME

// {{{RME classifier 'Logical View::Gravity::InitMain::InitMain'

#ifndef InitMain_H
#define InitMain_H

#ifdef PRAGMA
#pragma interface "InitMain.h"
#endif

#include <Gravity_lib.h>
class GravityEngine;

// {{{RME tool 'OT::Cpp' property 'HeaderPreface'
// {{{USR
extern "C" void SoftwareInitialize();
extern "C" unsigned int set_up_mmu(void);

// }}}USR
// }}}RME

class InitMain
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

public:
	// {{{RME tool 'OT::Cpp' property 'GenerateDefaultConstructor'
	InitMain( void );
	// }}}RME
	// {{{RME tool 'OT::Cpp' property 'GenerateDestructor'
	virtual ~InitMain( void );
	// }}}RME
	// {{{RME tool 'OT::Cpp' property 'GenerateCopyConstructor'
	InitMain( const InitMain & rtg_arg );
	// }}}RME
	// {{{RME tool 'OT::Cpp' property 'GenerateAssignmentOperator'
	InitMain & operator=( const InitMain & rtg_arg );
	// }}}RME
};

// {{{RME operation 'SoftwareInitialize()'
void SoftwareInitialize( void );
// }}}RME

// {{{RME tool 'OT::Cpp' property 'HeaderEnding'
// {{{USR

// }}}USR
// }}}RME

#endif /* InitMain_H */

// }}}RME

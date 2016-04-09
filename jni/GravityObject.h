// {{{RME classifier 'Logical View::Gravity::GravityObject::GravityObject'

#ifndef GravityObject_H
#define GravityObject_H

#ifdef PRAGMA
#pragma interface "GravityObject.h"
#endif

#include <Gravity_lib.h>
#include <Vector.h>

// {{{RME tool 'OT::Cpp' property 'HeaderPreface'
// {{{USR

// }}}USR
// }}}RME

// {{{RME classAttribute 'MAX_NAME_LENGTH'
#define MAX_NAME_LENGTH 16
// }}}RME

// {{{RME classAttribute 'RECT'
#define RECT 0
// }}}RME

// {{{RME classAttribute 'POLAR'
#define POLAR 1
// }}}RME

class GravityObject
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
	// {{{RME classAttribute 'm_name'
	char m_name[ MAX_NAME_LENGTH ];
	// }}}RME
	// {{{RME classAttribute 'm_mass'
	double m_mass;
	// }}}RME
	// {{{RME classAttribute 'm_pos'
	Vector m_pos;
	// }}}RME
	// {{{RME classAttribute 'm_vel'
	Vector m_vel;
	// }}}RME
	// {{{RME classAttribute 'm_force'
	Vector m_force;
	// }}}RME
	// {{{RME classAttribute 'm_max_sol_dist'
	double m_max_sol_dist;
	// }}}RME
	// {{{RME classAttribute 'm_min_sol_dist'
	double m_min_sol_dist;
	// }}}RME
	// {{{RME classAttribute 'm_max_sol_speed'
	double m_max_sol_speed;
	// }}}RME
	// {{{RME classAttribute 'm_min_sol_speed'
	double m_min_sol_speed;
	// }}}RME
	// {{{RME tool 'OT::Cpp' property 'GenerateDefaultConstructor'
	GravityObject( void );
	// }}}RME
	// {{{RME tool 'OT::Cpp' property 'GenerateDestructor'
	virtual ~GravityObject( void );
	// }}}RME
	// {{{RME tool 'OT::Cpp' property 'GenerateCopyConstructor'
	GravityObject( const GravityObject & rtg_arg );
	// }}}RME
	// {{{RME tool 'OT::Cpp' property 'GenerateAssignmentOperator'
	GravityObject & operator=( const GravityObject & rtg_arg );
	// }}}RME
	// {{{RME operation 'GravityObject(char *,double,uint8_t,double,double,double,double,double,double,double,GravityObject *)'
	GravityObject( char * name, double mass, uint8_t type, double pos_1, double pos_2, double pos_3, double vel_1, double vel_2, double vel_3, double angle = 0, GravityObject * baseObjectPtr = 0 );
	// }}}RME
	// {{{RME operation 'dump()'
	void dump( void );
	// }}}RME
	// {{{RME operation 'dumpVerbose()'
	void dumpVerbose( void );
	// }}}RME
	// {{{RME operation 'GravityObject(char *,double,double,double,double,double,double,GravityObject *)'
	GravityObject( char * name, double mass, double radialDistance, double tangentialVelocity, double inclination, double omegaArgOfPerigee, double capOmegaLongOfAscNode, GravityObject * baseObjectPtr = 0 );
	// }}}RME
};

// {{{RME tool 'OT::Cpp' property 'HeaderEnding'
// {{{USR

// }}}USR
// }}}RME

#endif /* GravityObject_H */

// }}}RME

// {{{RME classifier 'Logical View::Gravity::SolarSystem::SolarSystem'

#ifndef SolarSystem_H
#define SolarSystem_H

#ifdef PRAGMA
#pragma interface "SolarSystem.h"
#endif

#include <Gravity_lib.h>
class GravityObject;
class Vector;

// {{{RME tool 'OT::Cpp' property 'HeaderPreface'
// {{{USR

// }}}USR
// }}}RME

// {{{RME classAttribute 'MAX_OBJECTS'
#define MAX_OBJECTS 100
// }}}RME

// {{{RME classAttribute 'GRAVITATIONAL_CONSTANT'
#define GRAVITATIONAL_CONSTANT ((double)(6.67E-11) )
// }}}RME

class SolarSystem
{
public:
	uint32_t m_numObjects;

protected:

private:

protected:

private:
	// {{{RME classAttribute 'm_objects'
	GravityObject * m_objects[ MAX_OBJECTS ];
	// }}}RME

public:
	// {{{RME tool 'OT::Cpp' property 'GenerateDestructor'
	virtual ~SolarSystem( void );
	// }}}RME
	// {{{RME tool 'OT::Cpp' property 'GenerateCopyConstructor'
	SolarSystem( const SolarSystem & rtg_arg );
	// }}}RME
	// {{{RME tool 'OT::Cpp' property 'GenerateAssignmentOperator'
	SolarSystem & operator=( const SolarSystem & rtg_arg );
	// }}}RME
	// {{{RME operation 'SolarSystem()'
	SolarSystem( void );
	// }}}RME
	// {{{RME operation 'dumpSystem()'
	void dumpSystem( void );
	// }}}RME

protected:
	void computeNetForces( void );
	void resetNetForces( void );
	void computeNewVelocity( double timeInterval );
	void computeNewPosition( double timeInterval );
	void resetNetMomentum( void );

public:
	void dumpSystemVerbose( void );
	void processTimeInterval( double tickDuration = 1 );

protected:
	void computeInteraction( GravityObject * object_1, GravityObject * object_2 );

public:
	void getGravityObjectInfo( unsigned int objectIndex, char * * namePtr = 0, unsigned int * nameLengthPtr = 0, long * colorPtr = 0 );
	unsigned int getNumObjects( void );
	void getGravityObjectPosition( unsigned int objectIndex, Vector * vectorPtr );
	double getGravityObjectX( unsigned int objectIndex );
	double getGravityObjectY( unsigned int objectIndex );
	double getGravityObjectZ( unsigned int objectIndex );

};


#endif /* SolarSystem_H */

// }}}RME

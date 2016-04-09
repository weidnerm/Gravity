// {{{RME classifier 'Logical View::Gravity::GravityObject::Vector'

#ifndef Vector_H
#define Vector_H

#ifdef PRAGMA
#pragma interface "Vector.h"
#endif

#include <Gravity_lib.h>

// {{{RME tool 'OT::Cpp' property 'HeaderPreface'
// {{{USR
#define DEGS_TO_RADIANS(x) (x*2*3.141592653589/360)

// }}}USR
// }}}RME

class Vector
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
	// {{{RME classAttribute 'z'
	double z;
	// }}}RME
	// {{{RME classAttribute 'y'
	double y;
	// }}}RME
	// {{{RME classAttribute 'x'
	double x;
	// }}}RME
	// {{{RME tool 'OT::Cpp' property 'GenerateDestructor'
	virtual ~Vector( void );
	// }}}RME
	// {{{RME tool 'OT::Cpp' property 'GenerateCopyConstructor'
	Vector( const Vector & rtg_arg );
	// }}}RME
	// {{{RME tool 'OT::Cpp' property 'GenerateAssignmentOperator'
	Vector & operator=( const Vector & rtg_arg );
	// }}}RME
	// {{{RME operation 'set(double,double,double)'
	void set( double newX = 0, double newY = 0, double newZ = 0 );
	// }}}RME
	// {{{RME operation 'reset()'
	void reset( void );
	// }}}RME
	// {{{RME operation 'add(Vector *)'
	void add( Vector * vector );
	// }}}RME
	// {{{RME operation 'subtract(Vector *)'
	void subtract( Vector * vector );
	// }}}RME
	// {{{RME operation 'getMagnitude()'
	double getMagnitude( void );
	// }}}RME
	// {{{RME operation 'multiply(double)'
	void multiply( double scalar_value );
	// }}}RME
	// {{{RME operation 'divide(double)'
	void divide( double scalar_value );
	// }}}RME
	// {{{RME operation 'set(Vector *)'
	void set( Vector * vector );
	// }}}RME
	// {{{RME operation 'Vector()'
	Vector( void );
	// }}}RME
	// {{{RME operation 'dump()'
	void dump( void );
	// }}}RME
	// {{{RME operation 'dump(double)'
	void dump( double normalizeFactor );
	// }}}RME
	// {{{RME operation 'rotateAboutZ(double)'
	void rotateAboutZ( double angle );
	// }}}RME
	// {{{RME operation 'rotateAboutX(double)'
	void rotateAboutX( double angle );
	// }}}RME
};

// {{{RME tool 'OT::Cpp' property 'HeaderEnding'
// {{{USR

// }}}USR
// }}}RME

#endif /* Vector_H */

// }}}RME

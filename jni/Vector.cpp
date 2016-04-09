
#if defined( PRAGMA ) && ! defined( PRAGMA_IMPLEMENTED )
#pragma implementation "Vector.h"
#endif

#include <Gravity_lib.h>
#include <Vector.h>

#include "math.h"
#include "stdio.h"


Vector::~Vector( void )
{
}

Vector::Vector( const Vector & rtg_arg )
	: z( rtg_arg.z )
	, y( rtg_arg.y )
	, x( rtg_arg.x )
{
}

Vector & Vector::operator=( const Vector & rtg_arg )
{
	if( this != &rtg_arg )
	{
		z = rtg_arg.z;
		y = rtg_arg.y;
		x = rtg_arg.x;
	}
	return *this;
}

void Vector::set( double newX, double newY, double newZ )
{
	x = newX;
	y = newY;
	z = newZ;
}

void Vector::reset( void )
{
	x = 0;
	y = 0;
	z = 0;

}

void Vector::add( Vector * vector )
{
	x += vector->x;
	y += vector->y;
	z += vector->z;

}

void Vector::subtract( Vector * vector )
{
	x = x - (vector->x);
	y = y - (vector->y);
	z = z - (vector->z);

}

double Vector::getMagnitude( void )
{
	double length;

	length = sqrt(x*x + y*y + z*z );

	return length;

}

void Vector::multiply( double scalar_value )
{
	x *= scalar_value;
	y *= scalar_value;
	z *= scalar_value;

}

void Vector::divide( double scalar_value )
{
	x /= scalar_value;
	y /= scalar_value;
	z /= scalar_value;

}

void Vector::set( Vector * vector )
{
	x = vector->x;
	y = vector->y;
	z = vector->z;

}

Vector::Vector( void )
{
	// {{{USR
	x = 0;
	y = 0;
	z = 0;
}

void Vector::dump( void )
{
	printf("( %g, %g, %g )  Mag=%g\n",x,y,z,getMagnitude() );
}

void Vector::dump( double normalizeFactor )
{
	printf("( %g, %g, %g )  Mag=%g\n",x/normalizeFactor,y/normalizeFactor,z/normalizeFactor,getMagnitude()/normalizeFactor );
}

void Vector::rotateAboutZ( double angle )
{
	double old_x = x;
	double old_y = y;

	double new_x;
	double new_y;

	// perform the rotation into the new coordinate system.
	new_x = old_x*cos(DEGS_TO_RADIANS(angle)) - old_y*sin(DEGS_TO_RADIANS(angle));
	new_y = old_x*sin(DEGS_TO_RADIANS(angle)) + old_y*cos(DEGS_TO_RADIANS(angle));

	// copy the results back to the vector.
	x = new_x;
	y = new_y;


}

void Vector::rotateAboutX( double angle )
{
	double old_x = y;
	double old_y = z;

	double new_x;
	double new_y;

	// perform the rotation into the new coordinate system.
	new_x = old_x*cos(DEGS_TO_RADIANS(angle)) - old_y*sin(DEGS_TO_RADIANS(angle));
	new_y = old_x*sin(DEGS_TO_RADIANS(angle)) + old_y*cos(DEGS_TO_RADIANS(angle));

	// copy the results back to the vector.
	y = new_x;
	z = new_y;


}

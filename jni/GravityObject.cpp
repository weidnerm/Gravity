// {{{RME classifier 'Logical View::Gravity::GravityObject::GravityObject'

#if defined( PRAGMA ) && ! defined( PRAGMA_IMPLEMENTED )
#pragma implementation "GravityObject.h"
#endif

#include <Gravity_lib.h>
#include <GravityObject.h>

// {{{RME tool 'OT::Cpp' property 'ImplementationPreface'
// {{{USR
#include "string.h"
#include "stdio.h"
#include "math.h"

// }}}USR
// }}}RME

// {{{RME tool 'OT::Cpp' property 'GenerateDefaultConstructor'
GravityObject::GravityObject( void )
	: m_mass( 0 )
{
}
// }}}RME

// {{{RME tool 'OT::Cpp' property 'GenerateDestructor'
GravityObject::~GravityObject( void )
{
}
// }}}RME

// {{{RME tool 'OT::Cpp' property 'GenerateCopyConstructor'
GravityObject::GravityObject( const GravityObject & rtg_arg )
	: m_mass( rtg_arg.m_mass )
	, m_pos( rtg_arg.m_pos )
	, m_vel( rtg_arg.m_vel )
	, m_force( rtg_arg.m_force )
	, m_max_sol_dist( rtg_arg.m_max_sol_dist )
	, m_min_sol_dist( rtg_arg.m_min_sol_dist )
	, m_max_sol_speed( rtg_arg.m_max_sol_speed )
	, m_min_sol_speed( rtg_arg.m_min_sol_speed )
{
	for( int rtg_m_name_index = (MAX_NAME_LENGTH); --rtg_m_name_index >= 0; )
		m_name[ rtg_m_name_index ] = rtg_arg.m_name[ rtg_m_name_index ];
}
// }}}RME

// {{{RME tool 'OT::Cpp' property 'GenerateAssignmentOperator'
GravityObject & GravityObject::operator=( const GravityObject & rtg_arg )
{
	if( this != &rtg_arg )
	{
		for( int rtg_m_name_index = (MAX_NAME_LENGTH); --rtg_m_name_index >= 0; )
			m_name[ rtg_m_name_index ] = rtg_arg.m_name[ rtg_m_name_index ];
		m_mass = rtg_arg.m_mass;
		m_pos = rtg_arg.m_pos;
		m_vel = rtg_arg.m_vel;
		m_force = rtg_arg.m_force;
		m_max_sol_dist = rtg_arg.m_max_sol_dist;
		m_min_sol_dist = rtg_arg.m_min_sol_dist;
		m_max_sol_speed = rtg_arg.m_max_sol_speed;
		m_min_sol_speed = rtg_arg.m_min_sol_speed;
	}
	return *this;
}
// }}}RME

// {{{RME operation 'GravityObject(char *,double,uint8_t,double,double,double,double,double,double,double,GravityObject *)'
GravityObject::GravityObject( char * name, double mass, uint8_t type, double pos_1, double pos_2, double pos_3, double vel_1, double vel_2, double vel_3, double angle, GravityObject * baseObjectPtr )
	// {{{RME tool 'OT::Cpp' property 'ConstructorInitializer'
	// {{{USR

	// }}}USR
	// }}}RME
{
	// {{{USR

	// Save the name.
	strncpy(m_name, name, MAX_NAME_LENGTH );

	m_mass = mass;

	m_pos.x = 0;
	m_pos.y = 0;
	m_pos.z = 0;

	m_vel.x = 0;
	m_vel.y = 0;
	m_vel.z = 0;

	if ( type == RECT )
	{
	   // If we are specified relative to another object, then add its vector to ours.
	   if ( baseObjectPtr != 0 )
	   {
	      m_pos.x += baseObjectPtr->m_pos.x;
	      m_pos.y += baseObjectPtr->m_pos.y;
	      m_pos.z += baseObjectPtr->m_pos.z;

	      m_vel.x += baseObjectPtr->m_vel.x;
	      m_vel.y += baseObjectPtr->m_vel.y;
	      m_vel.z += baseObjectPtr->m_vel.z;
	   }
	      
	   m_pos.x += pos_1*cos(DEGS_TO_RADIANS( angle )) ;
	   m_pos.y += pos_2;
	   m_pos.z += pos_1*sin(DEGS_TO_RADIANS( angle ));

	   m_vel.x += vel_1;
	   m_vel.y += vel_2;
	   m_vel.z += vel_3;
	}
	else if ( type == POLAR )
	{
	   // If we are specified relative to another object, then add its vector to ours.
	   if ( baseObjectPtr != 0 )
	   {
	      m_pos.x += baseObjectPtr->m_pos.x;
	      m_pos.y += baseObjectPtr->m_pos.y;
	      m_pos.z += 0;

	      m_vel.x += baseObjectPtr->m_vel.x;
	      m_vel.y += baseObjectPtr->m_vel.y;
	      m_vel.z += baseObjectPtr->m_vel.z;
	   }

	   // pos1 = r in xy plane.
	   // pos2 = angle from x axis of point.
	   // pos3 = unused.

	   m_pos.x += pos_1*cos(DEGS_TO_RADIANS( pos_2 )) ;
	   m_pos.y += pos_1*sin(DEGS_TO_RADIANS( pos_2 ));
	   m_pos.z += 0;

	   m_vel.x = vel_1;
	   m_vel.y = vel_2;
	   m_vel.z = vel_3;
	}

	m_min_sol_speed = 9.99E+99;
	m_min_sol_dist  = 9.99E+99;
	m_max_sol_speed = 0;
	m_max_sol_dist  = 0;



	// }}}USR
}
// }}}RME

// {{{RME operation 'dump()'
void GravityObject::dump( void )
{
	// {{{USR
#define ONE_AU (149.6E+9) //  meters

	m_pos.dump( ONE_AU );



	// }}}USR
}
// }}}RME

// {{{RME operation 'dumpVerbose()'
void GravityObject::dumpVerbose( void )
{
	// {{{USR
#define ONE_AU (149.6E+9) //  meters
#define ONE_AU_VEL (29.79e+3)  // meters/sec

	printf("%s   mass = %g kg\n",m_name, m_mass);
	printf("   pos AU ");
	m_pos.dump(ONE_AU);
	printf("   vel norm ");
	m_vel.dump();
	printf("   force norm ");
	m_force.dump();
	printf("   m_min_sol_speed=%g    m_max_sol_speed=%g    m_min_sol_dist=%g   m_max_sol_dist=%g\n",m_min_sol_speed,m_max_sol_speed,m_min_sol_dist/ONE_AU,m_max_sol_dist/ONE_AU);

	// }}}USR
}
// }}}RME

// {{{RME operation 'GravityObject(char *,double,double,double,double,double,double,GravityObject *)'
GravityObject::GravityObject( char * name, double mass, double radialDistance, double tangentialVelocity, double inclination, double omegaArgOfPerigee, double capOmegaLongOfAscNode, GravityObject * baseObjectPtr )
	// {{{RME tool 'OT::Cpp' property 'ConstructorInitializer'
	// {{{USR

	// }}}USR
	// }}}RME
{
	// {{{USR

	double xpos,ypos,zpos;
	double xpos1,ypos1,zpos1;
	double xpos2,ypos2,zpos2;
	double xvel,yvel,zvel;
	double xvel1,yvel1,zvel1;
	double xvel2,yvel2,zvel2;


	// Save the name.
	strncpy(m_name, name, MAX_NAME_LENGTH );

	m_mass = mass;

	m_pos.x = radialDistance;
	m_pos.y = 0;
	m_pos.z = 0;

	m_vel.x = 0;
	m_vel.y = tangentialVelocity;
	m_vel.z = 0;

	// rotate the position and velocity vectors according to the argument of the perigee in the plane of rotation (initially the plane of ecliptic too).
	m_pos.rotateAboutZ( omegaArgOfPerigee );
	m_vel.rotateAboutZ( omegaArgOfPerigee );


	// rotate the system along the x axis by the inclination angle.
	m_pos.rotateAboutX( -1*inclination );
	m_vel.rotateAboutX( -1*inclination );

	// rotate the system about the Z axis to accommodate the longitude of node of ascention. 
	// (i.e. rotate the line of intersection of ecliptic and plane of orbit )
	m_pos.rotateAboutZ( capOmegaLongOfAscNode );
	m_vel.rotateAboutZ( capOmegaLongOfAscNode );


	// If we are specified relative to another object, then add its vector to ours.
	if ( baseObjectPtr != 0 )
	{
	   m_pos.x += baseObjectPtr->m_pos.x;
	   m_pos.y += baseObjectPtr->m_pos.y;
	   m_pos.z += baseObjectPtr->m_pos.z;

	   m_vel.x += baseObjectPtr->m_vel.x;
	   m_vel.y += baseObjectPtr->m_vel.y;
	   m_vel.z += baseObjectPtr->m_vel.z;
	}

	m_min_sol_speed = 9.99E+99;
	m_min_sol_dist  = 9.99E+99;
	m_max_sol_speed = 0;
	m_max_sol_dist  = 0;



	// }}}USR
}
// }}}RME

// {{{RME tool 'OT::Cpp' property 'ImplementationEnding'
// {{{USR

// }}}USR
// }}}RME

// }}}RME

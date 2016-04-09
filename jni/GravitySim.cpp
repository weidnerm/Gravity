/*******************************************************************************
*
*                         COPYRIGHT 2011 MOTOROLA
*                           ALL RIGHTS RESERVED.
*            MOTOROLA CONFIDENTIAL PROPRIETARY WHEN FILLED OUT
*
********************************************************************************
*
*   FILE NAME       : GravitySim.cpp
*
*          This file will serve as the outside interface to a gravity system simulation
*   FUNCTION NAME(S): ...
*
*******************************************************************************/

/*-------------------------- HEADER FILE INCLUDES ----------------------------*/
#include <string.h>
#include <jni.h>
#include <Gravity_lib.h>
#include "GravitySim.h"
#include "SolarSystem.h"

/*--------------------------------- CONSTANTS --------------------------------*/

/*-------------------------------- STATIC DATA -------------------------------*/

/*-------------------------------- GLOBAL DATA -------------------------------*/
SolarSystem mySolarSystem;

/*-------------------------------- GLOBAL FUNCTIONS --------------------------*/

/*-------------------------------- METHODS -----------------------------------*/

extern "C" 
{
   JNIEXPORT jstring Java_com_example_Gravity_Gravity_getGravityObjectName( JNIEnv* env, jobject obj, jint index );
   JNIEXPORT void    Java_com_example_Gravity_Gravity_computeNewPositions( JNIEnv* env, jobject obj, jint numIterationsPerDisplayPoint, jdouble computationTimeInterval);
   JNIEXPORT jdouble Java_com_example_Gravity_Gravity_getGravityObjectPosX( JNIEnv* env, jobject obj, jint index );
   JNIEXPORT jdouble Java_com_example_Gravity_Gravity_getGravityObjectPosY( JNIEnv* env, jobject obj, jint index );
   JNIEXPORT jdouble Java_com_example_Gravity_Gravity_getGravityObjectPosZ( JNIEnv* env, jobject obj, jint index );
   JNIEXPORT jint    Java_com_example_Gravity_Gravity_getGravityNumObjects( JNIEnv* env, jobject obj );
};


/*******************************************************************************
*
*   FUNCTION NAME:  getGravityObjectName
*
*------------------------ DETAILED FUNCTION DESCRIPTION ------------------------
*
*   Gets the name text for the object from the gravity system.
*
*******************************************************************************/
JNIEXPORT jstring Java_com_example_Gravity_Gravity_getGravityObjectName( JNIEnv* env, jobject obj, jint index )
{
   char * nameText = (char*)"Unknown";
   uint32_t textLength;
   uint32_t numObjects = mySolarSystem.getNumObjects();
   
   if ( ( index < numObjects ) && ( index >= 0 ) )
   {
   	mySolarSystem.getGravityObjectInfo( index, &nameText, &textLength );
   }

   return env->NewStringUTF(nameText);
}

/*******************************************************************************
*
*   FUNCTION NAME:  computeNewPositions
*
*------------------------ DETAILED FUNCTION DESCRIPTION ------------------------
*
*   numIterationsPerDisplayPoint - number of iterations to compute.
*
*   computationTimeInterval      - time in seconds per computation interval
*
*
*   Computes a number of iterations of the gravitational interactions on the full system.
*
*******************************************************************************/
JNIEXPORT void Java_com_example_Gravity_Gravity_computeNewPositions( JNIEnv* env, jobject obj, jint numIterationsPerDisplayPoint, jdouble computationTimeInterval)
{
	uint32_t index;

	for(index=0;index<numIterationsPerDisplayPoint ; index++)
	{
		mySolarSystem.processTimeInterval(computationTimeInterval);
	}
}


/*******************************************************************************
*
*   FUNCTION NAME:  getGravityNumObjects
*
*------------------------ DETAILED FUNCTION DESCRIPTION ------------------------
*
*
*******************************************************************************/
JNIEXPORT jint Java_com_example_Gravity_Gravity_getGravityNumObjects( JNIEnv* env, jobject obj )
{
   double retVal;
   
   retVal = mySolarSystem.getNumObjects();

   return retVal;
}


/*******************************************************************************
*
*   FUNCTION NAME:  getGravityObjectPosX
*
*------------------------ DETAILED FUNCTION DESCRIPTION ------------------------
*
*
*******************************************************************************/
JNIEXPORT jdouble Java_com_example_Gravity_Gravity_getGravityObjectPosX( JNIEnv* env, jobject obj, jint index )
{
   double retVal;
   
   retVal = mySolarSystem.getGravityObjectX(index);

   return retVal;
}
/*******************************************************************************
*
*   FUNCTION NAME:  getGravityObjectPosY
*
*------------------------ DETAILED FUNCTION DESCRIPTION ------------------------
*
*
*******************************************************************************/
JNIEXPORT jdouble Java_com_example_Gravity_Gravity_getGravityObjectPosY( JNIEnv* env, jobject obj, jint index )
{
   double retVal;
   
   retVal = mySolarSystem.getGravityObjectY(index);

   return retVal;
}
/*******************************************************************************
*
*   FUNCTION NAME:  getGravityObjectPosZ
*
*------------------------ DETAILED FUNCTION DESCRIPTION ------------------------
*
*
*******************************************************************************/
JNIEXPORT jdouble Java_com_example_Gravity_Gravity_getGravityObjectPosZ( JNIEnv* env, jobject obj, jint index )
{
   double retVal;
   
   retVal = mySolarSystem.getGravityObjectZ(index);

   return retVal;
}



/*------------------------------- HELPER FUNCTIONS ---------------------------*/

/***************************************************************
*  REVISION HISTORY
*
****************************************************************
* Date     Person        Change
* -----------------------------------------------------------------------------
*
****************************************************************
*  END OF HISTORY TEMPLATE
****************************************************************/

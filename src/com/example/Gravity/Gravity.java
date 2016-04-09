package com.example.Gravity;

import android.app.Activity;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.DashPathEffect;
import android.graphics.Paint;
import android.graphics.RectF;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.View;

/**
 * Main Activity class for the Gravity Simulator7
 * @author weidnerm
 * 
 */
public class Gravity extends Activity {

	private SensorManager mSensorManager = null;
	private GravityView mGravityView;
	private DisplayMetrics mDisplayMetrics;

	/**
	 * Native call that returns the name string for each object
	 * 
	 * @param object          Index of the object
	 * 
	 * @return string         representing the textual name of the object.
	 */
	public native String getGravityObjectName(int object);

	/**
	 * Native call that runs the simulation calculations for the given time
	 * interval and number of iterations of that time interval.
	 * 
	 * @param numIterationsPerDisplayPoint Number of iterations of the computationTimeInterval
	 *                                     calculation.
	 * 
	 * @param computationTimeInterval Time interval in seconds of each calculation
	 *                                iteration.
	 */
	public native void computeNewPositions(int numIterationsPerDisplayPoint,
			double computationTimeInterval);
	/**
	 * Native call that returns the Coordinate of the indexed object
	 * @param index  Index of the object whos coordinate is requested.
	 * @return Coordinate being requested.
	 */
	public native double getGravityObjectPosX(int index);
	/**
	 * Native call that returns the Coordinate of the indexed object
	 * @param index  Index of the object whos coordinate is requested.
	 * @return Coordinate being requested.
	 */	
	public native double getGravityObjectPosY(int index);
	/**
	 * Native call that returns the Coordinate of the indexed object
	 * @param index  Index of the object whos coordinate is requested.
	 * @return Coordinate being requested.
	 */	
	public native double getGravityObjectPosZ(int index);
	/**
	 * Native call that returns the number of objects present in the simulation.
	 * @return Number of objects present in the simulation.
	 */
	public native int getGravityNumObjects();

	/**
	 * Constant representing the diameter in pixels of the planet object circles
	 * for the display.
	 */
	final public int PLANETSIZE = 5;
	/**
	 * Number of history points to maintain in the simulation.  Determines
	 * the length of the history tail.
	 */
	final public int MAX_OBJECT_HISTORY = 10;
	/**
	 * One Astronomical unit represented in meters.
	 */
	final public double ONE_AU_METERS = 149.6E+9;// meters
	/**
	 * Scale factor that is used to scale the actual distances in meters to the
	 * display coordinates in pixels.
	 */
	double mDisplayScale = ONE_AU_METERS / 40; // start with xx pixels per AU
	/**
	 * History of display coordinates for all the gravity objects.
	 */
	DisplayPoint myGravityObjectCoordHistory[][];
	/**
	 * Array of points representing the point of the vertical projection onto the 
	 * plane of the ecliptic.
	 */
	DisplayPoint myGravityObjectShadow[];
	/**
	 * Positions of the gravity objects in 3D space with history.
	 */
	VectorPoint myGravityObjectVectorHistory[][];
	/**
	 * Array of names of gravity objects.
	 */
	String myGravityObjectNames[];
	/**
	 * Tracks the current number of history entries present in the arrays
	 */
	int myGravityObjectCoordHistoryDepth = 0;
	/**
	 * Number of gravity objects tracked by the simulation.
	 */
	int mNumGravityObjects = 0;
	/**
	 * Tracks the screen center.  Used as an offset for the display coordinates
	 * to center the simulation.
	 */
	DisplayPoint mDisplayOriginOffset;
	/**
	 * Index of the object to be displayed at the screen center.
	 */
	int mCenterObjectIndex = -1; // -1 = center at 0,0
	/**
	 * Time interval in seconds of the computation iteration time.
	 * This time is the duration of the iteration associated with 
	 * computing new speed change based on acceleration, and distance
	 * change based on speed.
	 */
	final public double COMPUTATION_TIME_INTERVAL = (1*60*60);
	/**
	 * Number of seconds in a day.
	 */
	final public double ONE_DAY_TIME_INTERVAL = (60*60*24);
	/**
	 * Number of iterations of the calculation per display update.
	 */
	int mNumIterationsPerDisplayPoint = 24;
    /**
     * Count of the number of times that the current GravityView.OnDraw method
     * is called.
     */
    int mOnDrawCount = 0;
    long mFirstOnDrawTimestamp = 0;
	double mViewingAngle = 45; // degrees
	int mTouchCount = 0;
    
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

        // Get an instance of the SensorManager
        mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);

		/*
		 * Create a TextView and set its content. the text is retrieved by
		 * calling a native function.
		 */
		initGravityEngine(); // init the App side of the simulation.

		// instantiate our simulation view and set it as the activity's content
		mGravityView = new GravityView(this);
		setContentView(mGravityView);
		mDisplayOriginOffset = new DisplayPoint();
		
	}

	
	

	class GravityView extends View implements SensorEventListener {
		private Sensor mAccelerometer = null;
		private RectF mPlanetRect;
		private Paint mPlanetPaint;
		private Paint mShadowPaint;
		private Paint mDistanceScalePaint;
		private Paint mTimeScalePaint;
		private Paint mAnglePaint;
		private Paint mElapsedTimePaint;
        private float mAccelerometerX;
        private float mAccelerometerY;
        private float mAccelerometerZ;
        private float mTouchX;
        private float mTouchY;
        private double mOldDisplayScale = 1.000;
        final int CLICK_DISTANCE_PIXELS=20;
 
		/**
		 * Custom view that is used to represent the gravity simulation graphical
		 * output area.
		 * 
		 * @param context Context for the view.
		 */
		public GravityView(Context context) 
		{
			super(context);
			if ( mSensorManager != null )
			{
				mAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
			}

			mDisplayMetrics = new DisplayMetrics();
			getWindowManager().getDefaultDisplay().getMetrics(mDisplayMetrics);

			mPlanetRect = new RectF();

			// set up paint for the planet object
			mPlanetPaint = new Paint();
			mPlanetPaint.setColor(Color.CYAN);

			mDistanceScalePaint = new Paint();
			mDistanceScalePaint.setColor(Color.YELLOW);

			mTimeScalePaint = new Paint();
			mTimeScalePaint.setColor(Color.YELLOW);

			mAnglePaint = new Paint();
			mAnglePaint.setColor(Color.YELLOW);

			mElapsedTimePaint = new Paint();
			mElapsedTimePaint.setColor(Color.YELLOW);
			
			// set up paint for the dashed line drawn to plane of ecliptic
			mShadowPaint = new Paint();
			mShadowPaint.setColor(Color.DKGRAY);
			DashPathEffect dashPath = new DashPathEffect(new float[] { 2, 2 },
					1);
			mShadowPaint.setPathEffect(dashPath);
			mShadowPaint.setStrokeWidth(1);

            if (( mAccelerometer != null) && (mSensorManager != null ))
        	{
            	mSensorManager.registerListener(this, mAccelerometer, SensorManager.SENSOR_DELAY_UI);
        	}
		
		}

		@Override
		protected void onDraw(Canvas canvas) 
		{

			/*
			 * draw the background
			 */

			mDisplayMetrics.widthPixels = getWidth();
			mDisplayMetrics.heightPixels = getHeight();
			
			canvas.drawRGB(0, 0, 0); // clear the screen. all black.

			displayDistanceScale(canvas);
			displayTheViewingAngle(canvas);
			displayTheTimeScale(canvas);
			displayElapsedTime(canvas);

			// Retrieve the size of the drawing area
			mDisplayOriginOffset.x = mDisplayMetrics.widthPixels / 2;
			mDisplayOriginOffset.y = mDisplayMetrics.heightPixels / 2;

//			final int extremeLeft = mDisplayOriginOffset.x -mDisplayMetrics.widthPixels;  
//			final int extremeRight = mDisplayOriginOffset.x + mDisplayMetrics.widthPixels;
//			final int extremeTop = mDisplayOriginOffset.y -mDisplayMetrics.heightPixels;  
//			final int extremeBottom = mDisplayOriginOffset.y + mDisplayMetrics.heightPixels;
			final int extremeLeft = mDisplayOriginOffset.x -mDisplayMetrics.heightPixels;  
			final int extremeRight = mDisplayOriginOffset.x + mDisplayMetrics.heightPixels;
			final int extremeTop = mDisplayOriginOffset.y -mDisplayMetrics.widthPixels;  
			final int extremeBottom = mDisplayOriginOffset.y + mDisplayMetrics.widthPixels;

			if (mCenterObjectIndex != -1) 
			{
				mDisplayOriginOffset.x -= myGravityObjectCoordHistory[mCenterObjectIndex][0].x;
				mDisplayOriginOffset.y -= myGravityObjectCoordHistory[mCenterObjectIndex][0].y;
			}

			int objectIndex;

			// for(objectIndex=0 ; objectIndex<numObjects ; objectIndex++)
			for (objectIndex = mNumGravityObjects - 1; objectIndex >= 0; objectIndex--) 
			{
				
				float objectX = mDisplayOriginOffset.x + myGravityObjectCoordHistory[objectIndex][0].x;
				float objectY = mDisplayOriginOffset.y + myGravityObjectCoordHistory[objectIndex][0].y;
				
				if ( ( extremeLeft < objectY ) &&
						( objectY < extremeRight ) &&
						( extremeTop < objectX ) &&
						( objectX < extremeBottom ) )        // only try to draw objects that are close to being on the screen.
				{
					// Draw the textual name of the object.
					canvas.drawText(myGravityObjectNames[objectIndex],objectX + 2, objectY - 2, mPlanetPaint);
	
					// Draw the Planet
					mPlanetRect.left = objectX - 2;
					mPlanetRect.top = objectY - 2;
					mPlanetRect.right = objectX + 2;
					mPlanetRect.bottom = objectY + 2;
					canvas.drawOval(mPlanetRect, mPlanetPaint);
	
					// Draw the history "tail"
					if ( myGravityObjectCoordHistoryDepth >= 2 )
					{
						for(int histIndex=0; histIndex<myGravityObjectCoordHistoryDepth-1 ; histIndex++)
						{
							canvas.drawLine(mDisplayOriginOffset.x + myGravityObjectCoordHistory[objectIndex][histIndex].x,
								            mDisplayOriginOffset.y + myGravityObjectCoordHistory[objectIndex][histIndex].y,
								            mDisplayOriginOffset.x + myGravityObjectCoordHistory[objectIndex][histIndex+1].x,
								            mDisplayOriginOffset.y + myGravityObjectCoordHistory[objectIndex][histIndex+1].y,
								            mPlanetPaint);
						}
					}
	
					// Draw the line to the shadow position.
					{
						float shadowX = mDisplayOriginOffset.x + myGravityObjectShadow[objectIndex].x;
						float shadowY = mDisplayOriginOffset.y + myGravityObjectShadow[objectIndex].y;
						
						if (shadowX > extremeBottom )
						{
							shadowX = extremeBottom;
						}
						else if (shadowX < extremeTop)
						{
							shadowX = extremeTop;
						}
						
						canvas.drawLine(shadowX,shadowY,
								objectX, objectY,
								mShadowPaint);
					}
				}

			}

		   
			// Calculate the new object positions.
			computeNewPositions(mNumIterationsPerDisplayPoint,COMPUTATION_TIME_INTERVAL);
		    // Fetch the calculated data.
		    storeNewSetOfCoords();
		    // Calculate the new display coordinates of objects, trails, and shadows.
		    computeNewDisplayCoords();

		   // and make sure to redraw asap
		    invalidate();
		}

		/**
		 * Display the current viewing angle.
		 * 
		 * @param canvas    The canvas upon which the viewing angle is drawn.
		 */
		private void displayTheViewingAngle(Canvas canvas)
		{
			String angleText = String.format("%3.1f degs", mViewingAngle);

			canvas.drawText(angleText, mDisplayMetrics.widthPixels - 60,
					mDisplayMetrics.heightPixels - 2, mAnglePaint);
		}
		
		/**
		 * Displays the total elapsed time of the simultion
		 * 
		 * @param canvas    The canvas upon which the viewing angle is drawn.
		 */
		private void displayElapsedTime(Canvas canvas)
		{
			String timeText = String.format("%d days", mOnDrawCount);
		
			canvas.drawText(timeText, 10, 10, mElapsedTimePaint);
		}
		
		/**
		 * Displays the distance scale bar with legend.
		 * 
		 * @param canvas    The canvas upon which the viewing angle is drawn.
		 */
		private void displayDistanceScale(Canvas canvas)
		{
			double scaleLength = 1000;  // number of AU
			int displayWidth;
			String scaleText;

			displayWidth = mDisplayMetrics.widthPixels;

			while ( ( (ONE_AU_METERS*scaleLength/mDisplayScale) < ((displayWidth*3 )/100) ) ||
			        ( (ONE_AU_METERS*scaleLength/mDisplayScale) > ((displayWidth*40)/100) ) )
		    {
		        scaleLength /= 10;

				if ( scaleLength < 0.0001 )
				{
					break;
				}
			}

			canvas.drawLine(10, mDisplayMetrics.heightPixels-5, 10, mDisplayMetrics.heightPixels-10, mDistanceScalePaint);
			canvas.drawLine((float)(10+(ONE_AU_METERS*scaleLength/mDisplayScale)), (float)mDisplayMetrics.heightPixels-5,
					(float)(10+(ONE_AU_METERS*scaleLength/mDisplayScale)), (float)mDisplayMetrics.heightPixels-10, mDistanceScalePaint);
			canvas.drawLine(10, (float)mDisplayMetrics.heightPixels-7,
					(float)(10+(ONE_AU_METERS*scaleLength/mDisplayScale)), (float)mDisplayMetrics.heightPixels-7, mDistanceScalePaint);
			
			scaleText = String.format("%f AU", scaleLength);
//			scaleText = String.format("%f AU %f", scaleLength,(float)mDisplayScale);
			
			canvas.drawText(scaleText, (float)(10+(ONE_AU_METERS*scaleLength/mDisplayScale)+10),
					(float)mDisplayMetrics.heightPixels-2, mDistanceScalePaint);
		}



		
		/**
		 * Displays the time scale of the simulation.
		 * 
		 * @param canvas    The canvas upon which the time scale is drawn.
		 */
		public void displayTheTimeScale(Canvas canvas)
		{
			mOnDrawCount += 1;
			long currentTimestamp = System.nanoTime();
			if ( mFirstOnDrawTimestamp == 0)
			{
				mFirstOnDrawTimestamp= currentTimestamp;
			}
			long elapsedTime_msec = (currentTimestamp-mFirstOnDrawTimestamp)/1000000;
			

			int displayWidth;
			String scaleText;
			double tempVal;

			displayWidth = mDisplayMetrics.widthPixels;

			if ( elapsedTime_msec != 0 )
			{
				tempVal = (double)mNumIterationsPerDisplayPoint*mOnDrawCount*COMPUTATION_TIME_INTERVAL/ONE_DAY_TIME_INTERVAL*1000/elapsedTime_msec;				
			}
			else
			{
				tempVal = 0;
			}

			scaleText = String.format("%4.1f days/sec", tempVal);

			canvas.drawText(scaleText, (float)displayWidth-100,
					(float)10, mTimeScalePaint);
		}
		
        @Override
        public void onSensorChanged(SensorEvent event) {
            if (event.sensor.getType() != Sensor.TYPE_ACCELEROMETER)
                return;
            /*
             * record the accelerometer data, the event's timestamp as well as
             * the current time. The latter is needed so we can calculate the
             * "present" time during rendering. In this application, we need to
             * take into account how the screen is rotated with respect to the
             * sensors (which always return data in a coordinate space aligned
             * to with the screen in its native orientation).
             */

//            switch (mDisplay.getRotation())
            switch (Surface.ROTATION_90) 
            {
                case Surface.ROTATION_0:
                    mAccelerometerX = event.values[0];
                    mAccelerometerY = event.values[1];
                    mAccelerometerZ = event.values[2];
                    break;
                case Surface.ROTATION_90:
                    mAccelerometerX = -event.values[1];
                    mAccelerometerY = event.values[0];
                    mAccelerometerZ = event.values[2];
                    break;
                case Surface.ROTATION_180:
                    mAccelerometerX = -event.values[0];
                    mAccelerometerY = -event.values[1];
                    mAccelerometerZ = event.values[2];
                    break;
                case Surface.ROTATION_270:
                    mAccelerometerX = event.values[1];
                    mAccelerometerY = -event.values[0];
                    mAccelerometerZ = event.values[2];
                    break;
            }
            
            mViewingAngle = Math.toDegrees( Math.atan2(mAccelerometerY,mAccelerometerZ) );
        }

        
        
        
        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {
        }

        @Override
        public boolean onTouchEvent(MotionEvent event) {
            float x = event.getX();
            float y = event.getY();
            mTouchCount = event.getPointerCount();
            
            switch (event.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    mTouchX = x;
                    mTouchY = y;
                    mOldDisplayScale = 1;
                    break;
                case MotionEvent.ACTION_MOVE:
                	double distance = getTwoTouchDistance(event);
                	double newScale;
                	distance = Math.floor(distance/10);  // scale it down a bit.
                	if ( distance >= 2 )
                	{
                		newScale = Math.pow(1.1, distance);
                
	                	if ( mOldDisplayScale != 1 )
	            		{
	                		mDisplayScale = mDisplayScale*mOldDisplayScale/ newScale ;
	                		
	                		if (mDisplayScale > 1e+12)
	                		{
	                			mDisplayScale = 1e+12;
	                		}
	                		else if ( mDisplayScale < 1e+6 )
	                		{
	                			mDisplayScale = 1e+6;	                			
	                		}
	            		}
	                	mOldDisplayScale = newScale;
                	}
                    break;
                case MotionEvent.ACTION_UP:
                    mOldDisplayScale = 1;
                	if ( ( Math.abs(mTouchX-x) < CLICK_DISTANCE_PIXELS ) &&
                			( Math.abs(mTouchY-y) < CLICK_DISTANCE_PIXELS ) )
                 	{
                		// We have a release that was near by a press.  its a non-slide tap.
                		handleNewOriginClick(mTouchX,mTouchY);
                	}
                	else if ( (mTouchX-x) > 50  )
                	{
                		mDisplayScale *= 1.5;
                	}
                	else if ( (mTouchX-x) < -50  )
                	{
                		mDisplayScale /= 1.5;
                	}
                    break;
            }
            
            
            return true;
        }
        
        /**
         * Calculates the pixel distance between the multitouch points.
         * 
         * @param ev MotionEvent containing touch information
         * @return Pixel distance between two touch points.
         */
        private double getTwoTouchDistance(MotionEvent ev) 
        {
            final int pointerCount = ev.getPointerCount();
            double distance = 0;

            if ( pointerCount == 2 )
            {
            	double deltaX = ev.getX(0)-ev.getX(1);
            	double deltaY = ev.getY(0)-ev.getY(1);
            	
            	distance = Math.sqrt( deltaX*deltaX + deltaY*deltaY );
            }
            
            return distance;
        }
        
        
        
        /**
         * Handles a quick screen tap to select a new origin
         * @param xPos  Horizontal coordinate of user tap
         * @param yPos  Vertical coordinate of user tap
         */
        private void handleNewOriginClick(float xPos, float yPos)
        {
        	int index;
        	mCenterObjectIndex = -1;
        	for(index=0 ; index<mNumGravityObjects ; index++)
        	{
                if ( ( mDisplayOriginOffset.x+myGravityObjectCoordHistory[index][0].x > xPos-CLICK_DISTANCE_PIXELS ) &&
        			 ( mDisplayOriginOffset.x+myGravityObjectCoordHistory[index][0].x < xPos+CLICK_DISTANCE_PIXELS ) &&
        			 ( mDisplayOriginOffset.y+myGravityObjectCoordHistory[index][0].y > yPos-CLICK_DISTANCE_PIXELS ) &&
        			 ( mDisplayOriginOffset.y+myGravityObjectCoordHistory[index][0].y < yPos+CLICK_DISTANCE_PIXELS ) )
        		{
        			// We're within the click range.  find the first object in the click range.
                	mCenterObjectIndex = index;
        			break;
        		}
        	}
        }

        
	}
	

	
	/**
	 * Initializes the GravityEngine and allocates necessary tracking memory.
	 * The vector and display point history buffers are allocated and
	 * initialized with the initial state of the gravity system.
	 */
	private void initGravityEngine() 
	{
		mNumGravityObjects = getGravityNumObjects();

		// Allocate the pointers to the objects.
		myGravityObjectCoordHistory = new DisplayPoint[mNumGravityObjects][MAX_OBJECT_HISTORY];
		myGravityObjectShadow = new DisplayPoint[mNumGravityObjects];
		myGravityObjectVectorHistory = new VectorPoint[mNumGravityObjects][MAX_OBJECT_HISTORY];
		myGravityObjectNames = new String[mNumGravityObjects];

		// Allocate the objects.
		for (int objectIndex = 0; objectIndex < mNumGravityObjects; objectIndex++) {
			myGravityObjectShadow[objectIndex] = new DisplayPoint();
			myGravityObjectNames[objectIndex] = getGravityObjectName(objectIndex);

			for (int historyIndex = 0; historyIndex < MAX_OBJECT_HISTORY; historyIndex++) {
				myGravityObjectCoordHistory[objectIndex][historyIndex] = new DisplayPoint();
				myGravityObjectVectorHistory[objectIndex][historyIndex] = new VectorPoint();
			}
		}

		storeNewSetOfCoords(); // Populate the objects with good stuff.
		computeNewDisplayCoords();

	}

	/**
	 * Fetches the coordinates from the Gravity Engine via JNI and stores
	 * the coordinates in the myGravityObjectVectorHistory area.
	 */
	private void storeNewSetOfCoords() {
		int index;
		int tempIndex;

		for (index = 0; index < mNumGravityObjects; index++) {
			// Save the historical values.
			for (tempIndex = myGravityObjectCoordHistoryDepth; tempIndex > 0; tempIndex--) {
				myGravityObjectVectorHistory[index][tempIndex].x = myGravityObjectVectorHistory[index][tempIndex - 1].x;
				myGravityObjectVectorHistory[index][tempIndex].y = myGravityObjectVectorHistory[index][tempIndex - 1].y;
				myGravityObjectVectorHistory[index][tempIndex].z = myGravityObjectVectorHistory[index][tempIndex - 1].z;
			}

			// fetch the recent coordinates from the computation engine.
			myGravityObjectVectorHistory[index][0].x = getGravityObjectPosX(index);
			myGravityObjectVectorHistory[index][0].y = getGravityObjectPosY(index);
			myGravityObjectVectorHistory[index][0].z = getGravityObjectPosZ(index);

		}
		if (myGravityObjectCoordHistoryDepth < MAX_OBJECT_HISTORY - 1) {
			myGravityObjectCoordHistoryDepth += 1; // Only advance the history
			// depth till it reaches
			// max.
		}
	}

	/**
	 * converts the physical positions into display coordinates.
	 */
	private void computeNewDisplayCoords() {
		int index;
		double xTemp;
		double yTemp;
		int tempIndex;
		double sinOfAngle = Math.sin(Math.toRadians(mViewingAngle));
		double cosOfAngle = Math.cos(Math.toRadians(mViewingAngle));

		for (index = 0; index < mNumGravityObjects; index++) {
			// Save the historical values.
			for (tempIndex = 0; tempIndex < myGravityObjectCoordHistoryDepth; tempIndex++) {
				// Compute the projected ccordinates based on the viewing angle.
				xTemp = myGravityObjectVectorHistory[index][tempIndex].x;
				yTemp = myGravityObjectVectorHistory[index][tempIndex].y
						* cosOfAngle
						- myGravityObjectVectorHistory[index][tempIndex].z
						* sinOfAngle;

				// Scale it down so that it fits our display.
				xTemp /= mDisplayScale; // One AU is about 149 pixels.
				yTemp /= -mDisplayScale; // One AU is about 149 pixels.

				myGravityObjectCoordHistory[index][tempIndex].x = (int) xTemp;
				myGravityObjectCoordHistory[index][tempIndex].y = (int) yTemp;
			}

			xTemp = myGravityObjectVectorHistory[index][0].x;
			yTemp = myGravityObjectVectorHistory[index][0].y * cosOfAngle;

			xTemp /= mDisplayScale; // One AU is about 149 pixels.
			yTemp /= -mDisplayScale; // One AU is about 149 pixels.

			myGravityObjectShadow[index].x = (int) xTemp;
			myGravityObjectShadow[index].y = (int) yTemp;

		}
	}

	/**
	 * Class that represents a 3D vector in non-polar format using x,y,z.
	 * 
	 * @author weidnerm
	 * 
	 */
	class VectorPoint {
		double x;
		double y;
		double z;
	}

	/**
	 * Class that represents a point on the display using x,y coordinates.
	 */
	class DisplayPoint {
		int x;
		int y;
	}

	static {
		System.loadLibrary("GravityEngine");
	}
	
}
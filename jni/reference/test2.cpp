// test2.cpp : Defines the entry point for the application.
//


#include "stdafx.h"
#define _WIN32_WINDOWS 0x401
#include "winuser.h"
#include "resource.h"
#include "stdio.h"
#include <time.h>
#include "SolarSystem.h"
#include "Vector.h"
//#include "GravityObject.h"
#include "windef.h"
#include "math.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text
char tempString[100];
char tempString2[100];
unsigned int tempVal = 0;


// Solar system stuff.
SolarSystem mySolarSystem;
#define MAX_GRAVITY_OBJECTS 200
#define MAX_OBJECT_HISTORY 10
POINT myGravityObjectCoordHistory[MAX_GRAVITY_OBJECTS][MAX_OBJECT_HISTORY];
POINT myGravityObjectShadow[MAX_GRAVITY_OBJECTS];
Vector myGravityObjectVectorHistory[MAX_GRAVITY_OBJECTS][MAX_OBJECT_HISTORY];
unsigned int myGravityObjectCoordHistoryDepth = 0;
char * myGravityObjectNames[MAX_GRAVITY_OBJECTS];
unsigned int myGravityObjectNamesLength[MAX_GRAVITY_OBJECTS];
double cumulativeTime = 0;
#define MSEC_PER_DISPLAY_UPDATE 50
#define ONE_AU_METERS (149.6E+9) //  meters
double displayScale = 2000000000;
double viewingAngle = 45;  // zero degrees.
#define DEGS_TO_RADIANS(x) (x*2*3.141592653589/360)
uint32_t numIterationsPerDisplayPoint = 32;
#define COMPUTATION_TIME_INTERVAL (1*60*60)
#define ONE_DAY_TIME_INTERVAL (60*60*24)
int32_t centerObjectIndex = -1;
POINT displayOriginOffset;


// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TEST2);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	srand((unsigned)time(NULL));

	return msg.wParam;
}

unsigned int getNumObjects(void)
{
	unsigned int numObjects;

	numObjects = mySolarSystem.getNumObjects();
	if ( numObjects>MAX_GRAVITY_OBJECTS)
	{
		numObjects = MAX_GRAVITY_OBJECTS;
	}

	return(numObjects);
}

void storeNewSetOfCoords(void)
{
	uint32_t index;
	double xTemp;
	double yTemp;
	uint32_t numObjects = getNumObjects();
	uint32_t tempIndex;

	for(index=0; index<numObjects ; index++ )
	{
		// Save the historical values.
		for(tempIndex=myGravityObjectCoordHistoryDepth; tempIndex>0 ; tempIndex-- )
        {
			myGravityObjectVectorHistory[index][tempIndex] = myGravityObjectVectorHistory[index][tempIndex-1];
//			myGravityObjectCoordHistory[index][tempIndex+1] = myGravityObjectCoordHistory[index][tempIndex];
		}

		mySolarSystem.getGravityObjectPosition(index, &myGravityObjectVectorHistory[index][0] );

		xTemp /= displayScale;   // One AU is about 149 pixels.
		yTemp /= -displayScale;   // One AU is about 149 pixels.

		myGravityObjectCoordHistory[index][0].x = (long)xTemp;
		myGravityObjectCoordHistory[index][0].y = (long)yTemp;

	}
	if ( myGravityObjectCoordHistoryDepth < MAX_OBJECT_HISTORY-1 )
	{
		myGravityObjectCoordHistoryDepth += 1; // Only advance the history depth till it reaches max.
    }
}

// converts the physical positions into display coordinates.
void computeNewDisplayCoords(void)
{
	uint32_t index;
	double xTemp;
	double yTemp;
	uint32_t numObjects = getNumObjects();
	uint32_t tempIndex;
	double sinOfAngle = sin( DEGS_TO_RADIANS( viewingAngle ));
	double cosOfAngle = cos( DEGS_TO_RADIANS( viewingAngle ));

	for(index=0; index<numObjects ; index++ )
	{
		// Save the historical values.
		for(tempIndex=0; tempIndex<myGravityObjectCoordHistoryDepth ; tempIndex++ )
        {
			// Compute the projected ccordinates based on the viewing angle.
			xTemp = myGravityObjectVectorHistory[index][tempIndex].x;
			yTemp = myGravityObjectVectorHistory[index][tempIndex].y*cosOfAngle -
                    myGravityObjectVectorHistory[index][tempIndex].z*sinOfAngle;

			// Scale it down so that it fits our display.
			xTemp /= displayScale;   // One AU is about 149 pixels.
			yTemp /= -displayScale;   // One AU is about 149 pixels.

			myGravityObjectCoordHistory[index][tempIndex].x = (long)xTemp;
			myGravityObjectCoordHistory[index][tempIndex].y = (long)yTemp;
		}

		xTemp = myGravityObjectVectorHistory[index][0].x;
		yTemp = myGravityObjectVectorHistory[index][0].y*cosOfAngle;

		xTemp /= displayScale;   // One AU is about 149 pixels.
		yTemp /= -displayScale;   // One AU is about 149 pixels.

		myGravityObjectShadow[index].x = (long)xTemp;
		myGravityObjectShadow[index].y = (long)yTemp;
		
	}
}
void displayTheViewingAngle(HWND hWnd, HDC hDC)
{
	RECT Recto;
	GetClientRect(hWnd, &Recto);
	uint32_t displayWidth;
	char scaleText[20];

	displayWidth = Recto.right - Recto.left;

    sprintf(scaleText, "%3.1f degs", viewingAngle );

	TextOut(hDC, displayWidth-100, 
			     Recto.bottom-15, scaleText, strlen(scaleText) );

}



void displayTheTimeScale(HWND hWnd, HDC hDC)
{
	RECT Recto;
	GetClientRect(hWnd, &Recto);
	uint32_t displayWidth;
	char scaleText[20];
	double tempVal;

	displayWidth = Recto.right - Recto.left;

	tempVal = (double)numIterationsPerDisplayPoint*COMPUTATION_TIME_INTERVAL/ONE_DAY_TIME_INTERVAL*1000/MSEC_PER_DISPLAY_UPDATE;

    sprintf(scaleText, "%4.1f days/sec", tempVal );

	TextOut(hDC, displayWidth-100, 
			     Recto.top+5, scaleText, strlen(scaleText) );

}

void displayTheScale(HWND hWnd, HDC hDC)
{
	RECT Recto;
	double scaleLength = 1000;  // number of AU
	uint32_t displayWidth;
	char scaleText[20];

	// Retrieve the size of the drawing area
	GetClientRect(hWnd, &Recto);

	displayWidth = Recto.right - Recto.left;

	while ( ( (ONE_AU_METERS*scaleLength/displayScale) < ((displayWidth*3 )/100) ) ||
	        ( (ONE_AU_METERS*scaleLength/displayScale) > ((displayWidth*40)/100) ) )
    {
        scaleLength /= 10;

		if ( scaleLength < 0.0001 )
		{
			break;
		}
	}

	MoveToEx(hDC, 10, Recto.bottom-5, NULL);
	LineTo(hDC, 10, Recto.bottom-10);

	MoveToEx(hDC, (unsigned int)(10+(ONE_AU_METERS*scaleLength/displayScale)), Recto.bottom-5, NULL);
	LineTo(hDC, (unsigned int)(10+(ONE_AU_METERS*scaleLength/displayScale)), Recto.bottom-10);

	MoveToEx(hDC, 10, Recto.bottom-7, NULL);
	LineTo(hDC, (unsigned int)(10+(ONE_AU_METERS*scaleLength/displayScale)), Recto.bottom-7);

    sprintf(scaleText, "%f AU", scaleLength );

	TextOut(hDC, (unsigned int)(10+(ONE_AU_METERS*scaleLength/displayScale)+10), 
			     Recto.bottom-15, scaleText, strlen(scaleText) );
}

void getSolarSystemInfo(void)
{
	uint32_t index;
	uint32_t numObjects = getNumObjects();

	for(index=0; index<numObjects ; index++ )
	{
		mySolarSystem.getGravityObjectInfo(index, &myGravityObjectNames[index],
			                                          &myGravityObjectNamesLength[index] );

	}
}



void computeNewPositions(void)
{
	uint32_t index;

	for(index=0;index<numIterationsPerDisplayPoint ; index++)
	{
		mySolarSystem.processTimeInterval(COMPUTATION_TIME_INTERVAL);
		cumulativeTime += COMPUTATION_TIME_INTERVAL;
	}

	storeNewSetOfCoords();
    computeNewDisplayCoords();
}


VOID HandleTimer(HWND hWnd)
{
	RECT rt;

	GetClientRect(hWnd, &rt);

	InvalidateRect(hWnd, &rt,TRUE );

	computeNewPositions();

//	UpdateWindow(hWnd);

}

#define CLICK_DISTANCE_PIXELS 20
void handleNewOriginClick(HWND hWnd, uint16_t fwKeys, uint16_t xPos, uint16_t yPos)
{
	uint32_t index;
	double largestMass = 0;
	uint32_t numObjects = getNumObjects();
	
	centerObjectIndex = -1;
	for(index=0 ; index<numObjects ; index++)
	{
        if ( ( displayOriginOffset.x+myGravityObjectCoordHistory[index][0].x > xPos-CLICK_DISTANCE_PIXELS ) &&
			 ( displayOriginOffset.x+myGravityObjectCoordHistory[index][0].x < xPos+CLICK_DISTANCE_PIXELS ) &&
			 ( displayOriginOffset.y+myGravityObjectCoordHistory[index][0].y > yPos-CLICK_DISTANCE_PIXELS ) &&
			 ( displayOriginOffset.y+myGravityObjectCoordHistory[index][0].y < yPos+CLICK_DISTANCE_PIXELS ) )
		{
			// We're within the click range.  find the first object in the click range.
			centerObjectIndex = index;
			break;
		}
	}

	if ( (fwKeys & (MK_CONTROL|MK_SHIFT)) == 0 )
	{
		// If control only is pressed.
	}
}


void handleMouseWheel(HWND hWnd, uint16_t fwKeys, int16_t zDelta, uint16_t xPos, uint16_t yPos)
{
	RECT rt;

    
	if ( (fwKeys & (MK_CONTROL|MK_SHIFT)) == MK_CONTROL )
	{
		// If control only is pressed.
		if ( zDelta < 0 )
		{
			displayScale *= 1.1;
		}
		else
		{
			displayScale /= 1.1;
		}

	}
	else if ( (fwKeys & (MK_CONTROL|MK_SHIFT)) == MK_SHIFT )
	{
		// If shift only is pressed.
		if ( zDelta < 0 )
		{
			if ( numIterationsPerDisplayPoint > 1 )
			{
				numIterationsPerDisplayPoint /= 2; 
			}
		}
		else
		{
			if ( numIterationsPerDisplayPoint < 512 )
			{
				numIterationsPerDisplayPoint *= 2; 
			}
		}
	}
	else if ( (fwKeys & (MK_CONTROL|MK_SHIFT)) == 0 )
	{
		// If control and shift are not pressed.
		// update the angle.
		viewingAngle -= zDelta*1/120;
		while ( viewingAngle <= -180 ) viewingAngle += 360;
		while ( viewingAngle > 180 ) viewingAngle -= 360;
	}

	GetClientRect(hWnd, &rt);

	computeNewDisplayCoords();

	InvalidateRect(hWnd, &rt,TRUE );
	
}

void GamePaint(HWND hWnd, HDC hDC)
{

	HPEN hBluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	HPEN hPen = (HPEN)SelectObject(hDC, hBluePen);
	RECT Recto;
	POINT oldOrigin;

	displayTheScale( hWnd,  hDC);
	displayTheViewingAngle( hWnd,  hDC);
	displayTheTimeScale( hWnd,  hDC);

	// Retrieve the size of the drawing area
	GetClientRect(hWnd, &Recto);

    displayOriginOffset.x = Recto.right/2;
	displayOriginOffset.y = Recto.bottom/2;

	if ( centerObjectIndex != -1 )
	{
		displayOriginOffset.x -= myGravityObjectCoordHistory[centerObjectIndex][0].x;
		displayOriginOffset.y -= myGravityObjectCoordHistory[centerObjectIndex][0].y;
	}
//	SetViewportOrgEx(hDC, Recto.right/2, Recto.bottom/2 , &oldOrigin);
	SetViewportOrgEx(hDC, displayOriginOffset.x, 
		                  displayOriginOffset.y, &oldOrigin);

	int32_t index;
	uint32_t numObjects = getNumObjects();

//	for(index=0 ; index<numObjects ; index++)
	for(index=numObjects-1 ; index>=0 ; index--)
	{
		// Draw the textual name of the object.
		TextOut(hDC, myGravityObjectCoordHistory[index][0].x, 
			         myGravityObjectCoordHistory[index][0].y, 
					 myGravityObjectNames[index], myGravityObjectNamesLength[index] );

		// Draw the 
		Ellipse(hDC, myGravityObjectCoordHistory[index][0].x-2,
			         myGravityObjectCoordHistory[index][0].y-2,
					 myGravityObjectCoordHistory[index][0].x+2,
					 myGravityObjectCoordHistory[index][0].y+2);

		// Draw the history "tail"
	    if ( myGravityObjectCoordHistoryDepth >= 2 )
		{
			Polyline( hDC, &myGravityObjectCoordHistory[index][0], myGravityObjectCoordHistoryDepth);
		}

		// Draw the line to the shadow position.
		{
			HPEN hGreyPen = CreatePen(PS_DOT, 1, RGB(170, 170, 170));
			HPEN hPen2 = (HPEN)SelectObject(hDC, hGreyPen);

			MoveToEx(hDC, myGravityObjectShadow[index].x, 
						  myGravityObjectShadow[index].y, NULL);
			LineTo(hDC, myGravityObjectCoordHistory[index][0].x, 
						myGravityObjectCoordHistory[index][0].y );

			DeleteObject(hGreyPen);   // flush our new pen.  we're done with it.
			SelectObject(hDC, hPen2);  // put the pen back to the original one.
		}


	}

	SelectObject(hDC, hPen);  // put the pen back to the original one.
	DeleteObject(hBluePen);   // flush our new pen.  we're done with it.
	SetViewportOrgEx(hDC, oldOrigin.x, oldOrigin.y , NULL);


}

//void GamePaint(HWND hWnd, HDC hDC)
//{
//
//	HPEN hBluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
//	HPEN hPen = (HPEN)SelectObject(hDC, hBluePen);
//	RECT Recto;
//
//	// Retrieve the size of the drawing area
//	GetClientRect(hWnd, &Recto);
//
//	POINT oldOrigin;
//	SetViewportOrgEx(hDC, Recto.right/2, Recto.bottom/2 , &oldOrigin);
//
//	MoveToEx(hDC, 0, 0, NULL);
//	LineTo(hDC, 50, 50);
//	LineTo(hDC, 60, 50);
//
//
//	Rectangle(hDC, 150, 30, 170, 50);
//
//	Ellipse(hDC, -20, -20, +20, +20);
//
//	TextOut(hDC, 60, 60, TEXT("Mike"), 4);
//
//	SelectObject(hDC, hPen);  // put the pen back to the original one.
//	DeleteObject(hBluePen);   // flush our new pen.  we're done with it.
//	SetViewportOrgEx(hDC, oldOrigin.x, oldOrigin.y , NULL);
//
//}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TEST2);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_TEST2;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   getSolarSystemInfo();

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   SetTimer(hWnd, 1, MSEC_PER_DISPLAY_UPDATE, 0);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
 				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);

			GamePaint( hWnd, hdc );

			// TODO: Add any drawing code here...
			RECT rt;

			sprintf( tempString, "Elapsed: %.1f Days", cumulativeTime/ONE_DAY_TIME_INTERVAL );

			GetClientRect(hWnd, &rt);
//			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			DrawText(hdc, tempString, strlen(tempString), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;

			
		case 0x020A: //WM_MOUSEWHEEL:
			{
				uint16_t fwKeys = LOWORD(wParam);    // key flags
				int16_t zDelta = (short) HIWORD(wParam);    // wheel rotation
				uint16_t xPos = (short) LOWORD(lParam);    // horizontal position of pointer
				uint16_t yPos = (short) HIWORD(lParam);    // vertical position of pointer

				handleMouseWheel(hWnd, fwKeys, zDelta, xPos, yPos);
			}
			break;

		case WM_LBUTTONDOWN: // WM_LBUTTONDBLCLK:
			{
				uint16_t fwKeys = wParam;        // key flags 
				uint16_t xPos = LOWORD(lParam);  // horizontal position of cursor 
				uint16_t yPos = HIWORD(lParam);  // vertical position of cursor 
 
				handleNewOriginClick(hWnd, fwKeys, xPos, yPos);
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_TIMER:
			HandleTimer(hWnd);
			break;
			
			//		case  WM_ERASEBKGND:
//			return TRUE;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


//---------------------------------------------------------------------------

// include the standard windows header files
#define WIN32_LEAN_AND_MEAN


#pragma comment( lib, "dinput.lib" )
#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

#pragma hdrstop

// the dimensions of the screen
int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;


#include "BaseGame.h"


//---------------------------------------------------------------------------

// The window message handler prototype
LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );


// The window handle
HWND hWnd;


// this struct holds information for the window class
WNDCLASSEX wc;


// The input handler ( stores all of the input information )
InputHandler *hInput;



// The quake 2 game that demonstrates BSP map technology
BaseGame *game;




#pragma argsused

// Main program starts here
WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{

    // Load in the Screen's width and height from the config file
    FILE *configFile = fopen( "config.txt", "r" );
    char buf[ 10 ];
    memset( buf, 0, 10 );
    SCREEN_WIDTH = atoi( fgets( buf, 10, configFile ) );
    memset( buf, 0, 10 );
    SCREEN_HEIGHT = atoi( fgets( buf, 10, configFile ) );
    fclose( configFile );

    // Init the Input handler
    hInput = new InputHandler();


    ZeroMemory( &wc, sizeof( wc ) );

    // give a few necessary attributes for the main window
    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = CS_HREDRAW | CS_VREDRAW;

    wc.lpfnWndProc = WindowProc;

    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = ( HBRUSH ) COLOR_ACTIVECAPTION;
    //wc.hbrBackground = ( HBRUSH ) 5;
    wc.lpszClassName = "WindowClass1";

    // register the window class
    if ( FAILED ( RegisterClassEx( &wc ) ) ) {
        return 0;
    }

    // create the window and use the result as the handle
    hWnd = CreateWindowEx(NULL, "WindowClass1",    // name of the window class
                          "Quake 2", WS_EX_TOPMOST | WS_POPUP,
                          0, 0,    // position of the window
                          SCREEN_WIDTH, SCREEN_HEIGHT,    // dimensions of the window
                          NULL, NULL, hInstance, NULL );    // used with multiple windows, NULL

    // Setup the input handler
    hInput->init( &hWnd );

    if ( hWnd == NULL ) {
        return 1;
    }

    // display the window on the screen
    ShowWindow( hWnd, nCmdShow );



    // Create the game and initialise it
    game = new BaseGame();
    game->init( hWnd, hInput, SCREEN_WIDTH, SCREEN_HEIGHT );



    // this struct holds Windows event messages
    MSG msg;


    // Main loop
    while ( TRUE ) {
        // Check to see if any messages are waiting in the queue
        while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            // Translate the message and dispatch it to WindowProc()
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }

        // Render the world
        game->draw();

        // Temporary: if 'q' was pressed, then quit
        if ( hInput->quit ) {
            break;
        }
    }

    // delete the objects
    delete hInput;
    delete game;


    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
};

// this is the main message handler for the program
LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if ( hInput->handleMessages( message, wParam, lParam ) ) {
        return 0;
    }

    // sort through and find what code to run for the message given
	switch ( message ) {
		case WM_SYSCOMMAND: {            // Intercept system commands
            switch ( wParam ) {
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
			break;

		} case WM_DESTROY: {
            // close the application entirely
            PostQuitMessage( 0 );
            return 0;

        } case WM_SETCURSOR: {
            // Turn off window cursor
            SetCursor( NULL );

            // Don't let windows use a cursor
            return TRUE;
        } case WM_MOUSEMOVE: {

            return TRUE;
        }
	}


    // Handle any messages the switch statement didn't
    return DefWindowProc ( hWnd, message, wParam, lParam );
};

//---------------------------------------------------------------------------


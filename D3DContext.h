//---------------------------------------------------------------------------

#ifndef D3DContextH
#define D3DContextH

#include <windows.h>
#include <DirectX/d3d9.h>
#include <DirectX/d3dx9.h>

#pragma hdrstop


/**
 * This module provides an easy and fast way of communication with some of DirectX's
 * capabilities through a set of high-level calls to tasks such as handle DirectX device
 * initialisation and model transformations.
 *
 * Without this module, the set of DirectX structures would be scattered in the main
 * program (WinMain.cpp), or the Engine object (Engine.cpp). Its use is purely for
 * the ease of use to the programmer and for cleaner code.
 *
 * DirectX Device Creation is possible because a link to the window handle is given as
 * a parameter in the call to create the DirectX device (init()).
 */


class D3DContext {
    public:

        /**
         * Default constructor routine that sets all values in the D3DContext
         * object to zero, and all of the pointers in the object to NULL.
         *
         * No other operations are done, and the DirectX device must be instantiated
         * later with the init() method.
         */
        D3DContext() {
            d3d = NULL;
            device = NULL;
            screenWidth = 0;
            screenHeight = 0;

            hWnd = 0;
        };

        /**
         * Constructor that calls init(). This constructor is differentiated from
         * the first one due to the number of parameters. After this constructor
         * is called, init() does NOT need to be called again - the DirectX
         * device has already been made.
         */
        D3DContext( HWND hWnd, int width, int height ) {
            init( hWnd, width, height );
        };

        /**
         * Destructor that tests to see if any of the objects contained within the
         * D3DContext object have been initiated. If they have, then delete them,
         * or in the case of DirectX, "Release" them. 
         */
        ~D3DContext() {
            if ( d3d != NULL ) {
                d3d->Release();
            }
            if ( device != NULL ) {
                device->Release();
            }
        };


        /**
         * D3DContext::init() creates the Direct3D device object in memory and sets
         * it up so the DirectX device can be drawn to by the main application.
         * After this, the device is ready to use for rendering
         */
        void init( HWND hWnd, int width, int height );


        /**
         * setupProjection() sets up Direct3D's projection matrix.
         * The projection matrix is just a way of transforming a point in 3d space
         * onto the user's screen, creating an illusion of a 3D world.
         */
        void setupProjection( float fov );

        /**
         * setupWorldProjection() modifies Direct3D's world transformation matrix.
         * The world matrix modifies how objects in the world are transformed. This
         * could include where the object is, how it is rotated, and how big it is.
         */
        void setupWorldTransform( float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz );


        /**
         * updateScreen() Sends Direct3D a message that tells it to update the
         * screen by sending the image that was rendered earlier by the game engine.
         */
        void updateScreen();


        /**
         * clearScreen() refreshes DirectX's Depth buffer. Clearing the depth
         * buffer prevents objects that are behind other objects from being
         * rendered in front of the object that should be in the foreground.
         */
        void clearScreen();

        /**
         * Several parts of the application need access to an LPDIRECT3DDEVICE9
         * structure for most of the rendering. This function simply gives access
         * to the application so the LPDIRECT3DDEVICE9 can be passed around easily.
         */
        LPDIRECT3DDEVICE9 getDevice() {
            return device;
        };

    private:
        /**
         * reset() simply resets all of DirectX's states and behaviours to the
         * default values for this application.
         */
        void reset();

        // The main Direct3D object
        LPDIRECT3D9 d3d;

        // A set of states and behaviours that DirectX uses to initialise an
        //  LPDIRECT3DDEVICE9
        D3DPRESENT_PARAMETERS d3dpp;

        // The screen size
        int screenWidth, screenHeight;

        // A link to the window handle used in WinMain.cpp
        HWND hWnd;

        // The DirectX device, used for all of the rendering purposes.
        LPDIRECT3DDEVICE9 device;

};

//---------------------------------------------------------------------------
#endif

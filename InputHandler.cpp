//---------------------------------------------------------------------------

#pragma hdrstop

#include "InputHandler.h"


//==============================================================================
//                  InputHandler methods
//==============================================================================

/**
 * Creates an InputHandler object, and sets it up for use.
 * Note that the init() method must be called before the object can be properly used.
 */
InputHandler::InputHandler() {
    quit = false;

    dInput = NULL;
    mouse = NULL;

    inputState = NULL;
    hWnd = NULL;
};


/**
 * Deletes all of the objects instantiated within the InputHandler object.
 * Also cleans up the DirectInput objects that were made.
 */
InputHandler::~InputHandler() {
    // delete the input state object
    if ( inputState != NULL ) {
        delete inputState;
        inputState = NULL;
    }

    // Release the DirectInput main object
    if ( dInput != NULL ) {
        dInput->Release();
        dInput = NULL;
    }

    // Release the DirectInput mouse object
    if ( mouse != NULL ) {
        mouse->Unacquire();
        mouse->Release();
        mouse = NULL;
    }
};


/**
 * This method sets up the mouse with DirectInput and instantiates the input state.
 * After this is called, the InputHandler can be used normally to detect the mouse
 * and keyboard input.
 */
bool InputHandler::init( HWND *hWnd ) {
    this->hWnd = hWnd;

    inputState = new InputState();

    initDirectInput();

    return true;
};

/**
 * This method simply gets the mouse state from DirectInput and updates the
 * input state accordingly.
 */
void InputHandler::updateMouse() {
    mouse->GetDeviceState( sizeof( inputState->mouse ), ( LPVOID ) &inputState->mouse );

    inputState->updateMouse( 0.15f );
};

/**
 * This method, called by init(), creates the DirectInput main object, then
 * calls initMouse() to register the mouse with DirectInput.
 */
bool InputHandler::initDirectInput() {
    // Create the main DirectInput object
    DirectInput8Create( GetModuleHandle(NULL),
                             DIRECTINPUT_VERSION,
                             IID_IDirectInput8,
                             ( void** ) &dInput,
                             NULL );

    // Initialise the mouse with DirectInput
    initMouse();

    return true;
};

/**
 * This method registers the mouse with DirectInput.
 */
void InputHandler::initMouse() {

    // Create the mouse object with DirectInput
    dInput->CreateDevice(GUID_SysMouse,
                                &mouse,
                                NULL);

    // Set up the mouse so it can be used properly.
    mouse->SetCooperativeLevel( *hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND );
    mouse->SetDataFormat( &c_dfDIMouse2 );

    mouse->Acquire();
};

const int KEY_ESCAPE = 27;

// Handles keyboard input messages from windows
//     Returns true if the message was handled
bool InputHandler::handleMessages( UINT message, WPARAM wParam, LPARAM lParam ) {
    switch( message ) {
        case WM_KEYUP: {
            // The user released a key:
            inputState->setKey( wParam, false );
            return true;

        } case WM_KEYDOWN: {
            // The user pressed a key:
            inputState->setKey( wParam, true );

            // if that key was the escape key, then quit
            if ( wParam == KEY_ESCAPE ) {
                quit = true;
            }

            return true;
        }
    }

    return false;
};

//==============================================================================
//                  InputState methods
//==============================================================================

// Constructor that resets all the keys and the mouse state
InputState::InputState() {
    reset();
};


InputState::~InputState() {
};


// Resets the properties of the input state
void InputState::reset() {

    // Reset all of the keys
    ZeroMemory( keys, NUM_KEYS );

    // Reset the DirectInput mouse state
    ZeroMemory( &mouse, sizeof( DIMOUSESTATE2 ) );

    // Reset the mouse state
    mouseState.reset();
};


// Update the mouse so its movements can be tracked
void InputState::updateMouse( float mouseSensitivity ) {

    // Rotate the mouse
    mouseState.x += float( mouse.lX ) * mouseSensitivity;
    mouseState.y += float( mouse.lY ) * mouseSensitivity;

    // Make sure the mouse can't be rotated more than 90 degrees up or
    // down so the camera can't just roll
    if ( mouseState.y > 90.0 ) {
        mouseState.y = 90.0;
    } else if ( mouseState.y < -90.0 ) {
        mouseState.y = -90.0;
    }
};


//---------------------------------------------------------------------------
#pragma package(smart_init)

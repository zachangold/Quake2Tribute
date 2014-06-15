//---------------------------------------------------------------------------

#ifndef InputHandlerH
#define InputHandlerH

#include <vector.h>
#include <DirectX/dinput.h>

#pragma hdrstop


/**
 * InputHandler.h, InputHandler.cpp:
 * These files are made to handle all mouse and keyboard input for the application.
 * Mouse input uses DirectInput, whereas Keyboard input uses standard Win32 calls.
 * In the future, keyboard input will be handled my DirectInput as well.
 * The only class that needs to be instantiated directly by the program is the
 * InputHandler class - the other two classes are for storing the information about
 * the state of the mouse and keyboard. 
 */


// Two constants for information within the MouseState and InputState classes
const int NUM_KEYS = 256;
const int NUM_MOUSE_BUTTONS = 8;

/**
 * MouseState structure:
 *
 * Holds the properties of the mouse
 */
class MouseState {
    public:
        float x, y;
        bool buttons[ NUM_MOUSE_BUTTONS ];

        // Resets the properties of the mouse
        void reset() {
            x = 0;
            y = 0;

            for ( int i = 0; i < NUM_MOUSE_BUTTONS; ++i ) {
                buttons[ i ] = false;
            }
        };

};


/**
 * InputState structure:
 *
 * Holds all input information
 */
class InputState {
    private:
        // the properties of the mouse
        MouseState mouseState;

        // The states of the keys
        bool keys[ NUM_KEYS ];

        // A set of keys that were pressed by the user
        vector< unsigned char > keysPressed;

    public:
        // DirectInput mouse state
        DIMOUSESTATE2 mouse;

        // Prepares the object for use (init() must be called first)
        InputState();

        // Deletes all of the memory and cleans up after this object
        ~InputState();

        // Resets the properties of the input state
        void reset();

        // Update the mouse so its movements can be tracked
        void updateMouse( float mouseSensitivity );


        /**
         * Returns the key that was just pressed by the user
         */
        unsigned char popKeyPress() {

            // Make sure we don't access an array with 0 elements
            if ( keysPressed.size() == 0 ) {
                return 0;
            }

            // return the key that was pressed last, and remove that entry from the array
            unsigned char rtn = keysPressed[ keysPressed.size() - 1 ];
            keysPressed.pop_back();

            return rtn;
        };


        // Sets the value of a key
        void setKey( unsigned char index, bool value ) {
            // If the key was pressed, add it to our keysPressed array
            if ( value ) {
                keysPressed.push_back( index );
            }

            // Set the value in the keys array to value (true = pressed, false = released)
            keys[ index ] = value;
        };

        // Returns the value of a key
        bool getKey( unsigned char index ) {
            return keys[ index ];
        };

        // Returns the mouse state
        MouseState *getMouseState() {
            return &mouseState;
        };

        // Returns a pointer to the key array
        bool *getKeys() {
            return keys;
        };
};


/**
 * InputHandler structure:
 *
 * Handles input from the keyboard and mouse
 */
class InputHandler {
    public:

        // Sets up the object for use
        InputHandler();

        // Cleans up after the object and deletes it
        ~InputHandler();

        // Creates the DirectInput structures
        bool init( HWND *hWnd );

        // Handle the keyboard messages that normally get sent to WindowProc()
        bool handleMessages( UINT message, WPARAM wParam, LPARAM lParam );

        // Updates the mouse with DirectInput
        void updateMouse();

        // Returns the Input state associated with this InputHandler
        inline InputState *InputHandler::getInputState() {
            return inputState;
        };

        // Temporary: if 'q' was pressed, then quit=true. Otherwise, quit=false
        bool quit;
        
    private:

        // DirectInput structure pointers
        LPDIRECTINPUT8 dInput;
        LPDIRECTINPUTDEVICE8 mouse;

        // Methods to initialise DirectInput
        bool initDirectInput( void );
        void initMouse();


        // A link to the window handle
        HWND *hWnd;

        // The state of the keys and mouse
        InputState *inputState;
};

//---------------------------------------------------------------------------
#endif

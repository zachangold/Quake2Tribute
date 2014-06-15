//---------------------------------------------------------------------------

#ifndef BaseGameH
#define BaseGameH

#include "Engine.h"

/**
 * The main file (WinMain.cpp) contains a BaseGame object. This BaseGame would
 * normally hold all of the game rules and game logic, but since this application
 * has no need for game logic, the only major part of this class is the Engine
 * object which it contains.
 */
class BaseGame {
    public:

        /**
         * Constructor initialises the object, preparing it for use.
         */
        BaseGame();

        /**
         * Destructor de-allocates any memory allocated by this object.
         */
        ~BaseGame();


        /**
         * init() initialises the Engine object, preparing the entire application
         * for use. After init() is called, draw() is the only function that needs
         * to be called after.
         */
        void init( HWND hWnd, InputHandler *hInput, int screenWidth, int screenHeight );

        /**
         * draw() updates the Engine, and draws everything in the application,
         * using the Engine's draw() method
         */
        void draw();

    private:

        // The Engine object for the application.
        Engine *engine;


};

//---------------------------------------------------------------------------
#endif

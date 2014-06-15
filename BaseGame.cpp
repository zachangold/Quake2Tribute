//---------------------------------------------------------------------------

#pragma hdrstop

#include "BaseGame.h"


// sets all fields to their default values or NULL
BaseGame::BaseGame() {
    engine = NULL;
};

/**
 * Deletes any memory allocated by this object
 */
BaseGame::~BaseGame() {
    if ( engine != NULL ) {
        delete engine; // delete the engine object
    }
};


/**
 * Loads in the map and creates the Engine object
 */
void BaseGame::init( HWND hWnd, InputHandler *hInput, int screenWidth, int screenHeight ) {
    engine = new Engine();
    engine->init( hWnd, hInput, screenWidth, screenHeight );
    
};

/**
 * Draws the entire scene to the screen
 */
void BaseGame::draw() {
    engine->draw();
};


//---------------------------------------------------------------------------
#pragma package(smart_init)

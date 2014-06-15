//---------------------------------------------------------------------------

#pragma hdrstop

#include "Console.h"

//==============================================================================
//          CONSOLE METHODS
//==============================================================================

/**
 * Constructor that sets all of the Console's values to their defaults,
 * preparing that console for later use.
 */
Console::Console() {
    font = NULL;
    d3dContext = NULL;

    hasFocus = false;
};


/**
 * Destructor that deletes any memory allocated by the Console object
 */
Console::~Console() {

    // Delete each line
    for ( unsigned int i = 0; i < lines.size(); ++i ) {
        delete lines[ i ];
    }

    lines.resize( 0 );

    // delete the font
    if ( font != NULL ) {
        delete font;
    }
};


/**
 * printMessage() adds a line to the end of the lines array, with parameter
 * message as its text, coloured with parameter color.
 */
void Console::printMessage( string message, unsigned int color ) {
    lines.push_back( new ConsoleLine( message, color, &time ) );
};


/**
 * loadMap() creates a BSP map with file name the same as parameter fName.
 * The BSP Map reports its loading progress to the Console while it is loading.
 * Parameter camera's position is set to the spawning point in the map.
 */
BSPMap *Console::loadMap( string fName, Camera *camera ) {

    // refresh the screen
    d3dContext->clearScreen();
    d3dContext->updateScreen();

    // create a BSP Map and load it in
    BSPMap *rtn = new BSPMap();
    if ( !rtn->load( fName, d3dContext, camera, this ) ) {
        delete rtn;
        return NULL;
    }

    return rtn;
};


/**
 * unloadMap() unloads parameter map, and the map reports its unloading
 * progress to the console while it is unloading.
 */
void Console::unloadMap( BSPMap *map ) {
    // unload the BSP map
    printMessage( "Unloading BSP map.", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
    map->unload( d3dContext, this );
};


/**
 * clearScreen() simply deletes all of the console's lines, "cleaning" the
 * console part of the screen.
 */
void Console::clearScreen() {
    // delete all of the lines
    for ( unsigned int i = 0; i < lines.size(); ++i ) {
        delete lines[ i ];
    }

    lines.resize( 0 );
};

/**
 * Appends parameter c to the end of the input string. This is used for
 * when the console has focus and the user presses a key - the letter that
 * the user input appears at the end of the input string.
 */
void Console::addInputChar( char c ) {
    inputLine.push_back( c );
};


/**
 * deleteChar() removes the last character of the input string. This is
 * used when the user presses the backspace key.
 */
void Console::deleteChar() {
    if ( inputLine.size() > 0 ) {
        inputLine.pop_back();
    }
};


/**
 * render() method draws the console to the screen. It also takes care of
 * making the Console lines fade out and disappear, de-allocating them
 * when they become invisible.
 */
void Console::render() {

    // for each line,
    for ( unsigned int i = 0; i < lines.size(); ++i ) {

        // if that line has disappeared, delete it and shift the rest of the lines
        // back one place.
        if ( lines[ i ]->getAlpha( &time ) < 0.0 ) {
            delete lines[ i ];

            for ( unsigned int shift = i; shift < lines.size() - 1; ++shift ) {
                lines[ shift ] = lines[ shift + 1 ];
            }
            lines.resize( lines.size() - 1 );

            i--;
        }
    }

    // Go through each line and render the lines
    for ( unsigned int lineNum = 0; lineNum < lines.size(); ++lineNum ) {
        lines[ lineNum ]->update( &time );
        font->setText( lines[ lineNum ]->getText() );
        font->render( d3dContext->getDevice(), lines[ lineNum ]->getColor(), lineNum, DT_LEFT | DT_NOCLIP );
    }

    // If the console has focus, render the input string
    if ( hasFocus ) {

        // Flicker an underscore (_) at the end of the input line every
        //  30 frames (1/2 a second)
        textFlicker++;
        if ( ( textFlicker / 30 ) % 2 == 0 ) {
            inputLine.push_back( '_' );
        }

        // render the input line
        font->setText( inputLine );
        font->render( d3dContext->getDevice(), D3DXCOLOR( 1.0, 0.8, 0.0, 1.0 ), lines.size(), DT_LEFT | DT_NOCLIP );

        // if an underscore was added earlier due to text flickering, delete it.
        if ( ( textFlicker / 30 ) % 2 == 0 ) {
            deleteChar();
        }
    }


};



/**
 * init() prepares the Console for rendering and handling input.
 * d3dContext: The Direct3D Context object used by the Engine.
 */
void Console::init( D3DContext *d3dContext, int screenWidth, int screenHeight ) {
    this->d3dContext = d3dContext;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    // create the font object
    font = new D3D::Font();
    font->init( d3dContext->getDevice(), screenWidth, screenHeight );

    // Tell the user that the console was created.
    lines.push_back( new ConsoleLine( string( "Console created." ), D3DXCOLOR( 0.0, 1.0, 0.0, 1.0 ), &time ) );
};


/**
 * Returns the map name from the last console command. This is called
 * when the COMMAND_NEWMAP value is returned from the previous call to
 * executeInputCommand().
 */
char *Console::getMapName() {
    char *token, *value;

    string tmp = lines[ lines.size() - 1 ]->getText();

    // parse the token and value parts of the command
    token = strtok( ( char * ) tmp.c_str(), " " );
    value = strtok( NULL, " " );

    // Check to make sure the map name is valid by testing to see if the map name
    // is the same as one of the predefined maps.
    bool isValid = false;
    for ( int i = 0; i < NUM_MAPS; ++i ) {
        if ( strcmp( value, BSPMap::ORDERED_MAP_NAMES[ i ] ) == 0 ) {
            isValid = true;
            break;
        }
    }

    // Return either the mapname if it is valid, or NULL if the mapname was invalid.
    if ( isValid ) {
        return value;
    } else {
        return NULL;
    }
};


/**
 * executeInputCommand() executes the input string as a command - this is
 * done when the user presses the Enter key.
 * The return value is the type of command that the user input. For now,
 * this can only be a new map, the command to show all maps, or an unknown
 * command, and the engine can respond accordingly. When the "map<mapname>"
 * command is used, the map name can be accessed by calling getMapName().
 */
int Console::executeInputCommand() {
    lines.push_back( new ConsoleLine( inputLine.c_str(), D3DXCOLOR( 1.0, 0.8, 0.0, 1.0 ), &time ) );

    char *token, *value;

    string tmp = inputLine;
    inputLine = string( "" );

    // parse the token and value parts of the command.
    token = strtok( ( char * ) tmp.c_str(), " " );
    value = strtok( NULL, " " );

    // if the command was map<mapname>, then return COMMAND_NEWMAP.
    if ( strcmp( token, "map" ) == 0 ) {
        return COMMAND_NEWMAP;
    } else if ( strcmp( token, "showmaps" ) == 0 ) {
        // if the command was showmaps, then print the mapnames to the console

        for ( int i = 0; i < NUM_MAPS; ++i ) {
            printMessage( string( BSPMap::ORDERED_MAP_NAMES[ i ] ), D3DXCOLOR( 1.0, 1.0, 1.0, 1.0 ) );
        }

        // Return the COMMAND_SHOWMAPS signal
        return COMMAND_SHOWMAPS;
    }


    // Otherwise, the command was of an unknown type.
    printMessage( "Unknown command type.", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
    return COMMAND_UNKNOWN;
};



//---------------------------------------------------------------------------
#pragma package(smart_init)

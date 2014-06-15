//---------------------------------------------------------------------------

#pragma hdrstop

#include "MapSelector.h"


/**
 * Constructor that prepares the object for initialisation by setting
 * all pointers to null. Before this object can be used properly, init()
 * must be called.
 */
MapSelector::MapSelector() {
    d3d = NULL;
    font = NULL;
    choice = 0;
    hasFocus = false;
};


/**
 * Destructor that tests to see if any memory has been allocated.
 * If any memory has been allocated, then this method deletes it.
 */
MapSelector::~MapSelector() {
    if ( font != NULL ) {
        delete font;
        font = NULL;
    }
};


/**
 * This method prepares the MapSelector object for normal use. It creates
 * the DirectX font object, and prepares the lines of text for the map
 * choices.
 *
 * d3d: A pointer to the Direct3D Context object used by the Engine.
 * screenWidth, screenHeight: The dimensions of the screen.
 */
void MapSelector::init( D3DContext *d3d, int screenWidth, int screenHeight ) {
    this->d3d = d3d;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    // create the font object
    font = new D3D::Font();
    font->init( d3d->getDevice(), screenWidth, screenHeight );

    // Reset all of the choiceLines
    for ( int i = 0; i < NUM_CHOICES_DRAWN; ++i ) {
        choiceLines[ i ].setData( "", D3DXCOLOR( 1.0, 1.0, 1.0, 1.0 ), NULL );
    }

    // highlight the middle line in yellow
    choiceLines[ ( int ) ( ( ( float ) NUM_CHOICES_DRAWN ) / 2.0 ) ].setData( "", D3DXCOLOR( 1.0, 0.8, 0.0, 1.0 ), NULL );

    // Initialise the choiceLines
    changeChoice( 0 );
};


/**
 * Draws the MapSelector in the bottom-left part of the screen. If the
 * "hasFocus" field is set to false, then drawing is canceled.
 */
void MapSelector::render() {

    // Don't render if the mapSelector doesn't have focus
    if ( !hasFocus ) {
        return;
    }

    // Draw the map choices
    for ( int i = 0; i < NUM_CHOICES_DRAWN; ++i ) {
        font->setText( choiceLines[ i ].getText() );
        font->render( d3d->getDevice(), choiceLines[ i ].getColor(), NUM_CHOICES_DRAWN - i, DT_LEFT | DT_BOTTOM | DT_NOCLIP );
    }

    // separate the map choices from the rest of the screen with a line at the top
    // and the bottom of the mapSelector
    font->setText( "--------------------------" );
    font->render( d3d->getDevice(), D3DXCOLOR( 1.0, 0.8, 0.0, 1.0 ), 0, DT_LEFT | DT_BOTTOM | DT_NOCLIP );
    font->render( d3d->getDevice(), D3DXCOLOR( 1.0, 0.8, 0.0, 1.0 ), NUM_CHOICES_DRAWN + 1, DT_LEFT | DT_BOTTOM | DT_NOCLIP );
};


/**
 * Changes the selected map.
 * change: The amount to change the map selection by - For example, the
 *      up arrow changes by +1, and the down arrow changes by -1.
 * This method also changes the choiceLines[] field, preparing for the
 * next call to render().
 */
void MapSelector::changeChoice( int change ) {
    choice += change;

    // prevent an access overrun
    if ( choice < 0 ) {
        choice = 0;
    } else if ( choice >= NUM_MAPS ) {
        choice = NUM_MAPS - 1;
    }

    // set all of the consoleLines to their appropriate values
    for ( int i = 0; i < NUM_CHOICES_DRAWN; ++i ) {
        setLine( i, choice + ( i - NUM_CHOICES_DRAWN / 2 ) );
    }
};


/**
 * Returns the choice field. This is used to get the map number when the
 * user presses the Enter key when the map selector has focus to load in
 * the BSP Map that the user just selected.
 */
int MapSelector::getChoice() {
    return choice;
};


/**
 * This method is used by the changeChoice() method. It sets the line at
 * lineNum to the map at BSPMap::MAP_UI_NAMES[ mapNum ]. If mapNum would
 * go over the array bounds of BSPMap::MAP_UI_NAMES, then the line at
 * lineNum is just set to an empty string ("").
 */
void MapSelector::setLine( int lineNum, int mapNum ) {
    if ( mapNum < 0 || mapNum >= NUM_MAPS ) {

        // If mapNum would go over the bounds of the mapname array, then just
        // set that line to blank text.
        choiceLines[ lineNum ].setText( "" );
    } else {

        // If mapNum is a valid index, then set that line to the appropriate
        // map name.
        choiceLines[ lineNum ].setText( BSPMap::MAP_UI_NAMES[ mapNum ] );
    }
};



//---------------------------------------------------------------------------
#pragma package(smart_init)

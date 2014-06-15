//---------------------------------------------------------------------------

#pragma hdrstop

#include "DrawingInfo.h"


//==============================================================================
//          DRAWINGINFO METHODS
//==============================================================================

/**
 * Constructor that sets all pointers to NULL, and prepares the object
 * to be initialised with the init() method.
 */
DrawingInfo::DrawingInfo() {
    d3d = NULL;
    camera = NULL;
    screenWidth = 0;
    screenHeight = 0;
    time = NULL;
    font = NULL;
};

/**
 * Destructor that simply tests to see if any memory has been allocated
 * by this object. If any memory has been allocated, then this method
 * de-allocates it.
 */
DrawingInfo::~DrawingInfo() {

    // Delete the timer object
    if ( time != NULL ) {
        delete time;
        time = NULL;
    }

    // Delete the font object
    if ( font != NULL ) {
        delete font;
        font = NULL;
    }
};

/**
 * This method simply calls map->draw(). It also collects information
 * about how the map was rendered so that information can be displayed
 * to the user later with the draw() call.
 */
void DrawingInfo::drawMap( BSPMap *map ) {

    // record the time going into renedering the BSP Map.
    unsigned int msStart = time->getTimeMillis();

    // Draw the map
    map->draw( d3d->getDevice(), camera, this );


    // get the number of milliseconds drawing the map took
    unsigned int msPassed = time->getTimeMillis() - msStart;

    // Prevent a divide by zero if drawing the map took < 1 ms
    if ( msPassed == 0 ) {
        msPassed = 1;
    }


    // temporary string for printing formatted text
    char str[ 128 ];

    // Set the drawing time line in the rendering information to the recorded value.
    sprintf( str, "Time taken to render map: %dms ( %d frames/s )", msPassed, ( int ) ( 1000.0 / ( float ) msPassed ) );
    lines[ INFO_DRAWING_TIME ].setData( str, D3DXCOLOR( 1.0, 1.0, 1.0, 1.0 ), NULL );
};

/**
 * This method takes the data collected earlier by the drawMap() method
 * and displays it at the top-right corner of the screen.
 */
void DrawingInfo::draw() {
    for ( int i = 0; i < NUM_RENDER_INFO_LINES; ++i ) {
        font->setText( lines[ i ].getText() );
        font->render( d3d->getDevice(), lines[ i ].getColor(), i, DT_RIGHT | DT_NOCLIP );
    }
};

/**
 * This method simply initialises this object so it can properly record
 * the rendering information and then display it to the screen.
 *
 * d3d: The Direct3D context used by the Engine
 * camera: The Camera object that holds the player's position
 * screenWidth, screenHeight: The dimensions of the screen
 */
void DrawingInfo::init( D3DContext *d3d, Camera *camera, int screenWidth, int screenHeight ) {
    this->d3d = d3d;
    this->camera = camera;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    // Create the timer object
    time = new Timer();

    // Create the font object
    font = new D3D::Font();
    font->init( d3d->getDevice(), screenWidth, screenHeight );
};

/**
 * This method allows the BSPMap to set a line in the rendering info. These
 * lines include the number of polygons rendered, and the number of lines
 * culled, and the time it took to render the map.
 *
 * lineNum: The line whose properties are going to be set to "text" and
 *      "color" parameters.
 * text: The text string to set the line to.
 * color: The colour to set the line to. This value should be a value
 *      returned from D3DXCOLOR().
 */
void DrawingInfo::setLine( int lineNum, string text, unsigned int color ) {
    lines[ lineNum ].setData( text, color, NULL );
};


//---------------------------------------------------------------------------
#pragma package(smart_init)

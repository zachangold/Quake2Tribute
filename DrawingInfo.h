//---------------------------------------------------------------------------

#ifndef DrawingInfoH
#define DrawingInfoH

// Include the header for the lines for rendering information
#include "ConsoleLine.h"

// Include the header for the BSP map
#include "BSPMap.h"


/**
 * The DrawingInfo class handles rendering a BSP map. While rendering, this class
 * collects information about how drawing went. This information includes the amount
 * of time taken to draw the map, the number of polygons rendered, and the number
 * of polygons culled. The information is then rendered by an Engine object, and
 * the Drawing information is printed in the upper-right corner of the screen.
 */
class DrawingInfo {
    public:

        /**
         * Constructor that sets all pointers to NULL, and prepares the object
         * to be initialised with the init() method.
         */
        DrawingInfo();

        /**
         * Destructor that simply tests to see if any memory has been allocated
         * by this object. If any memory has been allocated, then this method
         * de-allocates it.
         */
        ~DrawingInfo();


        /**
         * This method simply calls map->draw(). It also collects information
         * about how the map was rendered so that information can be displayed
         * to the user later with the draw() call.
         *
         * map: The BSP map instance that this call renders
         */
        void drawMap( BSPMap *map );

        /**
         * This method takes the data collected earlier by the drawMap() method
         * and displays it at the top-right corner of the screen.
         */
        void draw();

        /**
         * This method simply initialises this object so it can properly record
         * the rendering information and then display it to the screen.
         *
         * d3d: The Direct3D context used by the Engine
         * camera: The Camera object that holds the player's position
         * screenWidth, screenHeight: The dimensions of the screen
         */
        void init( D3DContext *d3d, Camera *camera, int screenWidth, int screenHeight );

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
        void setLine( int lineNum, string text, unsigned int color );


        /**
         * The following six constant integers define the purpose of each line.
         */

        // The Number of lines in the Drawing info
        static const int NUM_RENDER_INFO_LINES = 5;

        // Tells the user that some percentages can exceed 100%
        static const int INFO_NOTE = 0;

        // The number of polygons rendered
        static const int INFO_POLYGONS_RENDERED = 1;

        // The number of polygons culled by frustum or PVS-culling
        static const int INFO_NUM_PVS_CULLED = 2;
        static const int INFO_NUM_FRUSTUM_CULLED = 3;

        // The amount of time taken to render the frame and the number of
        //  frames per second.
        static const int INFO_DRAWING_TIME = 4;

    private:

        // A pointer to the application's D3DContext object
        D3DContext *d3d;

        // A pointer to the viewing camera
        Camera *camera;

        // The screen size
        int screenWidth, screenHeight;

        // The lines that tell the user about the rendering of the map
        ConsoleLine lines[ NUM_RENDER_INFO_LINES ];

        // The font (used to print the information out to the user)
        D3D::Font *font;

        // The timer used to calculate the time taken to render the BSP Map
        Timer *time;

};

//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------

#ifndef MapSelectorH
#define MapSelectorH

#include "ConsoleLine.h"
#include "D3DContext.h"
#include "Font.h"
#include "BSPMap.h"


/**
 * The MapSelector class provides a menu for the user to select a bsp map to read
 * in and use. The user interacts with the MapSelector in the following ways:
 * - The 'M' key opens or closes the map menu
 * - When the map menu is opened,
 *      - The up arrow scrolls up; and the down arrow scrolls down
 *      - The Enter key loads in the map that is selected.
 *
 * The input handling is done by the code in Engine.cpp.
 */
class MapSelector {
    public:
        /**
         * Constructor that prepares the object for initialisation by setting
         * all pointers to null. Before this object can be used properly, init()
         * must be called.
         */
        MapSelector();

        /**
         * Destructor that tests to see if any memory has been allocated.
         * If any memory has been allocated, then this method deletes it.
         */
        ~MapSelector();

        /**
         * This method prepares the MapSelector object for normal use. It creates
         * the DirectX font object, and prepares the lines of text for the map
         * choices.
         *
         * d3d: A pointer to the Direct3D Context object used by the Engine.
         * screenWidth, screenHeight: The dimensions of the screen.
         */
        void init( D3DContext *d3d, int screenWidth, int screenHeight );

        /**
         * Draws the MapSelector in the bottom-left part of the screen. If the
         * "hasFocus" field is set to false, then drawing is canceled.
         */
        void render();

        /**
         * Changes the selected map.
         * change: The amount to change the map selection by - For example, the
         *      up arrow changes by +1, and the down arrow changes by -1.
         * This method also changes the choiceLines[] field, preparing for the
         * next call to render().
         */
        void changeChoice( int change );

        /**
         * Returns the choice field. This is used to get the map number when the
         * user presses the Enter key when the map selector has focus to load in
         * the BSP Map that the user just selected.
         */
        int getChoice();


        // True if all input goes to the map selector,
        //  False if the input goes to something else and the map selector is not drawn.
        bool hasFocus;

        // The number of lines in the map selector.
        static const int NUM_CHOICES_DRAWN = 9;


    private:

        /**
         * This method is used by the changeChoice() method. It sets the line at
         * lineNum to the map at BSPMap::MAP_UI_NAMES[ mapNum ]. If mapNum would
         * go over the array bounds of BSPMap::MAP_UI_NAMES, then the line at
         * lineNum is just set to an empty string ("").
         */
        void setLine( int lineNum, int mapNum );

        // The Direct3D Context object that the engine uses
        D3DContext *d3d;

        // The Direct3D font used for the map selector's text
        D3D::Font *font;

        // The screen's dimensions
        int screenWidth, screenHeight;

        // The lines of text
        ConsoleLine choiceLines[ NUM_CHOICES_DRAWN ];

        // The choice number, which the user can modify with the up and down arrow keys.
        int choice;

};


//---------------------------------------------------------------------------
#endif

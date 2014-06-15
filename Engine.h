//---------------------------------------------------------------------------

#ifndef EngineH
#define EngineH


// Include the Direct3D context and input handler headers
#include "D3DContext.h"
#include "InputHandler.h"

// include a few DirectX utilities
#include "Light.h"
#include "Material.h"
#include "Shader.h"
#include "Camera.h"

#include "RenderTarget.h"

// Include the header for the sample MD2 Model (for demonstrating the world lights)
#include "MD2.h"

// Include the header for the BSP Map class
#include "BSPMap.h"

// Include the headers for the Console, MapSelector, and DrawingInfo classes.
#include "UI.h"



/**
 * The Engine object is the main module for this application.
 * It handles all Direct3D interactions, all models, and the main BSP map. The
 * Engine is handled by the main game module, BaseGame.cpp. For now, the engine
 * only has one method for interaction between it and its BaseGame, draw().
 * However, draw() carries out all functions necessary for the entire application.
 * If the Engine class was designed to support a full game, then several more
 * methods would be added, but for the purposes of BSP technology demonstration,
 * a smaller Engine class works just as well and reduces the amount of code needed.
 */
class Engine {
    public:

        /**
         * Constructor that sets all pointers to null, preparing the Engine object
         * for later use.
         */
        Engine();

        /**
         * Destructor that deallocated all memory allocated by the engine, and
         * releases the DirectX objects associated with the engine.
         */
        ~Engine();

        /**
         * init() method creates the DirectX objects, and loads in all data necessary
         * for the application.
         * hWnd: the window handle (from WinMain.cpp)
         * hInput: a pointer to the InputHandler (from WinMain.cpp)
         */
        void init( HWND hWnd, InputHandler *hInput, int screenWidth, int screenHeight );

        /**
         * draw() method draws and updates the engine. This includes drawing the
         * map and user inteface, and handing mouse and keyboard input. draw() handles
         * all tasks necessary for the application, so it is the only method that
         * needs to be called each frame.
         */
        void draw();


    private:
        /**
         * Loads in the BSP map with the same name as parameter mapName
         */
        void switchMap( string mapName );


        /**
         * Handles all keyboard and mouse interactions from the user
         */
        void handleInput();

        /**
         * Sets up the DirectX Material object so lighting can be possible.
         */
        void initLight();


        // The DirectX handler object
        D3DContext *d3d;

        // The bsp map object
        BSPMap *map;

        // The position of the player
        Camera *camera;

        // A sample model to demonstrate world lighting
        MD2Model *md2model;

        // A link to the input handler instantiated in WinMain.cpp
        InputHandler *hInput;

        // The DirectX material structure for lighting
        D3D::Material *material;


        // Boolean for whether or not to draw the sample model
        bool animateModel;

        // The User interface objects, including the console, rendering info,
        //  and the map menu
        Console console;
        DrawingInfo drawInfo;
        MapSelector mapSelector;


        D3D::Shader rtShader;
        RenderTarget rt;
        float time;
};


//---------------------------------------------------------------------------
#endif

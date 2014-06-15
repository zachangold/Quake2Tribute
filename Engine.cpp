//---------------------------------------------------------------------------

#pragma hdrstop

#include "Engine.h"



/**
 * Constructor that sets all pointers to null, preparing the Engine object
 * for later use.
 */
Engine::Engine() {
    d3d = NULL;

    map = NULL;
    camera = NULL;

    hInput = NULL;

    material = NULL;
    md2model = NULL;

    time = 0;

    // Default to NOT draw the sample MD2 Model
    animateModel = false;
};


/**
 * Destructor that deallocated all memory allocated by the engine, and
 * releases the DirectX objects associated with the engine.
 */
Engine::~Engine() {
    if ( d3d != NULL ) {
        delete d3d;
    }
    if ( map != NULL ) {
        delete map;
    }
    if ( camera != NULL ) {
        delete camera;
    }
    if ( material != NULL ) {
        delete material;
    }
    if ( md2model != NULL ) {
        delete md2model;
    }
};


/**
 * init() method creates the DirectX objects, and loads in all data necessary
 * for the application.
 * hWnd: the window handle (from WinMain.cpp)
 * hInput: a pointer to the InputHandler (from WinMain.cpp)
 */
void Engine::init( HWND hWnd, InputHandler *hInput, int screenWidth, int screenHeight ) {
    // Set the link to the input handler
    this->hInput = hInput;

    // Create the direct3d context
    d3d = new D3DContext( hWnd, screenWidth, screenHeight );


    // Setup the camera for viewing
    camera = new Camera();

    // Load in a sample .md2 model, and set its animation to
    //  the "idle" animation.
    md2model = new MD2Model();
    md2model->load( "Q2/models/monsters/soldier/", d3d->getDevice() );
    //md2model->setAnimation( 18, 56 );

    // Initialise the Text-based parts of the screen (Console, drawing
    //  information, and map selector)
    console.init( d3d, screenWidth, screenHeight );
    drawInfo.init( d3d, camera, screenWidth, screenHeight );
    mapSelector.init( d3d, screenWidth, screenHeight );


    // Create the map object and load it in with the first map in Quake 2
    map = new BSPMap();
    switchMap( "base1" );

    // Initialise the material structure
    initLight();

    rt.init( d3d->getDevice(), screenWidth, screenHeight );
    rtShader.createEffect( d3d->getDevice(), "transform.fx", "BBShader" );

};


/**
 * Sets up the DirectX Material object so lighting can be possible.
 */
void Engine::initLight() {

    // If the material object hasn't been made yet, then instantiate it
    if ( material == NULL ) {
        material = new D3D::Material();
    }

    /**
     * A small explanation:
     *      Lighting in DirectX requires use of two DirectX objects: the first
     *  is a light, and the second is a material. The light is how an object
     *  produces light, and the material is how an object might react in the
     *  presence of light. This could be used for a material acting differently
     *  when you use two different lights, or two materials acting differently
     *  under the same light. However, in this application, we will only be using
     *  one material for simplicity.
     */


    // Fill in the values for the lighting material.
    material->reset();

    material->getMaterial()->Diffuse = D3DXCOLOR( 1.0, 1.0, 1.0, 1.0f );
    material->getMaterial()->Ambient = D3DXCOLOR( 0.3f, 0.3f, 0.3f, 1.0f );
    material->getMaterial()->Specular = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );


    // Send the material to DirectX
    material->enable( d3d->getDevice() );
};


/**
 * draw() method draws and updates the engine. This includes drawing the
 * map and user inteface, and handing mouse and keyboard input. draw() handles
 * all tasks necessary for the application, so it is the only method that
 * needs to be called each frame.
 */
void Engine::draw() {

    handleInput();

    // Clear the screen before drawing
    d3d->clearScreen();



    // set up the transformations for the world and the view
    d3d->setupProjection( 80.0f );
    camera->setupTransform( d3d->getDevice() );


    // If the model is supposed to be animated, then update it.
    if ( animateModel ) {
        md2model->update( 0.016 );
    }

    rt.switchToRT( d3d->getDevice() );


    // Begin drawing the Direct3D scene
    d3d->getDevice()->BeginScene();

        // Transform to the origin and render the model there, if the model is to
        // be drawn.
        if ( animateModel ) {
            vector< Entity::Monster * > *monsters = map->getMonsters();

            for ( int i = 0; i < monsters->size(); ++i ) {
                Point3f monsterOrigin = ( *monsters )[ i ]->getOrigin();

                //map->enableLights( d3d->getDevice(), getPoint( -camera->pos->x, -camera->pos->y, -camera->pos->z ) );
                //d3d->setupWorldTransform( -camera->pos->x, -camera->pos->y, -camera->pos->z, -camera->pos->ry, camera->pos->rx - 180, 0.0, BSP::MAP_SCALE, BSP::MAP_SCALE, BSP::MAP_SCALE );
                map->enableLights( d3d->getDevice(), getPoint( monsterOrigin.y, monsterOrigin.z, -monsterOrigin.x ) );
                d3d->setupWorldTransform( monsterOrigin.y, monsterOrigin.z, -monsterOrigin.x, 0, 0, 0, BSP::MAP_SCALE, BSP::MAP_SCALE, BSP::MAP_SCALE );
                md2model->render( d3d->getDevice() );
            }
        }

        // Draw the BSP map
        d3d->setupWorldTransform( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, BSP::MAP_SCALE, BSP::MAP_SCALE, BSP::MAP_SCALE );
        drawInfo.drawMap( map );

        // Draw the User interface
        d3d->setupWorldTransform( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0 );
        console.render();
        drawInfo.draw();
        mapSelector.render();

    d3d->getDevice()->EndScene();

    rt.switchToBB( d3d->getDevice() );
    d3d->clearScreen();

    time += 0.001;

    UINT Pass, Passes;

    d3d->getDevice()->BeginScene();
        rtShader.getEffect()->SetFloat( "time", time );

        // draw the face with the pixel shader
        rtShader.getEffect()->Begin( &Passes, 0 );        for ( Pass = 0; Pass < Passes; Pass++ ) {
            rtShader.getEffect()->BeginPass( Pass );
            rt.drawRT( d3d->getDevice() );            rtShader.getEffect()->EndPass();        }
        rtShader.getEffect()->End();


    d3d->getDevice()->EndScene();

    // Stop drawing the scene and update what has been drawn to the screen
    d3d->updateScreen();


};

// Some key codes for handleInput()
const int KEY_TILDE = 192;
const int KEY_SHIFT = 16;
const int KEY_BACKSPACE = 8;
const int KEY_ARROW_LEFT = 37;
const int KEY_ARROW_UP = 38;
const int KEY_ARROW_RIGHT = 39;
const int KEY_ARROW_DOWN = 40;
const int KEY_ENTER = '\r';

/**
 * Handles all keyboard and mouse interactions from the user
 */
void Engine::handleInput() {

    // The key that the user pressed
    unsigned char keyPress;

    // get the keypress, terminating when there are no more recorded key presses.
    while ( ( keyPress = hInput->getInputState()->popKeyPress() ) != 0 ) {

        if ( keyPress == KEY_TILDE && !mapSelector.hasFocus ) {

            // If the user pressed the tilde key (~), then enable/disable the
            // console.
            console.hasFocus = !console.hasFocus;
        } else if ( keyPress == 'M' && !console.hasFocus ) {

            // If the user pressed the 'M' key, then enable/disable the map selector
            // menu.
            mapSelector.hasFocus = !mapSelector.hasFocus;
        } else if ( console.hasFocus ) {

            // If input should go to the console,

            // If the input key was a letter,
            if ( keyPress >= 'A' && keyPress <= 'Z' ) {

                // If the shift key is not being held down, make the letter
                // lower-case
                if ( !hInput->getInputState()->getKey( KEY_SHIFT ) ) {
                    keyPress -= ( int ) 'A' - ( int ) 'a';
                }

                // add the input character to the console command
                console.addInputChar( keyPress );
            } else if ( keyPress == ' ' || ( keyPress >= '0' && keyPress <= '9' ) ) {

                // if the input character was a space or a number, then add it to
                // the console input line.
                console.addInputChar( keyPress );
            } else if ( keyPress == KEY_BACKSPACE || keyPress == KEY_ARROW_LEFT ) {

                // delete the last character of the input string
                console.deleteChar();
            } else if ( keyPress == KEY_ENTER ) {

                // execute the console command and take focus away from the console.
                int commandType = console.executeInputCommand();
                console.hasFocus = false;

                // If the command was a new map,
                if ( commandType == Console::COMMAND_NEWMAP ) {

                    // get the map name from the console
                    char *mapName = console.getMapName();

                    // if the mapname was found and is valid, load that map.
                    if ( mapName != NULL ) {
                        switchMap( string( mapName ) );
                    } else {
                        // If the mapname was not found or is invalid, tell the
                        // user that it was invalid.
                        console.printMessage( "Invalid map name.", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
                    }
                }
            }
        } else if ( mapSelector.hasFocus ) {
            // if the map selector has focus, then the input goes to it.

            if ( keyPress == KEY_ARROW_UP ) {

                // if the up arrow was pressed, change the map choice
                mapSelector.changeChoice( -1 );
            } else if ( keyPress == KEY_ARROW_DOWN ) {

                // if the up arrow was pressed, change the map choice
                mapSelector.changeChoice( +1 );
            } else if ( keyPress == KEY_ENTER ) {

                // get the map choice and take focus away from the map selector.
                int mapChoice = mapSelector.getChoice();
                mapSelector.hasFocus = false;

                // Switch maps to the map at mapChoice
                switchMap( string( BSPMap::ORDERED_MAP_NAMES[ mapChoice ] ) );
            }
        }
    }


    // If input should go to the main application and not one of the UI elements,
    if ( !console.hasFocus && !mapSelector.hasFocus ) {

        // K disables lightmaps, L enables them
        if ( hInput->getInputState()->getKey( 'K' ) ) {
            map->lMap = 0;
        } else if ( hInput->getInputState()->getKey( 'L' ) ) {
            map->lMap = 1;
        }

        // O tells the engine to render the sample MD2 model, P tells the engine
        // to stop drawing the MD2 Model.
        if ( hInput->getInputState()->getKey( 'O' ) ) {
            animateModel = true;
        } else if ( hInput->getInputState()->getKey( 'P' ) ) {
            animateModel = false;
        }


        // Update the mouse position
        hInput->updateMouse();

        // Update the camera
        camera->update( hInput->getInputState()->getMouseState()->x, hInput->getInputState()->getMouseState()->y );
        camera->move( hInput->getInputState()->getKeys() );
    }
};

/**
 * Loads in the BSP map with the same name as parameter mapName
 */
void Engine::switchMap( string mapName ) {
    // unload the current BSP map
    console.unloadMap( map );
    delete map;

    // load in the new BSP Map
    map = console.loadMap( mapName, camera );
};

//---------------------------------------------------------------------------
#pragma package(smart_init)

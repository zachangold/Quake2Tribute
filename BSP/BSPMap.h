#ifndef BSPMAP_H_
#define BSPMAP_H_


// Include the standard bsp information
#include "BSPCommon.h"
#include "mapdef.h"

// Include the header files for the parts of the bsp map
#include "FaceInfo.h"
#include "LeafInfo.h"
#include "TextureInfo.h"
#include "Entity.h"
#include "LightMapInfo.h"
#include "BSPTree.h"

// Include a number of utilities for use in drawing the map
#include "D3DContext.h"
#include "Light.h"
#include "Shader.h"
#include "SkyBox.h"
#include "Font.h"


/**
 * The BSPMap Class is the main interface to be used for loading in and rendering
 * a Quake 2 .bsp map.
 * To load in a .bsp map, load() must be called. Before a .bsp map is replaced
 * by another .bsp map, unload() must be called.
 * To draw the .bsp map, the draw() routine must be called.
 *
 * Note that the draw(), load(), and unload() routines are sometimes not called
 * Directly by the containing Engine object, but instead through a special other
 * object that will collect information about how the map is loaded, unloaded,
 * or drawn, and then the special object will report this information to the user.
 * These objects include the Console and DrawingInfo classes.
 */


// Make sure the BSPMap class knows that these classes also exist:
class Console;
class DrawingInfo;


/**
 * BSPMap class
 */
class BSPMap {
	public:

        /**
         * Class Constructor simply initialises the class variables that are
         * not specifically associated with a .bsp map that is to be loaded.
         */
		BSPMap();

        /**
         * Class Destructor makes sure all of the memory allocated by the class
         * is deleted.
         */
		~BSPMap();


        /**
         * MAP_UI_NAMES is an array of strings that show the map name of a map
         * and its file name. This array is used for the MapSelector class, which
         * displays the formal map name of a map and its file name. For example,
         * it might show "Outer Base (base1)"
         */
        static const char *MAP_UI_NAMES[ NUM_MAPS ];

        /**
         * ORDERED_MAP_NAMES is an array of strings used as the file names for
         * all of the bsp map files in Quake 2. It is used for opening a map
         * file with an index, for example the MapSelector class returns an index
         * into this array for the map file name. Another use of this array is to
         * verify if a file name is a valid map name, when the map name is input
         * by the user using the console's "map" command
         */
        static const char *ORDERED_MAP_NAMES[ NUM_MAPS ];


        /**
         * load() routine:
         *  - fileName: the name of the .bsp file to be loaded, without its
         *      directory or file extension.
         *  - d3d: A pointer to a Direct3D Context object. This is needed in order
         *      to inform the user of the loading process because the console cannot
         *      be drawn without this object.
         *  - camera: A pointer to a camera object. After loading in the map, the
         *      position of the camera is found from a special entity in the map's
         *      entity lump.
         *  - console: A pointer to a Console object. While the map is loading, it
         *      reports its progress to the Console, and then draws the console to
         *      the display using the d3d parameter.
         *
         * load() returns false if the map file was not found.
         * load() returns true if the map file was loaded normally.
         */
		bool load( std::string fileName, D3DContext *d3d, Camera *camera, Console *console );


        /**
         * unload() routine:
         *  - d3d: A pointer to a Direct3D Context object. This is needed to update
         *      the screen with the unloading progress.
         *  - console: A pointer to a console object. This is needed to report the
         *      unloading progress, which is then drawn with the help of the d3d object.
         *
         * The first unload() routine reports the unloading progress to the console.
         * The second unload() routine does NOT report to the console. It is used
         *  for unloading the map when exiting the application.
         *
         * Both unload() routines delete all of the map data.
         */
        void unload( D3DContext *d3d, Console *console );
        void unload();


        /**
         * Draws the entire bsp map to the screen.
         *  - device: A link to a special DirectX structure for drawing. The map
         *      uses a specific set of methods in the device for drawing to the
         *      screen properly.
         *  - camera: A pointer to a Camera object. This is needed for Potentially
         *      Visible Set (PVS) culling. The set of visible polygons is determined
         *      based on where the camera is within the bsp map, and from that, frustum
         *      culling is done with the camera's frustum member to draw only what
         *      the user can see in the .bsp map.
         *  - drawInfo: A pointer to a DrawingInfo object. The map tells the object
         *      a few pieces of information based on how rendering the map went.
         *      The drawInfo object then shows the user the drawing statistics.
         *
         * After calling draw(), the .bsp map has been completely rendered. The
         * application can then render the rest of the objects in the world.
         */
        void draw( LPDIRECT3DDEVICE9 device, Camera *camera, DrawingInfo *drawInfo );

        int lMap;


        /**
         * enableLights() routine:
         *  - device: A link to a special DirectX structure for rendering. This
         *      is needed for enabling a set of world lights for rendering a model.
         *  - pos: A Point3f structure. This point is used to determine which lights
         *      in the world are closest to that point. These lights are then enabled
         *      with the device structure.
         *
         * All of the lights that are enabled with this call originate from the
         * entity lump of a bsp file. After these lights are enabled, the object
         * that is to be lit will be drawn with correct lighting.
         */
        void enableLights( LPDIRECT3DDEVICE9 device, Point3f pos );


        vector< Entity::Monster * > *getMonsters() {
            return entities->getMonsters();
        }


	private:

        /**
         * drawSkyBox() draws the sky around the camera.
         *  - device is a link to the DirectX object.
         *  - camera is the camera object.
         *
         * drawSkyBox() is called by draw()
         */
        void drawSkyBox( LPDIRECT3DDEVICE9 device, Camera *camera );

        // The "header" section of the .bsp map tells specifically where the map's
        // information is stored in the map file
        BSP::Header header;

        // The Objects for the data in the map
        TextureInfo *texInfo;
        FaceInfo *faceInfo;
        Entity::Parser *entities;
        LightMapInfo *lightMaps;
        BSPTree::Tree *bspTree;

        // The map's Pixel shader (This is just for combining the base texture
        //  of a face and its lightmap.)
        D3D::Shader *mapShader;

        // The map has a "skybox", which is just how the world looks around the map.
        // This object keeps track of the skybox information.
        SkyBox *skyBox;


        LPDIRECT3DTEXTURE9 ddsTexture;
        float vsTest;
};

#endif /* BSPMAP_H_ */



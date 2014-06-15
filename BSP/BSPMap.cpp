/*
 * BSPMap.cpp
 *
 *  Author: zach
 */

#include "BSPMap.h"
#include "UI.h"



/**
 * Class Constructor simply initialises the class variables that are
 * not specifically associated with a .bsp map that is to be loaded.
 */
BSPMap::BSPMap() {

    // Make sure all of our pointers are set to null so we know whether they are
    // instantiated or not.
    mapShader = NULL;

    texInfo = NULL;
    faceInfo = NULL;
    entities = NULL;
    lightMaps = NULL;
    bspTree = NULL;

    skyBox = NULL;

    ddsTexture = NULL;

    // Use lightmaps as default
    lMap = 0;
}


/**
 * Class Destructor makes sure all of the memory allocated by the class
 * is deleted.
 */
BSPMap::~BSPMap() {
    // just call unload() to unload the map's information
    unload();

};


/**
 * unload() routine:
 *  - d3d: A pointer to a Direct3D Context object. This is needed to update
 *      the screen with the unloading progress.
 *  - console: A pointer to a console object. This is needed to report the
 *      unloading progress, which is then drawn with the help of the d3d object.
 *
 * Reports the unloading progress to the console, and deletes all of the map data.
 */
void BSPMap::unload( D3DContext *d3d, Console *console ) {

    // Tell the user that we are deleting Textures
    d3d->clearScreen();
    d3d->getDevice()->BeginScene();
        console->printMessage( "Deleting Textures... ", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
        console->render();
    d3d->getDevice()->EndScene();
    d3d->updateScreen();

    // Delete the texture information.
    if ( texInfo != NULL ) {
        delete texInfo;
        texInfo = NULL;
    }

    // Tell the user that we just deleted the textures
    // Also, tell the user that we are deleting the map's vertex information
    d3d->clearScreen();
    d3d->getDevice()->BeginScene();
        console->printMessage( "Textures Deleted.", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
        console->printMessage( "Deleting Vertex Information... ", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
        console->render();
    d3d->getDevice()->EndScene();
    d3d->updateScreen();

    // Delete the vertex information
    if ( faceInfo != NULL ) {
        delete faceInfo;
        faceInfo = NULL;
    }

    // Tell the user that we just deleted the map's vertex information
    // Also, tell the user that we are deleting the map's entity section
    d3d->clearScreen();
    d3d->getDevice()->BeginScene();
        console->printMessage( "Vertex Information Deleted.", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
        console->printMessage( "Deleting Map Entities... ", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
        console->render();
    d3d->getDevice()->EndScene();
    d3d->updateScreen();

    // Delete the entity information
    if ( entities != NULL ) {
        delete entities;
        entities = NULL;
    }

    // Tell the user that we just deleted the map's entity section
    // Also, tell the user that we are deleting the lightmaps from the bsp file
    d3d->clearScreen();
    d3d->getDevice()->BeginScene();
        console->printMessage( "Map Entities Deleted.", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
        console->printMessage( "Deleting Lightmaps... ", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
        console->render();
    d3d->getDevice()->EndScene();
    d3d->updateScreen();

    // Delete the lightmaps
    if ( lightMaps != NULL ) {
        delete lightMaps;
        lightMaps = NULL;
    }

    // Tell the user that we just deleted the lightmaps from the bsp file
    // Also, tell the user that we are deleting the BSP Tree structure
    d3d->clearScreen();
    d3d->getDevice()->BeginScene();
        console->printMessage( "Lightmaps Deleted.", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
        console->printMessage( "Deleting Binary Space Partitioning Tree... ", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
        console->render();
    d3d->getDevice()->EndScene();
    d3d->updateScreen();


    // delete the BSP Tree
    if ( bspTree != NULL ) {
        delete bspTree;
        bspTree = NULL;
    }

    // delete the skybox object
    if ( skyBox != NULL ) {
        delete skyBox;
        skyBox = NULL;
    }

    // delete the map's Pixel Shader
    if ( mapShader != NULL ) {
        delete mapShader;
        mapShader = NULL;
    }

    // Tell the user that we just deleted the BSP Tree
    // Also, tell the user that we just finished deleting the entire BSP Map
    d3d->clearScreen();
    d3d->getDevice()->BeginScene();
        console->printMessage( "Binary Space Partitioning Tree Deleted. ", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
        console->printMessage( "Map Deleted.", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
        console->render();
    d3d->getDevice()->EndScene();
    d3d->updateScreen();

};

/**
 * unload() routine:
 *
 * This unload() routine does NOT report to the console. It is used
 *  for unloading the map when exiting the application. It also deletes all
 *  of the map's information.
 */
void BSPMap::unload() {
    // Delete the textures
    if ( texInfo != NULL ) {
        delete texInfo;
        texInfo = NULL;
    }

    // Delete the vertex information
    if ( faceInfo != NULL ) {
        delete faceInfo;
        faceInfo = NULL;
    }

    // Delete the entity section
    if ( entities != NULL ) {
        delete entities;
        entities = NULL;
    }

    // Delete the Lightmaps
    if ( lightMaps != NULL ) {
        delete lightMaps;
        lightMaps = NULL;
    }

    // Delete the BSP Tree
    if ( bspTree != NULL ) {
        delete bspTree;
        bspTree = NULL;
    }

    // Delete the map's pixel shader
    if ( mapShader != NULL ) {
        delete mapShader;
        mapShader = NULL;
    }

    // Delete the skybox that exists around the map
    if ( skyBox != NULL ) {
        delete skyBox;
        skyBox = NULL;
    }
    if ( ddsTexture != NULL ) {
        ddsTexture->Release();
        ddsTexture = NULL;
    }

};


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
bool BSPMap::load( std::string fileName, D3DContext *d3d, Camera *camera, Console *console ) {

    // Our base file object
	FILE *file = NULL;

    // add in the directory and file extension to the map name
    fileName = string( "Q2/maps/" ) + fileName + string( ".bsp" );

    // Tell the user that we are loading that bsp file
    console->printMessage( "Loading " + fileName, D3DXCOLOR( 1.0, 1.0, 1.0, 1.0 ) );


    // open the .bsp file for loading. If opening fails, return false
	if ( ( file = fopen( fileName.c_str(), "rb" ) ) == NULL ) {
        // Tell the user that the map was not found
        console->printMessage( "BSP Map file was not found.", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );
		return false;
	}

    // Instantiate all of our "info" structures
    texInfo = new TextureInfo();
    faceInfo = new FaceInfo();
    entities = new Entity::Parser();
    lightMaps = new LightMapInfo();
    bspTree = new BSPTree::Tree();

    // Create the Pixel shader object
    mapShader = new D3D::Shader();


    // read in the header
	fread( &header, sizeof( header ), 1, file );

    // Tell the user that we are loading in the lightmaps section
    d3d->getDevice()->BeginScene();
        console->printMessage( "Loading Lightmaps... ", D3DXCOLOR( 1.0, 1.0, 1.0, 1.0 ) );
    d3d->getDevice()->EndScene();
    d3d->updateScreen();

    // Load in the lightmaps
    lightMaps->load( &header, file );


    // Tell the user that we just loaded in the lightmaps section
    // Also, tell the user that we are loading in the Textures
    d3d->clearScreen();
    d3d->getDevice()->BeginScene();
        console->printMessage( "Lightmaps Loaded!", D3DXCOLOR( 0.0, 1.0, 0.0, 1.0 ) );
        console->printMessage( "Loading Textures... ", D3DXCOLOR( 1.0, 1.0, 1.0, 1.0 ) );
        console->render();
    d3d->getDevice()->EndScene();
    d3d->updateScreen();

    // load in the textures
    texInfo->load( &header, file, d3d->getDevice() );

    // Tell the user that we just loaded in the textures
    // Also, tell the user that we are loading in the Vertex Information
    d3d->clearScreen();
    d3d->getDevice()->BeginScene();
        console->printMessage( "Textures Loaded!", D3DXCOLOR( 0.0, 1.0, 0.0, 1.0 ) );

        console->printMessage( "Loading Vertex Information... ", D3DXCOLOR( 1.0, 1.0, 1.0, 1.0 ) );
        console->render();
    d3d->getDevice()->EndScene();
    d3d->updateScreen();

    // load in the vertex information
    faceInfo->load( &header, file, texInfo, lightMaps, d3d->getDevice() );

    // Tell the user that we just loaded in the vertex information
    // Also, tell the user that we are loading in the BSP tree
    d3d->clearScreen();
    d3d->getDevice()->BeginScene();
        console->printMessage( "Vertex Information Loaded!", D3DXCOLOR( 0.0, 1.0, 0.0, 1.0 ) );

        console->printMessage( "Loading Binary Space Partitioning Tree... ", D3DXCOLOR( 1.0, 1.0, 1.0, 1.0 ) );
        console->render();
    d3d->getDevice()->EndScene();
    d3d->updateScreen();

    // load in the BSP Tree
    bspTree->load( &header, file );

    // Tell the user that we just loaded in the BSP Tree
    // Also, tell the user that we are loading in the map Entities
    d3d->clearScreen();
    d3d->getDevice()->BeginScene();
        console->printMessage( "Binary Space Partitioning Tree Loaded!", D3DXCOLOR( 0.0, 1.0, 0.0, 1.0 ) );

        console->printMessage( "Loading Map Entities... ", D3DXCOLOR( 1.0, 1.0, 1.0, 1.0 ) );
        console->render();
    d3d->getDevice()->EndScene();
    d3d->updateScreen();

    // load in the map entities
    entities->load( &header, file );

    // Tell the user that we just loaded in the map entities
    // Also, tell the user that we just completed loading the entire .bsp map
    d3d->clearScreen();
    d3d->getDevice()->BeginScene();
        console->printMessage( "Map Entities Loaded!", D3DXCOLOR( 0.0, 1.0, 0.0, 1.0 ) );
        console->printMessage( "Map Finished Loading!", D3DXCOLOR( 0.0, 1.0, 0.0, 1.0 ) );
        console->printMessage( "------------------------", D3DXCOLOR( 1.0, 1.0, 1.0, 1.0 ) );
        console->render();
    d3d->getDevice()->EndScene();
    d3d->updateScreen();

    // Create the skybox and get the name of the skybox
    skyBox = new SkyBox();
    char *skyboxName = entities->getSkyBoxName();

    // See if we could find the skybox's name
    if ( skyboxName != NULL ) {
        // If we could, create the skybox and prepare it for rendering
        skyBox->load( d3d->getDevice(), string( "Q2/env/" ) + string( entities->getSkyBoxName() ) );
    } else {
        // If there is no skybox name, then just load in a file that will fail. This means
        // a black texture will be made instead of the normal skybox.
        skyBox->load( d3d->getDevice(), string( "Q2/env/" ) + string( "" ) );
    }

    // Set the camera's position to where the player appears in the map
    entities->setCameraPos( camera );


    // If the file was opened correctly, then close it
	if ( file != NULL ) {
        fclose( file );
    }

    // Load in the Pixel Shader
    mapShader->createEffect( d3d->getDevice(), "transform.fx", "MapShader" );

    vsTest = 0.0;


    if ( ddsTexture == NULL ) {
        D3DXCreateTextureFromFile( d3d->getDevice(), "ATDD/static_objects/machine/elevator.dds", &ddsTexture );
    }


    // Loading was successful!
	return true;
}



// Draws the map
void BSPMap::draw( LPDIRECT3DDEVICE9 device, Camera *camera, DrawingInfo *drawInfo ) {

    /**
     * Here's how the data is laid out of rendering:
     *  FaceInfo has a vertex buffer that is a set of polygons.
     *  A bsp face has an index into the vertex buffer.
     *  A bsp leaf has a set of bsp faces that it draws. A leaf also has data for
     *      determining whether or not its faces are visible within a viewing
     *      frustum. If not visible, then none of the faces are drawn.
     *  Leaves are grouped into clusters. Clusters control which of the other
     *      clusters should be drawn based on which cluster the camera is in.
     *      Clusters are the final tier of drawing information.
     */

    // The array of clusters
    vector< BSP::Cluster > *clusters = bspTree->getClusters();

    // This array is for determining which leaf to use for frustum culling
    vector< vector< BSP::Leaf * > > *clusterLeaves = bspTree->getClusterLeaves();

    // The visibility state of each cluster, found by traversing the BSP Tree
    //  with the camera's position.
    BitVector *visState = bspTree->getVisState( camera );


    // Set the Fixed Vertex Format (FVF) to the BSP FVF
    device->SetFVF( BSP_FVF );    // Set the vertex buffer used by Direct3D to the vertex buffer with all of
    //  the map vertex information in it.
    device->SetStreamSource( 0, faceInfo->getVertexBuffer(), 0, sizeof( D3D::Vertex ) );    // Special variables for using the map's pixel shader    UINT Pass, Passes;    // Setup backface culling (so polygons that are facing away from you aren't drawn)    device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

    // Set the useLightMap variable in the Pixel Shader
    mapShader->getEffect()->SetInt( "useLightMap", lMap );


    // The variables for how many polgons were drawn or culled.
    int totalPolygons = faceInfo->getNumVertices() / 3;
    int polygonsDrawn = 0;
    int numPVSCulled = 0;
    int numFrustumCulled = 0;

    D3DXMATRIX world;
    D3DXMATRIX view;
    D3DXMATRIX proj;

    device->GetTransform( D3DTS_WORLD, &world );
    device->GetTransform( D3DTS_VIEW, &view );
    device->GetTransform( D3DTS_PROJECTION, &proj );

    mapShader->getEffect()->SetMatrix( "worldViewProj", &( world * view * proj ) );
    mapShader->getEffect()->SetMatrix( "world", &( world ) );
    mapShader->getEffect()->SetMatrix( "view", &( view ) );
    mapShader->getEffect()->SetMatrix( "proj", &( proj ) );

    mapShader->getEffect()->SetFloat( "camPosX", -camera->pos->x );
    mapShader->getEffect()->SetFloat( "camPosY", -camera->pos->y );
    mapShader->getEffect()->SetFloat( "camPosZ", -camera->pos->z );

    vsTest += 0.1;
    mapShader->getEffect()->SetFloat( "vsTest", vsTest );



    mapShader->getEffect()->SetTexture( "modelTexture", ddsTexture );
    mapShader->getEffect()->SetTexture( "modelTexture", texInfo->getMegaTexture() );    // draw the map with the pixel shader    /*mapShader->getEffect()->Begin( &Passes, 0 );    for ( Pass = 0; Pass < Passes; Pass++ ) {
        mapShader->getEffect()->BeginPass( Pass );
        mapShader->getEffect()->SetInt( "useLightMap", 1 );        device->DrawPrimitive( D3DPT_TRIANGLELIST, 0,                               faceInfo->getNumVertices() / 3 );        mapShader->getEffect()->EndPass();    }
    mapShader->getEffect()->End();
    */


    // make sure lightmaps are enabled
    //mapShader->getEffect()->SetInt( "useLightMap", lMap );

    // Add in the number of polygons drawn
    //polygonsDrawn += ( faceInfo->getFaceStartIndex( i + 1 ) - faceInfo->getFaceStartIndex( i ) ) / 3;


    /**
     * This block is simply for drawing the map's polygons, while making sure no
     * polygons are drawn when they don't need to be drawn.
     */

    // For each cluster,

    for ( unsigned int c = 0; c < clusters->size(); ++c ) {
        // If the cluster is visible,
        if ( visState->getData( c ) ) {            // If it is, for each leaf in that cluster,            for ( unsigned int l = 0; l < ( *clusters )[ c ].size(); ++l ) {                // Is that leaf within the viewing frustum?                if ( camera->leafInFrustum( ( *clusterLeaves )[ c ][ l ] ) ) {                    // If it is, then draw the faces in that leaf                    // For each face in that leaf                    for ( unsigned int f = 0; f < ( *clusters )[ c ][ l ].size(); ++f ) {                        int i = ( *clusters )[ c ][ l ][ f ];                        // If it's not a skybox, then draw it.                        if ( !texInfo->getTexture( faceInfo->getTextureNum( i ) )->isSkyBox ) {                            // Setup the lightmap and texture for the pixel shader                            mapShader->getEffect()->SetTexture( "modelTexture", texInfo->getTexture( faceInfo->getTextureNum( i ) )->getTexture() );                            //mapShader->getEffect()->SetTexture( "modelTexture", texInfo->getMegaTexture() );                            //mapShader->getEffect()->SetTexture( "modelTexture", ddsTexture );                            mapShader->getEffect()->SetTexture( "lightMap", lightMaps->getTexture( i ) );                            //mapShader->getEffect()->SetTexture( "normalMap", normalMap );                            // If the texture doesn't use lightmaps (for example, water and lava), then disable lightmaps                            if ( !texInfo->getTexture( faceInfo->getTextureNum( i ) )->usesLightMaps ) {                                mapShader->getEffect()->SetInt( "useLightMap", 0 );                            }                            // draw the face with the pixel shader                            mapShader->getEffect()->Begin( &Passes, 0 );                            for ( Pass = 0; Pass < Passes; Pass++ ) {
                                mapShader->getEffect()->BeginPass( Pass );
                                device->DrawPrimitive( D3DPT_TRIANGLELIST,                                                       faceInfo->getFaceStartIndex( i ),                                                       ( faceInfo->getFaceStartIndex( i + 1 ) - faceInfo->getFaceStartIndex( i ) ) / 3 );                                mapShader->getEffect()->EndPass();                            }
                            mapShader->getEffect()->End();

                            // make sure lightmaps are enabled
                            mapShader->getEffect()->SetInt( "useLightMap", lMap );

                            // Add in the number of polygons drawn
                            polygonsDrawn += ( faceInfo->getFaceStartIndex( i + 1 ) - faceInfo->getFaceStartIndex( i ) ) / 3;
                        }                    }                } else {                    for ( unsigned int f = 0; f < ( *clusters )[ c ][ l ].size(); ++f ) {                        int i = ( *clusters )[ c ][ l ][ f ];                        // Add in the number of polygons frustum-culled                        numFrustumCulled += ( faceInfo->getFaceStartIndex( i + 1 ) - faceInfo->getFaceStartIndex( i ) ) / 3;                    }                }            }        } else {            for ( unsigned int l = 0; l < ( *clusters )[ c ].size(); ++l ) {                // For each face in that leaf                for ( unsigned int f = 0; f < ( *clusters )[ c ][ l ].size(); ++f ) {                    int i = ( *clusters )[ c ][ l ][ f ];                    // Add in the number of polygons Potentially-Visible-Set culled                    numPVSCulled += ( faceInfo->getFaceStartIndex( i + 1 ) - faceInfo->getFaceStartIndex( i ) ) / 3;                }            }        }    }    
    // Buffer for printing to. This is so the polygon variables can be printed into a string
    //  using the sprintf() function.
    char buf[ 128 ];

    // Tell the user about a problem with the percentages (exceed 100%)
    drawInfo->setLine( DrawingInfo::INFO_NOTE, "Note that some polygons may be drawn multiple times, so percentages may exceed 100%.", D3DXCOLOR( 1.0, 0.0, 0.0, 1.0 ) );

    // Print the number and percentage of polygons rendered
    sprintf( buf, "# of polygons rendered: %d / %d ( %f% )", polygonsDrawn, totalPolygons, 100.0 * float( polygonsDrawn ) / float( totalPolygons ) );
    drawInfo->setLine( DrawingInfo::INFO_POLYGONS_RENDERED, buf, D3DXCOLOR( 1.0, 1.0, 1.0, 1.0 ) );

    // Print the number and percentage of polygons culled by the Potentially-Visible-Set culling method
    sprintf( buf, "# of polygons PVS culled: %d / %d ( %f% )", numPVSCulled, totalPolygons, 100.0 * float( numPVSCulled ) / float( totalPolygons ) );
    drawInfo->setLine( DrawingInfo::INFO_NUM_PVS_CULLED, buf, D3DXCOLOR( 1.0, 1.0, 1.0, 1.0 ) );

    // Print the number and percentage of polygons culled by Frustum culling
    sprintf( buf, "# of polygons frustum culled: %d / %d ( %f% )", numFrustumCulled, totalPolygons, 100.0 * float( numFrustumCulled ) / float( totalPolygons ) );
    drawInfo->setLine( DrawingInfo::INFO_NUM_FRUSTUM_CULLED, buf, D3DXCOLOR( 1.0, 1.0, 1.0, 1.0 ) );


    // Render the map's skybox
    drawSkyBox( device, camera );};

/**
 * drawSkyBox() draws the sky around the camera.
 *  - device is a link to the DirectX object.
 *  - camera is the camera object.
 *
 * drawSkyBox() is called by draw()
 */
void BSPMap::drawSkyBox( LPDIRECT3DDEVICE9 device, Camera *camera ) {

    // Start by making the skybox centered on the camera
    D3DXMATRIX trans;
    D3DXMatrixTranslation( &trans, -camera->pos->x, -camera->pos->y, -camera->pos->z );
    device->SetTransform( D3DTS_WORLD, &( trans ) );

    // Disable culling
    device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // call the skybox's drawing method
    skyBox->show( device );

};


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
void BSPMap::enableLights( LPDIRECT3DDEVICE9 device, Point3f pos ) {
    entities->enableLights( device, pos );
};



/**
 * MAP_UI_NAMES is an array of strings that show the map name of a map
 * and its file name. This array is used for the MapSelector class, which
 * displays the formal map name of a map and its file name. For example,
 * it might show "Outer Base (base1)"
 */
const char *BSPMap::MAP_UI_NAMES[ NUM_MAPS ] = {
    "Outer Base (base1)",
    "Installation (base2)",
    "Comm Center (base3)",
    "Lost Station (train)",
    "Ammo Depot (bunk1)",
    "Supply Station (ware1)",
    "Warehouse (ware2)",
    "Main Gate (jail1)",
    "Detention Center (jail2)",
    "Security Complex (jail3)",
    "Torture Chambers (jail4)",
    "Gaurd House (jail5)",
    "Grid Control (security)",
    "Mine Entrance (mintro)",
    "Upper Mines (mine1)",
    "Bore Hole (mine2)",
    "Drilling Area (mine3)",
    "Lower Mines (mine4)",
    "Receiving Center (fact1)",
    "Processing Plant (fact2)",
    "Sudden Death (fact3)",
    "Power Plant (power1)",
    "The Reactor (power2)",
    "Cooling Facility (cool1)",
    "Toxic Waste Dump (waste1)",
    "Pumping Station 1 (waste2)",
    "Pumping Station 2 (waste3)",
    "Big Gun (biggun)",
    "Outer Hangar (hangar1)",
    "Inner Hangar (hangar2)",
    "Research Lab (lab)",
    "Launch Command (command)",
    "Outlands (strike)",
    "Comm Satellite (space)",
    "Outer Courts (city1)",
    "Lower Palace (city2)",
    "Upper Palace (city3)",
    "Inner Chamber (boss1)",
    "Final Showdown (boss2)"
};


/**
 * ORDERED_MAP_NAMES is an array of strings used as the file names for
 * all of the bsp map files in Quake 2. It is used for opening a map
 * file with an index, for example the MapSelector class returns an index
 * into this array for the map file name. Another use of this array is to
 * verify if a file name is a valid map name, when the map name is input
 * by the user using the console's "map" command
 */
const char *BSPMap::ORDERED_MAP_NAMES[ NUM_MAPS ] = {
    "base1",
    "base2",
    "base3",
    "train",
    "bunk1",
    "ware1",
    "ware2",
    "jail1",
    "jail2",
    "jail3",
    "jail4",
    "jail5",
    "security",
    "mintro",
    "mine1",
    "mine2",
    "mine3",
    "mine4",
    "fact1",
    "fact2",
    "fact3",
    "power1",
    "power2",
    "cool1",
    "waste1",
    "waste2",
    "waste3",
    "biggun",
    "hangar1",
    "hangar2",
    "lab",
    "command",
    "strike",
    "space",
    "city1",
    "city2",
    "city3",
    "boss1",
    "boss2"
};


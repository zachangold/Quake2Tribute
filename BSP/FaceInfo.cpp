//---------------------------------------------------------------------------

#pragma hdrstop

#include "FaceInfo.h"

/**
 * Switches the locations of the vertices in the vertex lump.
 * Quake 2 coordinates: (x, y, z) = Direct3D coordinates: (y, z, -x)
 */
void FaceInfo::transformVertices() {

    // The pointer to the first vertex
    Point3f *vertices = vertexLump.getData( 0 );

    // quake coords ( x, y, z ) == D3D coords ( y, z, -x )
    for ( int i = 0; i < vertexLump.getSize(); ++i ) {

        // set a temporary value
        Point3f temp = vertices[ i ];

        // Swap the values for that vertex
        vertices[ i ].x = temp.y;
        vertices[ i ].y = temp.z;
        vertices[ i ].z = -temp.x;
    }
};

/**
 * Loads in the bsp lumps necessary for the faces of the bsp map
 * load() loads in the vertex information.
 * load() with extra parameters loads in the vertex information, then calls
 * setupFaces().
 */
void FaceInfo::load( BSP::Header *header, FILE *mapFile ) {

    // Load in the vertices and transform them to Direct3D coordinates
    vertexLump.load( header, mapFile );
    transformVertices();

    // Load in the Edge Lump
    edgeLump.load( header, mapFile );

    // Load in the two Face Lumps
    faceLump.load( header, mapFile );
    faceEdgeLump.load( header, mapFile );
};

/**
 * Alternate loading method that calls load( bsp_header *, FILE * ) and loads
 * in the Faces and Vertex Buffer
 * load() loads in the vertex information.
 * load() with extra parameters loads in the vertex information, then calls
 * setupFaces().
 */
void FaceInfo::load( BSP::Header *header, FILE *file, TextureInfo *texInfo, LightMapInfo *lightMaps, LPDIRECT3DDEVICE9 device ) {
    // Load the bsp Lumps
    load( header, file );

    // Set up the D3DFaces and Vertex Buffer
    setupFaces( texInfo, lightMaps, device );
};

/**
 * Loads in the D3DFaces from the lumps that have been loaded in, and stores
 * them in a vertex buffer created with DirectX
 */
void FaceInfo::setupFaces( TextureInfo *texInfo, LightMapInfo *lightMaps, LPDIRECT3DDEVICE9 device ) {

    // make sure the vertex buffer is empty
    vertices.resize( 0 );

    // temporary D3DFace for loading in the faces
    D3D::Face temp;

    // Go through each of the bsp faces
    for ( int i = 0; i < faceLump.getSize(); ++i ) {

        // push back the start index for this face
        startIndices.push_back( vertices.size() );

        // load in the bsp_face vertex information
        temp.load( vertexLump.getData( 0 ), edgeLump.getData( 0 ),
                   faceEdgeLump.getData( 0 ), faceLump.getData( i ),
                   texInfo->getData( 0 ), texInfo->getTexture( faceLump.getData( i )->texture_info ) );

        lightMaps->loadLightMap( device, faceLump.getData( i ), &temp );

        temp.transformTexCoords();

        // Attach the vertex information to the end of the array of vertices
        temp.appendTo( &vertices );
    }

    // push back the last index for the vertex array
    startIndices.push_back( vertices.size() );


    // Set up Direct3D's vertex Buffer
    setupVertexBuffer( device );
};

/**
 * Puts all of the vertex and face information into a useable Direct3D vertex buffer
 */
void FaceInfo::setupVertexBuffer( LPDIRECT3DDEVICE9 device ) {

    // Allocate the vertex buffer
    device->CreateVertexBuffer( sizeof( D3D::Vertex ) * vertices.size(), 0, BSP_FVF, D3DPOOL_MANAGED, &vertexBuffer, NULL );

    // Pointer to the vertex buffer's contents    VOID* pVoid;
    // Lock the vertex buffer's contents so we can put in the vertices    vertexBuffer->Lock( 0, 0, ( void ** ) &pVoid, 0 );
    // Copy in the vertex information    memcpy( pVoid, &vertices[ 0 ], sizeof( D3D::Vertex ) * vertices.size() );
    // Unlock the vertex buffer so Direct3D can use it
    vertexBuffer->Unlock();    pVoid = NULL;

};


/**
 * Deletes all memory allocated by load() and setupFaces()
 */
void FaceInfo::unload() {

    // delete the memory allocated by vertics and startIndices
    vertices.resize( 0 );
    startIndices.resize( 0 );

    // delete the vertex buffer
    if ( vertexBuffer != NULL ) {
        vertexBuffer->Release();
        vertexBuffer = NULL;
    }

    // unload each of the lumps
    vertexLump.unload();
    edgeLump.unload();
    faceLump.unload();
    faceEdgeLump.unload();

};

//---------------------------------------------------------------------------
#pragma package(smart_init)

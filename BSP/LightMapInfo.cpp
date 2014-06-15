//---------------------------------------------------------------------------

#pragma hdrstop

#include "LightMapInfo.h"


/**
 * Constructor that prepares the lightmap for use. load() must be called
 * before this lightmap can be used to texture an object in Direct3D.
 */
LightMap::LightMap() {
    texture = NULL;
};


/**
 * Destructor that makes sure all memory allocated by this class has been
 * de-allocated.
 */
LightMap::~LightMap() {
    if ( texture != NULL ) {
        texture->Release();
    }
};


/**
 * load() method loads in a lightmap from pixel buffer "data", using the
 * information found in face. The d3dFace parameter is the face used for
 * triangulating a BSP Face. The d3dFace also keeps track of the texture
 * coordinates used for the lightmap, which is why it is needed to load
 * the lightmap. The "device" parameter is used in making a texture from
 * the lightmap with Direct3D.
 */
void LightMap::load( char *data, BSP::Face *face, D3D::Face *d3dFace, LPDIRECT3DDEVICE9 device ) {

    // compute the width and height of this lightmap
    width = ceil( d3dFace->getMaxU() / 16 ) - floor( d3dFace->getMinU() / 16 ) + 1;
    height = ceil( d3dFace->getMaxV() / 16 ) - floor( d3dFace->getMinV() / 16 ) + 1;

    // The maximum width or height of a lightmap is 16 pixels
    if ( width > 16 ) {
        width = 16;
    }
    if ( height > 16 ) {
        height = 16;
    }

    // Copy the data from the data pointer into a temporary Pixel buffer
    vector< Pixel > pixels;
    pixels.resize( width * height );

    data += face->lightmap_offset;

    Pixel *copy = ( Pixel * ) data;

    for ( int i = 0; i < width * height; ++i ) {
        pixels[ i ].r = copy->b;
        pixels[ i ].g = copy->g;
        pixels[ i ].b = copy->r;
        pixels[ i ].a = 255;

        data += 3;
        copy = ( Pixel * ) data;
    }

    // Give DirectX our lightmap information
    D3DLOCKED_RECT lr;

    device->CreateTexture(width,
		height, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture, NULL);

	texture->LockRect( 0, &lr, NULL, 0 );

	unsigned char* pRect = ( UCHAR* ) lr.pBits;

    // copy image to pRect
    memcpy( pRect, &pixels[ 0 ], width * height * sizeof( Pixel ) );

	texture->UnlockRect( 0 );

    //d3dFace->divideLMTexCoords( d3dFace->getMaxU() - d3dFace->getMinU(), d3dFace->getMaxV() - d3dFace->getMinV() );
    //d3dFace->shiftLMTexCoords( -d3dFace->getMinU(), -d3dFace->getMinV() );

    // Top-left corner is at luxel coord: (-8, -8)
    d3dFace->shiftLMTexCoords( -d3dFace->getMinU() - 8, -d3dFace->getMinV() - 8 );
    //d3dFace->shiftLMTexCoords( -d3dFace->getMinU() - 7, -d3dFace->getMinV() - 7 );
    //d3dFace->shiftLMTexCoords( -8, -8 );
    d3dFace->divideLMTexCoords( d3dFace->getMaxU() - d3dFace->getMinU(), d3dFace->getMaxV() - d3dFace->getMinV() );

};

// LIGHTMAPINFO METHODS


/**
 * Constructor initialises the object and prepares it for use.
 */
LightMapInfo::LightMapInfo() {
    lightMapData = NULL;

    lightMapNum = 0;
};


/**
 * Destructor makes sure that all lightmaps that have been loaded are
 * properly disposed of.
 */
LightMapInfo::~LightMapInfo() {
    if ( lightMapData != NULL ) {
        delete[] lightMapData;
        lightMapData = NULL;
    }

    unload();
};

/**
 * load() method prepares the lightmaps so they can be loaded individually
 * with loadLightMap(). It also loads in the lightmap lump of the BSP file
 * so the lightmaps can be loaded easily.
 */
void LightMapInfo::load( BSP::Header *header, FILE *mapFile ) {
    if ( lightMapData != NULL ) {
        delete[] lightMapData;
    }

    // Load in the lightmap data, storing it in lightMapData
    lightMapData = new char[ header->lump[ BSP_LIGHTMAP_LUMP ].length ];
    fseek( mapFile, header->lump[ BSP_LIGHTMAP_LUMP ].offset, 0 );
    fread( lightMapData, header->lump[ BSP_LIGHTMAP_LUMP ].length, 1, mapFile );

    // allocate memory for the lightmaps
    lightMaps.resize( header->lump[ BSP_FACE_LUMP ].length / sizeof( BSP::Face ) );

};

/**
 * loadLightMap() loads in the lightmap that belongs to parameter face.
 * It then modifies the lightmap texture coordinates, sending the lightmap
 * texture coordinates to parameter d3dFace. Parameter device is to help
 * create the Direct3D texture object that is associated with the lightmap.
 */
void LightMapInfo::loadLightMap( LPDIRECT3DDEVICE9 device, BSP::Face *face, D3D::Face *d3dFace ) {
    // load in a new lightmap
    lightMaps[ lightMapNum ].load( lightMapData, face, d3dFace, device );
    lightMapNum++;
};

/**
 * unload() method unloads all of the BSP map's lightmaps
 */
void LightMapInfo::unload() {

    // Go through each lightmap, deleting each one
    for ( unsigned int i = 0; i < lightMaps.size(); ++i ) {
        lightMaps[ i ].unload();
    }

    lightMapNum = 0;

    lightMaps.resize( 0 );
};

//---------------------------------------------------------------------------
#pragma package(smart_init)

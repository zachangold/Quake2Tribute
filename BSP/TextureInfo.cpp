//---------------------------------------------------------------------------

#pragma hdrstop

#include "TextureInfo.h"


/**
 * load() method loads in all of the map's textures and registers them
 * all with the Direct3D device parameter.
 */
void TextureInfo::load( BSP::Header *header, FILE *file, LPDIRECT3DDEVICE9 device ) {
    // load in the texture info structures
    texInfoLump.load( header, file );

    // load in every texture in the map
    for ( int i = 0; i < texInfoLump.getSize(); ++i ) {
        textures.loadNew( texInfoLump.getData( i )->texture_name, device );
    }

    textures.loadMegaTexture( device );
};


/**
 * unload() deletes any allocated memory
 */
void TextureInfo::unload() {
    texInfoLump.unload();
    textures.unload();
};

//---------------------------------------------------------------------------
#pragma package(smart_init)

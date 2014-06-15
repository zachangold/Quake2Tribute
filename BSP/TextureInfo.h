//---------------------------------------------------------------------------

#ifndef TextureInfoH
#define TextureInfoH


#include "BSPCommon.h"
#include "Lump.h"
#include "TextureLoader.h"

/**
 * The TextureInfo class contains all of the information on texturing the BSP Map.
 * Texturing just means applying an image to a surface. The textures are loaded in,
 * and are accessible through the getTexture() method.
 */
class TextureInfo {
    public:

        /**
         * Empty constructor does nothing
         */
        TextureInfo() {};

        /**
         * Destructor unloads any memory that was allocated in this object
         */
        ~TextureInfo() {
            unload();
        };

        /**
         * load() method loads in all of the map's textures and registers them
         * all with the Direct3D device parameter.
         */
        void load( BSP::Header *header, FILE *file, LPDIRECT3DDEVICE9 device );

        /**
         * unload() deletes any allocated memory
         */
        void unload();

        /**
         * Returns a BSP::TexInfo structure. This is mostly called by the FaceInfo
         * object for computing texture coordinates (how a texture is applied to
         * the object)
         */
        BSP::TexInfo *getData( int index ) {
            return texInfoLump.getData( index );
        };

        /**
         * Returns a loaded texture in the form of a WALImage. This is mostly used
         * by the BSPMap when drawing to texture each surface.
         */
        WALImage *getTexture( int index ) {
            return textures.getImage( index );
        };

        LPDIRECT3DTEXTURE9 getMegaTexture() {
            return textures.getMegaTexture();
        };

    private:
        // The texture information lump of the BSP file
        Lump< BSP::TexInfo, BSP_TEX_INFO_LUMP > texInfoLump;

        // The loaded Textures
        TextureLoader textures;

};


//---------------------------------------------------------------------------
#endif 

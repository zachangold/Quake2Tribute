//---------------------------------------------------------------------------

#ifndef TextureH
#define TextureH

#include <DirectX/d3d9.h>

#include "pcx.h"


/**
 * Texture Class:
 * This object provides a container for DirectX texture objects. It is purely
 * intended for convenience in texturing faces in DirectX. It loads in a single
 * .PCX file, then stores it as a DirectX texture
 */
class Texture
{
    private:

        // The texture's pixels
        unsigned char *texels;

        // The width and height of the texture
        int width, height;

        // The Direct3D Texture object
        LPDIRECT3DTEXTURE9 d3dTexture;

        /**
         * prepareD3DTexture registers the texture information with Direct3D.
         * the Direct3D texture is now usable by the main application.
         */
        void prepareD3DTexture( LPDIRECT3DDEVICE9 device );

        /**
         * prepareBlankTexture creates a texture that is coloured black. This is
         * used when a file could not be found.
         */
        void prepareBlankTexture( LPDIRECT3DDEVICE9 device );

    public:

        /**
         * Constructor that initialises the object and prepares it for use
         */
        Texture();

        /**
         * Destructor that de-allocates any allocated memory.
         */
        ~Texture();

        /**
         * Loads the PCX file specified by filename, then makes a texture with it
         * using parameter "device".
         */
        void loadImage( const char *filename, LPDIRECT3DDEVICE9 device );

        /**
         * unload() method that deallocates any memory allocated by load().
         */
        void unload();

        /**
         * reset() simply resets all of the fields in this object
         */
        void reset();

        /**
         * Returns the Direct3D texture object for rendering with this texture.
         */
        LPDIRECT3DTEXTURE9 getTexture();
};

//---------------------------------------------------------------------------
#endif
 

 

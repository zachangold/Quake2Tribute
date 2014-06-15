//---------------------------------------------------------------------------

#ifndef WALImageH
#define WALImageH

#include <DirectX/d3d9.h>
#include <iostream.h>
#pragma hdrstop


// Each WAL image's name is 32 characters long
#define WAL_IMAGE_NAME_SIZE 32


#pragma pack ( push, 1 )

/**
 * The WAL Image header specifies all of the properties of a WAL image.
 *
 * The WAL Image format is a propietary format used by ID software for storing
 * the textures of objects in the BSP Map. The data in the WAL Image contains
 * indices into a colour "palette", which contains the colours of the texture.
 */
typedef struct
{
    // The name of this WAL image.
    char    name[ WAL_IMAGE_NAME_SIZE ];        // name of the texture

    // The dimensions of the texture.
    unsigned int width;           // width (in pixels) of the largest mipmap level
    unsigned int height;          // height (in pixels) of the largest mipmap level

    // The offsets to the texture data
    int offset[ 4 ];       // byte offset of the start of each of the 4 mipmap levels

    // These values are unsused
    char    next_name[ WAL_IMAGE_NAME_SIZE ];   // name of the next texture in the animation

    unsigned int flags;           // ?
    unsigned int contents;        // ?
    unsigned int value;           // ?

} WALHeader;
#pragma pack ( pop )


/**
 * Function finds out if character string str contains character string substr.
 */
bool strContains( char *str, char *substr );

/**
 * WALImage class loads in an image file with the .WAL file extension and loads
 * it in as a Direct3D texture, usable for rendering the BSP Map later.
 */
class WALImage {
    private:

        // The WAL image's header
        WALHeader header;

        // The Direct3D texture made from this WALImage.
        LPDIRECT3DTEXTURE9 texture;

        unsigned char *data;

    public:
        /**
         * Constructor that prepares the object for loading
         */
        WALImage();


        /**
         * Destructor that makes sure all memory has been deallocated.
         */
        ~WALImage();

        /**
         * load() method loads in the WALImage with file name "fName", colour
         * palette "palette", index to that colour palette "rowNum", and sends
         * that loaded image as a texture to Direct3D Device "device".
         */
        bool load( char *fName, unsigned char *palette, int rowNum, LPDIRECT3DDEVICE9 device );

        /**
         * unload() method de-allocates any memory allocated by "load"
         */
        void unload() {
            if ( texture != NULL ) {
                texture->Release();
                texture = NULL;
            }
        };

        /**
         * getName() returns the name of this WALImage.
         */
        char *getName() {
            return header.name;
        };

        /**
         * Returns a Direct3D texture for use when drawing with this texture
         */
        LPDIRECT3DTEXTURE9 getTexture();

        /**
         * Returns the width of this texture
         */
        unsigned int getWidth() {
            return header.width;
        }

        /**
         * Returns the height of this texture
         */
        unsigned int getHeight() {
            return header.height;
        }

        unsigned char *getData() {
            return data;
        };

        // Two booleans - isSkyBox means that skyboxes are or are not drawn,
        // and usesLightMaps mean that lightmaps are or are not used for this texture
        bool isSkyBox;
        bool usesLightMaps;
};


//---------------------------------------------------------------------------
#endif
 
 


//---------------------------------------------------------------------------

#pragma hdrstop

#include "TextureLoader.h"

#include <math.h>


TextureLoader::TextureLoader() {
    // load in the Quake 2 colour palette
    // WAL textures do not contain definitive colours in RGB format.
    // Instead, they contain indices into this colour palette, which in turn
    // has the colours in RGB format.
    LoadFilePCX( "Q2/pics/colormap.pcx", &palette, NULL, NULL, false );

    megaTexture = NULL;
};

TextureLoader::~TextureLoader() {
    delete[] palette;


    unload();
};


/**
 * Loads in the .WAL Image under the directory "Q2/textures/"
 * If the image already exists in the data array, then keep a pointer
 *  to the already-loaded .WAL Image.
 * If the image does not already exist, load it in and store its name
 *  in alphabetically-ordered array of texture names.
 */
void TextureLoader::loadNew( char *name, LPDIRECT3DDEVICE9 device ) {

    // Go through each image, testing to see if the image was already loaded
    for ( unsigned int imageNum = 0; imageNum < loadedImages.size(); ++imageNum ) {

        // Compare the WALImages' names. If they are the same,
        if ( stringCompare( name, loadedImages[ imageNum ]->getName() ) == 0 ) {
            // make a pointer to it in textures
            textures.push_back( loadedImages[ imageNum ] );
            return;
        }
    }

    // If the image was not loaded previously, then load it in.

    // Load in the WAL image directly
    WALImage *temp = new WALImage();
    temp->load( name, palette, 319, device );

    loadedImages.push_back( temp );
    textures.push_back( temp );
};



/**
 * Goes through each character of two 32-character long strings, and if
 * the difference between them is NOT zero, then return the difference.
 * If the strings are equal, then return value is 0
 */
int TextureLoader::stringCompare( char *a, char *b ) {
    // Go through 32 characters (size of a WAL Image name)
    for ( int i = 0; i < WAL_IMAGE_NAME_SIZE; ++i ) {

        // if two characters at the same place are unequal, then return the difference.
        if ( ( *b ) != ( *a ) ) {
            return ( *b ) - ( *a );
        }

        // if not, go to the next character in each string
        a++;
        b++;
    }

    return 0;
};


/**
 * Goes through each of the WAL Images and unloads them all.
 */
void TextureLoader::unload() {
    for ( unsigned int i = 0; i < loadedImages.size(); ++i ) {
        delete loadedImages[ i ];
    }
    loadedImages.resize( 0 );

    if ( megaTexture != NULL ) {
        megaTexture->Release();
        megaTexture = NULL;
    }

    textures.resize( 0 );
};

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------

#pragma hdrstop

#include "Texture.h"


/**
 * Constructor that initialises the object and prepares it for use
 */
Texture::Texture() {
    texels = NULL;
    width = 0;
    height = 0;
    d3dTexture = NULL;
};

/**
 * reset() simply resets all of the fields in this object
 */
void Texture::reset() {
    texels = NULL;
    width = 0;
    height = 0;
    d3dTexture = NULL;
};

/**
 * unload() method that deallocates any memory allocated by load().
 */
Texture::~Texture() {
    unload();
};

/**
 * Loads the PCX file specified by filename, then makes a texture with it
 * using parameter "device".
 */
void Texture::loadImage( const char *filename, LPDIRECT3DDEVICE9 device ) {
    // If loading the PCX file went fine, then prepare the Direct3D texture normally.
    if ( LoadFilePCX( filename, &texels, &width, &height, false ) ) {
        prepareD3DTexture( device );
    } else {
        // If not, then prepare a black texture instead.
        prepareBlankTexture( device );
    }
};

/**
 * prepareBlankTexture creates a texture that is coloured black. This is
 * used when a file could not be found.
 */
void Texture::prepareBlankTexture( LPDIRECT3DDEVICE9 device ) {
    HRESULT rtn;
    D3DLOCKED_RECT lr;

    // Create the texture
    rtn = device->CreateTexture(1,
		1, 1, 0 ,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &d3dTexture, NULL);

    // Make sure the texture was made properly
	if ( FAILED( rtn ) )
	{
		return;
	}

    // prepare to copy the pixel data into the texture
	rtn = d3dTexture->LockRect( 0, &lr, NULL, 0 );
	if ( FAILED( rtn ) ){
		return;
	}


    // the pointer to the texture's data
	unsigned char* pRect = ( UCHAR* ) lr.pBits;

    // copy the single black pixel to the texture
    unsigned char blank[] = { 0, 0, 0, 255 };
    memcpy( pRect, blank, 4 );

    // tell Direct3D that we are no longer copying in texture information.
	rtn = d3dTexture->UnlockRect( 0 );
};

/**
 * prepareD3DTexture registers the texture information with Direct3D.
 * the Direct3D texture is now usable by the main application.
 */
void Texture::prepareD3DTexture( LPDIRECT3DDEVICE9 device ) {

    // If the pixel data was not loaded or the width or height is 0, then don't
    //  load the texture.
    if ( !texels || !width || !height ) {
        return;
    }

    HRESULT rtn;
    D3DLOCKED_RECT lr;

    // Create the texture
    rtn = device->CreateTexture(width,
		height, 1, 0 ,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &d3dTexture, NULL);

	if ( FAILED( rtn ) )
	{
		return;
	}

    // Prepare to copy in the image data
	rtn = d3dTexture->LockRect( 0, &lr, NULL, 0 );
	if ( FAILED( rtn ) ){
		return;
	}


    // Copy the image data into the texture object
	unsigned char* pRect = ( UCHAR* ) lr.pBits;
    memcpy( pRect, texels, width * height * 4 );

    // Tell Direct3D that we are done with copying image data in.
    d3dTexture->UnlockRect( 0 );
};

/**
 * unload() method that deallocates any memory allocated by load().
 */
void Texture::unload() {
    if ( texels != NULL ) {
        delete[] texels;
    }
    if ( d3dTexture != NULL ) {
        d3dTexture->Release();
    }

    // reset all of the class fields
    texels = 0;
    width = 0;
    height = 0;
    d3dTexture = NULL;

};


/**
 * Returns the Direct3D texture object for rendering with this texture.
 */
LPDIRECT3DTEXTURE9 Texture::getTexture() {
    return d3dTexture;
};


//---------------------------------------------------------------------------
#pragma package(smart_init)


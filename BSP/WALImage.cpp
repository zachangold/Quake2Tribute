//---------------------------------------------------------------------------

#pragma hdrstop

#include "WALImage.h"

using namespace std;


/**
 * Constructor that prepares the object for loading
 */
WALImage::WALImage() {
    texture = NULL;
};


/**
 * Destructor that makes sure all memory has been deallocated.
 */
WALImage::~WALImage() {
    unload();
    delete[] data;
};


/**
 * Function finds out if character string str contains character string substr.
 * Returns true if the substrung was found, false if not.
 */
bool strContains( char *str, char *substr ) {
    // find the lengths of the strings to make sure we don't check more than we
    // need to.
    int len = strlen( str );
    int substrLen = strlen( substr );

    // Go through each of the characters in str
    for ( int i = 0; i < len - substrLen; ++i ) {

        // If the first character of substr was found,
        if ( str[ i ] == substr[ 0 ] ) {
            bool strFound = true;

            // Check the next number of letters for equality (the number being
            // the length of substr)
            for ( int e = 0; e < substrLen; ++e ) {
                if ( str[ i + e ] != substr[ e ] ) {
                    strFound = false;
                }
            }

            // If nothing was unequal, then return true - the substring was found.
            if ( strFound ) {
                return true;
            }
        }
    }

    // If the substring was not found, then return false.
    return false;
};


/**
 * load() method loads in the WALImage with file name "fName", colour
 * palette "palette", index to that colour palette "rowNum", and sends
 * that loaded image as a texture to Direct3D Device "device".
 */
bool WALImage::load( char *fName, unsigned char *palette, int rowNum, LPDIRECT3DDEVICE9 device ) {

    // Fing the complete filename of the WAL image by adding the directory and file extension.
    string fileName = string( "Q2/textures/" ) + string( fName ) + string( ".wal" );

    // Determine if the WAL image is a skybox or if it uses lightmaps, based on its
    // file name
    isSkyBox = strContains( fName, "sky" );
    usesLightMaps = !( strContains( fName, "wter" ) || strContains( fName, "lava" ) || strContains( fName, "water" ) || strContains( fName, "sewer" ) );

    // data is the final pixel information to be sent to the Direct3D texture object.
    // packedData is the data that was found in the WAL file.
    unsigned char *packedData;

    // Open the WAL file
    FILE *fh = NULL;

    if ( ( fh = fopen( fileName.c_str(), "rb" ) ) == NULL ) {
        // if the file open failed, return false
        return false;
    }

    // read in the WAL header
    fread( &header, sizeof( WALHeader ), 1, fh );

    // create memory for the data
    packedData = new unsigned char[ header.width * header.height ];
    data = new unsigned char[ header.width * header.height * 4 ];

    // Read in the packed data
    fseek( fh, header.offset[ 0 ], 0 );
    fread( packedData, header.width * header.height, 1, fh );

    // unpack the packed data, placing the new data into the data array
    for ( unsigned int i = 0; i < header.width * header.height; ++i ) {
        data[ i * 4 ] = palette[ packedData[ i ] * 4 + rowNum * 256 * 4 ];
        data[ i * 4 + 1 ] = palette[ packedData[ i ] * 4 + 1 + rowNum * 256 * 4 ];
        data[ i * 4 + 2 ] = palette[ packedData[ i ] * 4 + 2 + rowNum * 256 * 4 ];
        data[ i * 4 + 3 ] = 255;
    }


    HRESULT rtn;
    D3DLOCKED_RECT lr;

    // Create the Direct3D texture
    rtn = device->CreateTexture(header.width,
		header.height, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture, NULL);

	if ( FAILED( rtn ) )
	{
		return false;
	}

    // Prepare to send the texture information to Direct3D
	rtn = texture->LockRect( 0, &lr, NULL, 0 );
	if ( FAILED( rtn ) ){
		return false;
	}


	unsigned char* pRect = ( UCHAR* ) lr.pBits;

    // Copy the image information to the Direct3D texture
    memcpy( pRect, data, header.width * header.height * 4 );

    // Stop sending texture information to the Direct3D texture object
	rtn = texture->UnlockRect( 0 );

    // delete the memory allocated to load the WAL Image data
    delete[] packedData;

    // close the file
    if ( fh != NULL ) {
        fclose( fh );
    }

    return true;
};

/**
 * Returns a Direct3D texture for use when drawing with this texture
 */
LPDIRECT3DTEXTURE9 WALImage::getTexture() {
    return texture;
};

//---------------------------------------------------------------------------
#pragma package(smart_init)
 



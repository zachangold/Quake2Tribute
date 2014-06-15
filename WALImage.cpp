//---------------------------------------------------------------------------

#pragma hdrstop

#include "WALImage.h"

using namespace std;

WALImage::WALImage() {
    texture = NULL;
};

WALImage::~WALImage() {
    if ( texture != NULL ) {
        texture->Release();
    }
};


bool WALImage::load( const char *fName, unsigned char *palette, int rowNum, LPDIRECT3DDEVICE9 device ) {

    rowNum = 115;

    string fileName = fName;
    //fileName = string( "Q2/textures/" ) + fileName + string( ".wal" );

    unsigned char *data;
    unsigned char *packedData;

    FILE *fh = NULL;

    if ( ( fh = fopen( fileName.c_str(), "rb" ) ) == NULL ) {
        // if the file open failed, return false
        return false;
    }

    fread( &header, sizeof( wal_header ), 1, fh );

    packedData = new unsigned char[ header.width * header.height ];
    data = new unsigned char[ header.width * header.height * 5 ];

    fseek( fh, header.offset[ 0 ], 0 );
    fread( packedData, header.width * header.height, 1, fh );

    for ( unsigned int i = 0; i < header.width * header.height; ++i ) {
        data[ i * 4 ] = palette[ packedData[ i ] * 4 + rowNum * 256 * 4 ];
        data[ i * 4 + 1 ] = palette[ packedData[ i ] * 4 + 1 + rowNum * 256 * 4 ];
        data[ i * 4 + 2 ] = palette[ packedData[ i ] * 4 + 2 + rowNum * 256 * 4 ];
        data[ i * 4 + 3 ] = 255;
    }


    HRESULT rtn;
    D3DLOCKED_RECT lr;

    rtn = device->CreateTexture(header.width,
		header.height, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture, NULL);

	if ( FAILED( rtn ) )
	{
		return false;
	}

	rtn = texture->LockRect( 0, &lr, NULL, 0 );
	if ( FAILED( rtn ) ){
		return false;
	}

	unsigned char* pRect = ( UCHAR* ) lr.pBits;

    // memcpy image to pRect
    memcpy( pRect, data, header.width * header.height * 4 );

	rtn = texture->UnlockRect( 0 );


    delete[] data;
    delete[] packedData;

    if ( fh != NULL ) {
        fclose( fh );
    }

    return true;
};

LPDIRECT3DTEXTURE9 WALImage::getTexture() {
    return texture;
};

//---------------------------------------------------------------------------
#pragma package(smart_init)
 



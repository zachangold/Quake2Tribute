//---------------------------------------------------------------------------

#ifndef WALImageH
#define WALImageH

#include <DirectX/d3d9.h>

#include <stdio.h>
#include <iostream.h>

#pragma pack ( push, 1 )
typedef struct
{

    char    name[ 32 ];        // name of the texture

    unsigned int width;           // width (in pixels) of the largest mipmap level
    unsigned int height;          // height (in pixels) of the largest mipmap level

    int offset[ 4 ];       // byte offset of the start of each of the 4 mipmap levels

    char    next_name[ 32 ];   // name of the next texture in the animation

    unsigned int flags;           // ?
    unsigned int contents;        // ?
    unsigned int value;           // ?

} wal_header;
#pragma pack ( pop )

class WALImage {
    private:
        wal_header header;

        LPDIRECT3DTEXTURE9 texture;

    public:
        WALImage();
        ~WALImage();

        bool load( const char *fName, unsigned char *palette, int rowNum, LPDIRECT3DDEVICE9 device );

        void unload() {
            texture->Release();
        };


        LPDIRECT3DTEXTURE9 getTexture();

        unsigned int getWidth() {
            return header.width;
        }
        unsigned int getHeight() {
            return header.height;
        }
};


//---------------------------------------------------------------------------
#endif
 
 


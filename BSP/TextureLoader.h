//---------------------------------------------------------------------------

#ifndef TextureLoaderH
#define TextureLoaderH

#include <vector.h>
#include <math.h>

#include "WALImage.h"
#include "pcx.h"

using namespace std;


// A cell in the megatexture holds a single image.
class TextureCell {
    public:
        TextureCell( WALImage *image ) {
            width = image->getWidth();
            height = image->getHeight();

            data = new unsigned char[ width * height * 4 ];
            memcpy( data, image->getData(), width * height * 4 );
        };

        TextureCell( TextureCell *cell ) {
            width = cell->width;
            height = cell->height;

            data = new unsigned char[ width * height * 4 ];
            memcpy( data, cell->data, width * height * 4 );
        };

        TextureCell( vector< TextureCell * > *cells, int width, int height ) {
            this->width = width;
            this->height = height;

            data = new unsigned char[ width * height * 4 ];
            //memcpy( data, cell->data, width * height * 4 );
            memset( data, 0, width * height * 4 );


            int rows = width / (*cells)[ 0 ]->width, columns = height / (*cells)[ 0 ]->height;

            int subCellW = (*cells)[ 0 ]->width;
            int subCellH = (*cells)[ 0 ]->height;

            for ( int r = 0; r < rows; ++r ) {
                for ( int c = 0; c < columns; ++c ) {
                    if ( cells->size() == 0 ) {
                        //cells->resize( 0 );
                        return;
                    }
                    copyToBuf( data, width, r * subCellW, c * subCellH, (*cells)[ cells->size() - 1 ]->data, subCellW, subCellH );
                    delete (*cells)[ cells->size() - 1 ];
                    cells->pop_back();
                }
            }

            //cells->resize( cells->size() - rows * columns );

        };


        ~TextureCell() {
            delete[] data;
        };

        /*bool hasSpace( int width, int height ) {
            // If this is the smallest cell size, then return whether or not this cell has data in it.
            if ( size == smallestSize ) {

            }
        };*/

        bool isSameSizeAs( WALImage *image ) {
            return ( image->getWidth() == width && image->getHeight() == height );
        };

        bool isSameSizeAs( int w, int h ) {
            return ( w == width && h == height );
        };

        void copyToBuf( unsigned char *buf, int bW, int x, int y ) {
            buf = buf + x * 4 + y * bW * 4;

            for ( int i = 0; i < height; ++i ) {
                memcpy( buf, data + width * i * 4, width * 4 );
                if ( i < height - 1 ) {
                    buf += bW * 4;
                }
            }
        };

        void copyToBuf( unsigned char *buf, int bW, int x, int y, unsigned char *cpyData, int dW, int dH ) {
            buf = buf + x * 4 + y * bW * 4;

            for ( int i = 0; i < dH; ++i ) {
                memcpy( buf, cpyData + dW * i * 4, dW * 4 );

                if ( i < dH - 1 ) {
                    buf += bW * 4;
                }
            }
        };
        
        unsigned char *data;

    private:

        int width, height;
};

typedef struct {
    float x, y;
} Point2f;

typedef struct {
    Point2f min, max;
} TexRect;

class MegaTexture {
    public:
        MegaTexture( int cellSize ) {
            this->cellSize = cellSize;
            width = 0;
            height = 0;
        };
        ~MegaTexture() {
            for ( int i = 0; i < cells.size(); ++i ) {
                for ( int e = 0; e < cells[ i ].size(); ++e ) {
                    delete cells[ i ][ e ];
                }
            }
        };

        void loadCell( WALImage *image ) {
            for ( int i = 0; i < cells.size(); ++i ) {
                if ( cells[ i ][ 0 ]->isSameSizeAs( image ) ) {
                    cells[ i ].push_back( new TextureCell( image ) );
                    return;
                }
            }

            int newCellNum = cells.size();
            cells.push_back();
            cells[ newCellNum ].push_back( new TextureCell( image ) );
        };

        LPDIRECT3DTEXTURE9 packCells( LPDIRECT3DDEVICE9 device ) {
            vector< TextureCell * > finalCells;
            for ( int i = 0; i < cells.size(); ++i ) {

                if ( cells[ i ][ 0 ]->isSameSizeAs( cellSize, cellSize ) ) {
                    for ( int e = 0; e < cells[ i ].size(); ++e ) {
                        finalCells.push_back( new TextureCell( cells[ i ][ e ] ) );
                        delete cells[ i ][ e ];
                    }
                    cells[ i ].resize( 0 );

                    break;
                }
            }

            for ( int i = 0; i < cells.size(); ++i ) {
                while ( cells[ i ].size() > 0 ) {
                    finalCells.push_back( new TextureCell( &cells[ i ], cellSize, cellSize ) );
                }
            } 

            LPDIRECT3DTEXTURE9 megaTexture = NULL;
            int rows = ( int ) pow( finalCells.size(), 0.5 );
            int columns = ( int ) ceil( ( float ) finalCells.size() / ( float ) rows );

            data = new unsigned char[ rows * cellSize * columns * cellSize * 4 ];

            for ( int i = 0; i < finalCells.size(); ++i ) {
                int x = ( i % rows ) * cellSize;
                int y = ( i / rows ) * cellSize;

                finalCells[ i ]->copyToBuf( data, rows * cellSize, x, y );
            }


            HRESULT rtn;
            D3DLOCKED_RECT lr;

            // Create the Direct3D texture
            rtn = device->CreateTexture( rows * cellSize,
                columns * cellSize, 1, 0,
                D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &megaTexture, NULL);
            //rtn = device->CreateTexture( cellSize,
            //    cellSize, 1, 0,
            //    D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &megaTexture, NULL);


            if ( FAILED( rtn ) ) {
                return NULL;
            }

            // Prepare to send the texture information to Direct3D
            rtn = megaTexture->LockRect( 0, &lr, NULL, 0 );
            if ( FAILED( rtn ) ){
                return NULL;
            }


            unsigned char* pRect = ( UCHAR* ) lr.pBits;

            // Copy the image information to the Direct3D texture
            memcpy( pRect, data, rows * cellSize * columns * cellSize * 4 );
            //memcpy( pRect, data, cellSize * cellSize * 4 );

            // Stop sending texture information to the Direct3D texture object
            rtn = megaTexture->UnlockRect( 0 );



            for ( int i = 0; i < finalCells.size(); ++i ) {
                delete finalCells[ i ];
            }
            delete[] data;

            return megaTexture;
        };

    private:
        // each vector< TextureCell * > has its own unique size.
        vector< vector< TextureCell * > > cells;

        vector< TexRect > texCoords;

        int cellSize;

        unsigned char *data;

        int width;
        int height;
};



/**
 * Handles all WAL image loading and use to reduce the amount of
 * memory used and loading time
 */
class TextureLoader {
    public:
        /**
         * Constructor that loads in the Colour palette for the WAL Images.
         */
        TextureLoader();

        /**
         * Destructor that deletes all memory that was loaded, and deletes the
         * colour palette that the WAL Images used.
         */
        ~TextureLoader();


        /**
         * Goes through each of the WAL Images and unloads them all.
         */
        void unload();

        /**
         * Loads in the .WAL Image under the directory "Q2/textures/"
         * If the image already exists in loadedImages, then keep a pointer to it
         *  in the "texture" field, which is an array
         * If the image does not already exist, load it in and store it in the
         *  loadedImages field, which is an array of WALImages
         */
        void loadNew( char *name, LPDIRECT3DDEVICE9 device );

        /**
         * Returns the .WAL image at index "texNum". Index goes by the first
         *  loaded image is at 0, the last loaded image is at the array maximum.
         */
        WALImage *getImage( int texNum ) {
            return textures[ texNum ];
        };

        void loadMegaTexture( LPDIRECT3DDEVICE9 device ) {
            int shortest = 1024;
            int largest = 0;

            int dataSize = 0;

            for ( int i = 0; i < loadedImages.size(); ++i ) {
                if ( loadedImages[ i ]->getWidth() < shortest ) {
                    shortest = loadedImages[ i ]->getWidth();
                }

                if ( loadedImages[ i ]->getHeight() < shortest ) {
                    shortest = loadedImages[ i ]->getHeight();
                }

                if ( loadedImages[ i ]->getWidth() > largest ) {
                    largest = loadedImages[ i ]->getWidth();
                }

                if ( loadedImages[ i ]->getHeight() > largest ) {
                    largest = loadedImages[ i ]->getHeight();
                }

                dataSize = dataSize + loadedImages[ i ]->getWidth() * loadedImages[ i ]->getHeight() * 4;
            }

            // The smallest cell size is shortest * shortest pixels.
            // The largest cell size is largest * largest pixels.
            // The final megatexture will be made up of cells of the largest cell size.

            MegaTexture texturePacker( largest );

            for ( int i = 0; i < loadedImages.size(); ++i ) {
                texturePacker.loadCell( loadedImages[ i ] );
            }

            megaTexture = texturePacker.packCells( device );

            return;
        };

        LPDIRECT3DTEXTURE9 getMegaTexture() {
            return megaTexture;
        };


    private:

        // the color palette used by the PCX images
        unsigned char *palette;

        /**
         * Returns a pointer to an already-loaded .WAL image that has the same
         * file name as parameter "name"
         */
        WALImage *getImage( char *name );

        /**
         * Goes through each character of two 32-character long strings, and if
         * the difference between them is NOT zero, then return the difference.
         * If the strings are equal, then return value is 0
         */
        int stringCompare( char *a, char *b );

        // Vector of non-redundant textures
        vector< WALImage * > loadedImages;

        // Vector of Pointers to the WAL images in sortedImages
        vector< WALImage * > textures;

        LPDIRECT3DTEXTURE9 megaTexture;
};


//---------------------------------------------------------------------------
#endif
 
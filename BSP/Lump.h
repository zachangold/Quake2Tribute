//---------------------------------------------------------------------------

#ifndef LumpH
#define LumpH

#include <stdio.h>

#include "BSPCommon.h"

/**
 * Holds the data for one BSP lump, and handles loading in the lump.
 * Lump data can be accessed through the use of the getData() method.
 *
 * NOTE THAT THIS LUMP CLASS DOES NOT WORK WITH THE ENTITY LUMP, OR ANY
 * OTHER LUMP WITH VARIABLE STORAGE SIZES.
 */
template< class LumpType, int lumpNum >
class Lump {
    public:
        Lump() {
            // Empty constructor
        };

        ~Lump() {
            // Empty destructor - the data array automatically de-allocates its
            //  memory when this object (Lump) is deleted
        };

        /**
         * load(): loads in the lump specified by template parameter lumpNum
         * unload(): Empties the data vector, freeing any memory allocated
         *          when loading
         */
        void load( BSP::Header *header, FILE *mapFile );
        void unload();

        /**
         * Returns the size of the vector of data
         */
        int getSize() {
            return data.size();
        };

        /**
         * Returns a pointer to the element in the data at element index
         */
        LumpType *getData( unsigned int index ) {
            if ( index < data.size() ) {
                return &data[ index ];
            } else {
                return NULL;
            }
        };

    private:

        // The data array
        vector< LumpType > data;

};

/**
 * Loads in the lump of a BSP Map:
 *     Lump type is template parameter "LumpType"
 *     Lump number is template parameter "lumpNum"
 *
 *     BSP map file to be read from is parameter mapFile
 *     BSP header is the bsp_header structure at the start of the BSP File
 */
template< class LumpType, int lumpNum >
void Lump< LumpType, lumpNum >::load( BSP::Header *header, FILE *mapFile ) {

    // Resize the data array such that it will fit the lump it needs to load
    data.resize( header->lump[ lumpNum ].length / sizeof( LumpType ) );

    // Navigate to the lump in the BSP map
    fseek( mapFile, header->lump[ lumpNum ].offset, 0 );

    // Read in the data in the BSP Map
    fread( &data[ 0 ], sizeof( LumpType ), data.size(), mapFile );
};


template< class LumpType, int lumpNum >
void Lump< LumpType, lumpNum >::unload() {
    // Shrink the data array to a size of zero
    data.resize( 0 );
};


//---------------------------------------------------------------------------
#endif

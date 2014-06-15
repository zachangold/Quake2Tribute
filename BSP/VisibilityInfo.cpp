//---------------------------------------------------------------------------

#pragma hdrstop

#include "VisibilityInfo.h"


/**
 * load() method loads in the visibility states for the map clusters
 */
void VisibilityInfo::load( BSP::Header *header, FILE *mapFile ) {

    // Read in the first 4 bytes of the BSP Map. This integer is the number
    // of clusters that are in the map.
    fseek( mapFile, header->lump[ BSP_VISIBILITY_LUMP ].offset, 0 );
    fread( &numClusters, 4, 1, mapFile );

    // Read in the visibility offsets. The number of visiblility offsets is equal
    // to the number of clusters.
    visOffsets.resize( numClusters );
    fseek(  mapFile, header->lump[ BSP_VISIBILITY_LUMP ].offset + 4, 0  );
    fread( &visOffsets[ 0 ], sizeof( BSP::VisOffset ), numClusters, mapFile );


    // Allocate memory for the visibility states and read them in 
    unsigned char *visData = new unsigned char[ visOffsets[ 0 ].pas - visOffsets[ 0 ].pvs ];
    fseek( mapFile, header->lump[ BSP_VISIBILITY_LUMP ].offset + visOffsets[ 0 ].pvs, 0 );
    fread( visData, visOffsets[ 0 ].pas - visOffsets[ 0 ].pvs, 1, mapFile );

    // For each cluster,
    for ( unsigned int i = 0; i < numClusters; ++i ) {

        // Load in the visibility information for that cluster
        BitVector temp;
        temp.readFromPtr( visData + ( visOffsets[ i ].pvs - visOffsets[ 0 ].pvs ), numClusters );

        // store the visibility information in the visStates array
        visStates.push_back( temp );
    }

    // De-allocate the memory allocated for the visibility states data
    delete[] visData;


    // The final visibility state is where all values are true. This value is used
    // when the player is outside of the map.
    BitVector temp;
    temp.resize( numClusters );
    visStates.push_back( temp );


};

/**
 * unload() method deletes all memory allocated by load()
 */
void VisibilityInfo::unload() {
    numClusters = 0;
    visOffsets.resize( 0 );
    visStates.resize( 0 );
};


//---------------------------------------------------------------------------
#pragma package(smart_init)

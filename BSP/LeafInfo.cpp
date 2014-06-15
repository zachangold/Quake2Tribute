//---------------------------------------------------------------------------

#pragma hdrstop

#include "LeafInfo.h"

void LeafInfo::load( BSP::Header *header, FILE *mapFile ) {
    leafLump.load( header, mapFile );
    leafFaceLump.load( header, mapFile );

    setupLeafFaces();
};

void LeafInfo::unload() {
    leafLump.unload();
    leafFaceLump.unload();

    //for ( unsigned int i = 0; i < leafFaces.size(); ++i ) {
    //    leafFaces[ i ].resize( 0 );
    //}
    //leafFaces.resize( 0 );
};


void LeafInfo::setupLeafFaces() {
    for ( int i = 0; i < leafLump.getSize(); ++i ) {
        //leafFaces.push_back();

        //for ( int l = 0; l < leafLump.getData( i )->num_leaf_faces; ++l ) {
        //    leafFaces[ i ].push_back( *leafFaceLump.getData( leafLump.getData( i )->first_leaf_face + l ) );
        //}
    }
};


//---------------------------------------------------------------------------
#pragma package(smart_init)

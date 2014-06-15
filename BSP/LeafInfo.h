//---------------------------------------------------------------------------

#ifndef LeafInfoH
#define LeafInfoH

#include "StandardHeaders.h"
#pragma hdrstop

#include "BSPCommon.h"
#include "Lump.h"

using namespace std;

class LeafInfo {
    public:
        LeafInfo() {

        };
        ~LeafInfo() {
            unload();
        };

        void load( BSP::Header *header, FILE *mapFile );
        void unload();

        int getLeafOfPoint( Point3f pos );

        vector< BSP::LeafFace > *getLeafFaces( int leafNum ) {
            //return &leafFaces[ leafNum ];
            return NULL;
        };

        int getNumLeaves() {
            //return leafFaces.size();
            return 0;
        };

    private:
        //vector< vector< BSP::LeafFace > > leafFaces;

        void setupLeafFaces();

        Lump< BSP::Leaf, BSP_LEAF_LUMP > leafLump;
        Lump< BSP::LeafFace, BSP_LEAF_FACE_LUMP > leafFaceLump;
};


//---------------------------------------------------------------------------
#endif
 
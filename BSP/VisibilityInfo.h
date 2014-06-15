//---------------------------------------------------------------------------

#ifndef VisibilityInfoH
#define VisibilityInfoH


#include "BSPCommon.h"

#include "Lump.h"


/**
 * The BitVector class is made to decompress the data in a bit stream. The
 * bit stream is found in the visibility lump, and is decompressed into an
 * array of boolean values. These boolean values determine whether or not
 * a cluster is to be drawn. There are the same number of bit vectors as there
 * are clusters.
 */
class BitVector {
    public:

        // Empty constructor and destructor do nothing. Behind the scenes, however,
        // the destructor deletes the memory allocated in the bits array.
        BitVector() {};
        ~BitVector() {};

        /**
         * This call simply resizes this bit vector, setting all of the values in
         * it to true.
         */
        void resize( int numBits ) {
            bits.resize( numBits );
            for ( int i = 0; i < numBits; ++i ) {
                bits[ i ] = true;
            }
        };

        /**
         * This call reads in and decompresses a bit vector from memory.
         * The data is stored in the bits array, usable for PVS culling later.
         */
        void readFromPtr( unsigned char *data, int numBits ) {
            int v = 0;

            this->numBits = numBits;
            //bits.resize( 0 );

            for (int c = 0; c < numBits; v++) {

                if ( data[ v ] == 0 ) {
                    // A byte that is equal to 0 means that the number of
                    // visibility states that are false is equal to the value
                    // of the next byte.
                    v++;
                    c += 8 * data[ v ];
                    for ( int i = 0; i < 8 * data[ v ]; ++i ) {
                        bits.push_back( false );
                    }

                } else {
                    for ( unsigned char bit = 1; bit != 0; bit *= 2, c++ ) {
                        // Append the value of that bit to the end of
                        // the "bits" array.
                        if ( data[ v ] & bit ) {
                            bits.push_back( true );
                        } else {
                            bits.push_back( false );
                        }
                    }
                }
            }

        };

        /**
         * Returns the visibility state of cluster #bitnum:
         *  - true if visible
         *  - false if not visible
         */
        bool getData( int bitNum ) {
            return bits[ bitNum ];
        };

    private:
        // The visibility state of each cluster
        vector< bool > bits;

        int numBits;
};

/**
 * The VisibilityInfo class loads in the visibility state for each cluster of a
 * BSP Map. Each cluster has a BitVector, dictating the visibility states of all
 * other clusters.
 */
class VisibilityInfo {
    public:

        // Empty constructor and destructor do nothing (except the destructor
        // deletes the memory allocated by visOffsets and visStates)
        VisibilityInfo() {};
        ~VisibilityInfo() {};

        /**
         * load() method loads in the visibility states for the map clusters
         */
        void load( BSP::Header *header, FILE *mapFile );

        /**
         * unload() method deletes all memory allocated by load()
         */
        void unload();

        /**
         * Returns the number of clusters in the BSP Map.
         */
        int getNumClusters() {
            return numClusters;
        };

        /**
         * Returns the visibility states of the other clusters for the cluster
         * specified by stateNum
         */
        BitVector *getVisState( int stateNum ) {
            // Make sure the cluster index is NOT less than 0. If it is, then tell
            // the application to draw ALL of the clusters in the map.
            if ( stateNum < 0 ) {
                return &visStates[ visStates.size() - 1 ];
            }
            return &visStates[ stateNum ];
        };

    private:

        // Number of clusters in the BSP Map
        unsigned int numClusters;

        // The offsets to the bit vectors loaded in by visStates
        vector< BSP::VisOffset > visOffsets;

        // The visibility states of each cluster
        vector< BitVector > visStates;
};


//---------------------------------------------------------------------------
#endif

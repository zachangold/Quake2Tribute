//---------------------------------------------------------------------------

#pragma hdrstop

#include "BSPTree.h"

namespace BSPTree {

    /**
     * The Constructor for the Node class has several parts:
     * - First, it creates itself, just like all other constructors. It stores
     *      a pointer to the BSP::Node and a pointer to the BSP::Plane for
     *      BSP tree traversal.
     * - Afterwards, it instantiates its "children", based on the information
     *      found in the BSP::Node array at index nodeNum.
     *      - If that child is a Leaf, then it creates a Leaf object and
     *          stores it.
     *      - If that child is another node, this constructor is called
     *          in a recursive style to instantiate the next node.
     * Each branch of the BSP Tree ends in a leaf.
     *
     * leafArray: the array of BSP Leaves from the BSP Map file
     * leafFaceArray: the array of BSP leaf faces from the BSP map file
     * nodeArray: the array of BSP Nodes
     * clusters: the array of BSP::Clusters that the leaves append their
     *      face index information to.
     * planeArray: the array of BSP::Planes that the Nodes use for the
     *      splitting plane.
     * nodeNum: the index of the node in the node array to be instantiated
     * clusterLeaves: the array of BSP::Leaves to add to for frustum culling
     *      purposes. The array is sorted by clusters, then by leaves (2 dimensions)
     */
    Node::Node( BSP::Leaf *leafArray, BSP::LeafFace *leafFaceArray, BSP::Node *nodeArray, vector< BSP::Cluster > *clusters, BSP::Plane *planeArray, int nodeNum, vector< vector< BSP::Leaf * > > *clusterLeaves ) {

        // Set the pointers to the BSP node and BSP plane
        bspNode = nodeArray + nodeNum;
        bspPlane = planeArray + bspNode->plane;

        // Instantiate the front branch
        if ( bspNode->front_child < 0 ) {
            front = new Leaf( leafArray, leafFaceArray, clusters, -( bspNode->front_child + 1 ), clusterLeaves );
        } else {
            front = new Node( leafArray, leafFaceArray, nodeArray, clusters, planeArray, bspNode->front_child, clusterLeaves );
        }

        // Instantiate the back branch
        if ( bspNode->back_child < 0 ) {
            back = new Leaf( leafArray, leafFaceArray, clusters, -( bspNode->back_child + 1 ), clusterLeaves );
        } else {
            back = new Node( leafArray, leafFaceArray, nodeArray, clusters, planeArray, bspNode->back_child, clusterLeaves );
        }

    };

    /**
     * Destructor deletes both of the children of this node
     */
    Node::~Node() {
        delete front;
        delete back;
    };                           

    
    /**
     * Returns the leaf that the parameter point is within.
     * Calls getLeaf() on the node/leaf that contains parameter point
     * (the containing leaf/node is found by the splitting plane).
     */
    Leaf *Node::getLeaf( Point3f point ) {

        // If this value is negative, use back child
        // If it is positive, use the front child
        float planeResult = point.x * bspPlane->normal.x + point.y * bspPlane->normal.y + point.z * bspPlane->normal.z - bspPlane->distance;

        if ( planeResult < 0.0 ) {
            return back->getLeaf( point );
        } else {
            return front->getLeaf( point );
        }
    };


    // LEAF METHODS

    /**
     * The Leaf constructor does the following:
     * - Instantiates this leaf
     * - Appends a set of indices to faces (a set of polygons) in the clusters
     *      array, and appends its BSP::Leaf to the appropriate cluster in clusterLeaves
     *
     * leafArray: The array of BSP::Leaves from the BSP Map file
     * leafFaceArray: The array of indices to the faces
     * clusters: The array of clusters, which contain the leaves' indices
     *      to the faces
     * leafNum: The index to the leafArray to use in loading the leaf
     * clusterLeaves: The array of indices to the BSP::Leaves for the
     *      purposes of frustum culling
     */
    Leaf::Leaf( BSP::Leaf *leafArray, BSP::LeafFace *leafFaceArray, vector< BSP::Cluster > *clusters, int leafNum, vector< vector< BSP::Leaf * > > *clusterLeaves ) {
        // set the pointer to the BSP leaf
        bspLeaf = leafArray + leafNum;

        // If the bspLeaf's cluster member
        if ( bspLeaf->cluster == -1 ) {
            // The leaf has no visibility information
            return;
        }

        // Add this leaf's information to the cluster's data
        ( *clusters )[ bspLeaf->cluster ].push_back();
        ( *clusterLeaves )[ bspLeaf->cluster ].push_back( bspLeaf );

        for ( int i = 0; i < bspLeaf->num_leaf_faces; ++i ) {
            ( *clusters )[ bspLeaf->cluster ][ ( *clusters )[ bspLeaf->cluster ].size() - 1 ].push_back( leafFaceArray[ bspLeaf->first_leaf_face + i ] );
        }
    };

    // Empty destructor does nothing
    Leaf::~Leaf() {};

    /**
     * Purpose: returns a pointer to this when Leaf::getLeaf() is called in
     *  the BSP array, giving the leaf the camera is in.
     * Return: Pointer to this
     */
    Leaf *Leaf::getLeaf( Point3f point ) {
        return this;
    };

    // TREE METHODS


    /**
     * Constructor sets all pointers to null, preparing the object for
     * later use.
     */
    Tree::Tree() {
        firstNode = NULL;
    };


    /**
     * Destructor deletes the memory allocated when loaded, if any remains
     */
    Tree::~Tree() {
        if ( firstNode != NULL ) {
            delete firstNode;
            firstNode = NULL;
        }
    };


    /**
     * load() method loads in and initialises the entire BSP Tree from the
     * BSP file.
     */
    void Tree::load( BSP::Header *header, FILE *mapFile ) {
        // load each of the lumps associated with the BSP Tree
        leafLump.load( header, mapFile );
        leafFaceLump.load( header, mapFile );

        nodeLump.load( header, mapFile );
        planeLump.load( header, mapFile );

        // Load in the visibility information
        visInfo.load( header, mapFile );

        // Resize the cluster information to fit the data that is going to be put
        // into it
        clusters.resize( visInfo.getNumClusters() );
        clusterLeaves.resize( visInfo.getNumClusters() );

        // Instantiate the first node, which recursively instantiates the entire
        // BSP tree
        firstNode = new Node( leafLump.getData( 0 ), leafFaceLump.getData( 0 ), nodeLump.getData( 0 ), &clusters, planeLump.getData( 0 ), 0, &clusterLeaves );

    };


    /**
     * unload() method deletes all memory allocated by the load() method
     */
    void Tree::unload() {
        // delete the first node, which recursively deletes the entire BSP Tree
        if ( firstNode != NULL ) {
            delete firstNode;
            firstNode = NULL;
        }

        // unload each of the lumps
        leafLump.unload();
        leafFaceLump.unload();
        planeLump.unload();
        nodeLump.unload();

        // unload the visibility information
        visInfo.unload();

        // unload the cluster information
        clusters.resize( 0 );
        clusterLeaves.resize( 0 );
    };
}
//---------------------------------------------------------------------------
#pragma package(smart_init)

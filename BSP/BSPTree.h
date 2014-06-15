//---------------------------------------------------------------------------

#ifndef BSPTreeH
#define BSPTreeH

#include "BSPCommon.h"
#include "BSPMath.h"
#include "VisibilityInfo.h"
#include "Lump.h"
#include "Camera.h"

/**
 * This module loads in and interprets the BSP Tree structure contained within a .BSP map.
 * The structuring of a BSP tree is as follows:
 *     - The BSP Tree is made up of nodes and leaves. The nodes are used for traversing
 *       the BSP tree, and the leaves are the final data structures for drawing the map
 *     - The BSP Tree Starts with one node. Each node has two "children", which can be
 *       either another node, or a leaf. Traversing the BSP tree (finding which node
 *       child to use) is done by comparing a point (usually the position of the camera)
 *       to a plane, stored by the bsp node structure.
 *     - Finally, the BSP tree is always terminated by a bsp leaf. After finding this
 *       leaf, the visible portions of the BSP map can be determined, thus improving
 *       rendering speed. The details of the Potentially-Visible-Set culling algorithm
 *       are further discussed in the files "VisibilityInfo.h" and "VisibilityInfo.cpp"
 */


/**
 * BSPTree namespace:
 * Contains:
 *     BSPTree::TreeChild: "child" type in BSP Tree
 *     BSPTree::Leaf: represents a BSP::Leaf (BSPCommon.h) in the BSP tree
 *     BSPTree::Node: represents a BSP::Node (BSPCommon.h) in the BSP tree
 *     BSPTree::Tree: represents the entire BSP Tree.
 */
namespace BSPTree {

    // This just tells the TreeChild class that the Leaf class exists
    class Leaf;

    /**
     * Base class TreeChild is a class that has only three methods: a constructor,
     * a destructor, and a getLeaf( Point3f ) routine. This class has two subclasses:
     * The Node, and the Leaf. The Node and Leaf classes override the destructor and
     * the getLeaf( Point3f ) routines. The Node class defines its two children as
     * pointers to this class, then instantiates them as either another node, or a leaf.
     */
    class TreeChild {
        public:
            // Constructor does nothing
            TreeChild() {};

            // Overloadable destructor does nothing
            virtual ~TreeChild() {};

            /**
             * getLeaf() method is must be overridden by a base class - see functions
             * at definitions in Node and Leaf classes.
             */
            virtual Leaf *getLeaf( Point3f point ) = 0;

    };

    /**
     * A Node in a BSP Tree represents a split in the BSP tree. The node has two
     * children, which can either be another node, or a leaf.
     */
    class Node : public TreeChild {
        public:
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
            Node( BSP::Leaf *leafArray, BSP::LeafFace *leafFaceArray,
                    BSP::Node *nodeArray, vector< BSP::Cluster > *clusters,
                    BSP::Plane *planeArray, int nodeNum,
                    vector< vector< BSP::Leaf * > > *clusterLeaves );

            /**
             * Destructor deletes both of the children of this node
             */
            ~Node();

            /**
             * Returns the leaf that the parameter point is within.
             * Calls getLeaf() on the node/leaf that contains parameter point
             * (the containing leaf/node is found by the splitting plane).
             */
            Leaf *getLeaf( Point3f point );

        private:
            // The Node's children
            TreeChild *front, *back;

            // The pointer to the node that this object represents
            BSP::Node *bspNode;

            // The pointer to the splitting plane of this node
            BSP::Plane *bspPlane;
    };

    /**
     * The BSPTree::Leaf class represents a leaf in the BSP tree. The BSP leaf is
     * the final step in traversing the BSP Tree. The leaf contains the cluster
     * number that it is within, which in turn lets us find which clusters are
     * visible by the camera. The leaf is treated like any other TreeChild subclass,
     * with the exception of tree traversal (see getLeaf())
     */
    class Leaf : public TreeChild {
        public:
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
            Leaf( BSP::Leaf *leafArray, BSP::LeafFace *leafFaceArray,
                    vector< BSP::Cluster > *clusters, int leafNum,
                    vector< vector< BSP::Leaf * > > *clusterLeaves );
            ~Leaf();

            /**
             * Traversing the BSP Tree goes by finding on which side of a plane the
             * parameter point is on, eventually terminating on a leaf. The leaf
             * is the final step in traversal, so it is the leaf that the point is
             * within.
             * Returns a pointer to this leaf.
             */
            Leaf *getLeaf( Point3f point );

            // The BSP::Leaf that this object represents
            BSP::Leaf *bspLeaf;
    };

    /**
     * The BSPTree::Tree object represents the entire BSP Tree.
     */
    class Tree {
        public:
            /**
             * Constructor sets all pointers to null, preparing the object for
             * later use.
             */
            Tree();

            /**
             * Destructor deletes the memory allocated when loaded, if any remains
             */
            ~Tree();

            /**
             * load() method loads in and initialises the entire BSP Tree from the
             * BSP file.
             */
            void load( BSP::Header *header, FILE *mapFile );

            /**
             * unload() method deletes all memory allocated by the load() method
             */
            void unload();

            /**
             * Returns an array of all of the map's clusters
             */
            vector< BSP::Cluster > *getClusters() {
                return &clusters;
            };

            /**
             * Returns an array sorted by clusters, then by leaf numbers. The drawing
             * code in BSPMap.cpp uses this information for frustum culling.
             */
            vector< vector< BSP::Leaf * > > *getClusterLeaves() {
                return &clusterLeaves;
            };


            /**
             * Returns the BitVector (VisibilityInfo.h) dictating the visibility
             * state of each of the clusters in the map by traversing the BSP tree
             * to find out which leaf the camera is in, and from that, which cluster
             * number. That cluster number is which BitVector to return in this call.
             */
            BitVector *getVisState( Camera *camera ) {

                // Find the leaf by using the camera's position, translated to Quake coordinates
                Leaf *leaf = getLeaf( getPoint( camera->pos->z * BSP::REVERSE_SCALE,
                                               -camera->pos->x * BSP::REVERSE_SCALE,
                                               -camera->pos->y * BSP::REVERSE_SCALE ) );

                // If the camera was not in any specific leaf, then just draw the
                // entire map.
                if ( leaf == NULL ) {
                    return visInfo.getVisState( visInfo.getNumClusters() );
                }

                // Otherwise, return the BitVector for the cluster that the camera is within
                return visInfo.getVisState( leaf->bspLeaf->cluster );
            };


            /**
             * Returns the leaf that a point is within by traversing the BSP Tree.
             */
            Leaf *getLeaf( Point3f point ) {
                return firstNode->getLeaf( point );
            };

        private:


            // The topmost and first node of the BSP Tree
            Node *firstNode;

            // The BSP node lump
            Lump< BSP::Node, BSP_NODE_LUMP > nodeLump;

            // The BSP leaf lump
            Lump< BSP::Leaf, BSP_LEAF_LUMP > leafLump;

            // The BSP leaf face lump
            Lump< BSP::LeafFace, BSP_LEAF_FACE_LUMP > leafFaceLump;

            // The BSP Plane lump
            Lump< BSP::Plane, BSP_PLANE_LUMP > planeLump;

            // The Visibility states of all of the clusters in the map
            VisibilityInfo visInfo;

            // The array of indices to faces, for drawing
            vector< BSP::Cluster > clusters;

            // The array of indices to leaves, for frustum culling
            vector< vector< BSP::Leaf * > > clusterLeaves;
    };

};

//---------------------------------------------------------------------------
#endif
 
#ifndef BSP_COMMON
#define BSP_COMMON

#include <vector.h>


#define BSP_ENTITY_LUMP         0  // MAP entity text buffer
#define BSP_PLANE_LUMP          1  // Plane array
#define BSP_VERTEX_LUMP         2  // Vertex array
#define BSP_VISIBILITY_LUMP     3  // Compressed PVS data and directory for all clusters
#define BSP_NODE_LUMP           4  // Internal node array for the BSP tree
#define BSP_TEX_INFO_LUMP       5  // Face texture application array
#define BSP_FACE_LUMP           6  // Face array
#define BSP_LIGHTMAP_LUMP       7  // Lightmaps
#define BSP_LEAF_LUMP           8  // Internal leaf array of the BSP tree
#define BSP_LEAF_FACE_LUMP      9  // Index lookup table for referencing the face array from a leaf
//#define BSP_LEAF_BRUSH_LUMP     10 // ?
#define BSP_EDGE_LUMP           11 // Edge array
#define BSP_FACE_EDGE_LUMP      12 // Index lookup table for referencing the edge array from a face
//#define BSP_MODEL_LUMP          13 // ?
//#define BSP_BRUSH_LUMP          14 // ?
//#define BSP_BRUSH_SIDE_LUMP     15 // ?
//#define BSP_POP_LUMP            16 // ?
//#define BSP_AREA_LUMP           17 // ?
//#define BSP_AREA_PORTAL_LUMP    18 // ?

/**
 * point3f structure:
 *     x, y, z: coordinates of a point
 *
 * Stores information for one vertex (or normal)
 */
typedef struct {
    float x, y, z;
} Point3f;

/**
 * Point3s structure:
 *     x, y, z: coordinates of a point
 *
 * Stores information for a corner of a bounding box in a BSP::Leaf structure
 */
typedef struct {
    short x, y, z;} Point3s;


// The Fixed Vertex Format of the bsp map. This is required by DirectX for rendering.
// This FVF specifies that there will be the x, y, z coordinates, the vertex's normal,
// and two sets of texture coordinates
#define BSP_FVF ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEX2 )

// This vertex type is associated with Direct3D rendering, so it is in the
// D3D namespace, and is accessible by D3D::Vertex
namespace D3D {
    typedef struct {
        // The coordinates of the point
        float x, y, z;

        // The coordinates of the point's normal
        float nx, ny, nz;

        // The Texture coordinate set for the main texture
        float u, v;

        // The Texture coordinates for the lightmap (lm) texture
        float lmu, lmv;
    } Vertex;
};


/**
 * These structures are associated with loading in and using a BSP File, so they
 * are put into the BSP namespace. They are accessible by BSP::<typename>.
 * Most of the structures in this namespace were taken from the "Quake 2 BSP
 * File Format" web page at "http://www.flipcode.com/archives/Quake_2_BSP_File_Format.shtml".
 * This web page provided useful information to reading in and using a BSP map,
 * but some of the code was left out, making this a valid project instead of a
 * simple copy-and-paste task.
 */
namespace BSP {

    // The scaling factor of the BSP Map.
    const float MAP_SCALE = 0.0005f;

    // The reverse of the scaling factor - this is for scaling a point's position
    //  into real bsp space from rendering bsp space
    const float REVERSE_SCALE = 1.0 / MAP_SCALE;


    /**
     * Lump structure:
     *     offset: The offset in bytes from the beginning of the file
     *     length: The size of the lump in bytes
     */
    typedef struct {
        int offset;        int length;
    } Lump;


    /**
     * Header structure:
     *     magic: The "identifying" number of the bsp format, equal to "IBSP"
     *     version: The version number of the BSP map (38 for quake 2 maps)
     *     lump: The lumps in the BSP map
     */
    typedef struct {        int magic;
        int version;

        Lump lump[ 19 ];
    } Header;



    // Define the type BSP::Vertex to be the same thing as a Point3f
    typedef Point3f Vertex;

    /**     * Edge structure:
     *     p1, p2: The indices of the vertices in the edge
     *
     * Connects two points from the map's vertex array
     */
    typedef struct {        unsigned short p1, p2;
    } Edge;


    /**
     * Face structure:
     *     plane: index of the bsp_plane the face is parallel to
     *     plane_side: set if the normal is parallel to the plane normal
     *     first_edge: index of the first edge in the face edge array
     *     num_edges: number of edges in the face
     *     texture_info: index of the texture info struct
     *     lightmap_styles: styles (bit flags) for the lightmaps
     *     lightmap_offset: offset of the lightmap (in bytes) in the lightmap lump     */
    typedef struct {
        unsigned short plane;        unsigned short plane_side;

        unsigned int first_edge;        unsigned short num_edges;

        unsigned short texture_info;

        unsigned char lightmap_styles[ 4 ];
        int lightmap_offset;
    } Face;


    /**
     * FaceEdge structure:
     *
     * indexes into the BSP Edge array from the BSP Face array
     */
    typedef int FaceEdge;



    /**
     * TexInfo structure:
     *
     * Stores information about a WAL texture
     */
    typedef struct {
        Point3f  u_axis;        float    u_offset;

        Point3f  v_axis;        float    v_offset;

        unsigned int flags;        unsigned int value;

        char texture_name[ 32 ];
        unsigned int next_texinfo;
    } TexInfo;    /**     * Leaf structure:     *  Holds information about a leaf in a bsp tree     *     */    typedef struct {        unsigned int brush_or;          // ?

        short cluster;           // -1 for cluster indicates no visibility information
        unsigned short area;              // ?

        Point3s bbox_min;          // bounding box minimums
        Point3s bbox_max;          // bounding box maximums

        unsigned short first_leaf_face;   // index of the first face (in the face leaf array)
        unsigned short num_leaf_faces;    // number of consecutive edges (in the face leaf array)

        unsigned short first_leaf_brush;  // ?
        unsigned short num_leaf_brushes;  // ?

    } Leaf;


    // LeafFace is an index into the BSP Face array from the BSP Leaf Array.
    // The BSP leaf array uses this array for which faces a leaf contains
    typedef unsigned short LeafFace;


    /**
     * The VisOffset structure is an offset to a bit vector (a set of on-bits and
     * off-bits) showing which clusters are visible or audible from other clusters.
     */
    typedef struct {

        unsigned int pvs;   // offset (in bytes) from the beginning of the visibility lump
        unsigned int pas;   // offset (in bytes) to the potentially audible set (unused)

    } VisOffset;


    /**
     * The node structure represents a node in the BSP tree. The main use of the
     * BSP Tree in this program is for Potentially-Visible-Set culling.
     */
    typedef struct {

        unsigned int   plane;             // index of the splitting plane (in the plane array)

        int    front_child;       // index of the front child node or leaf
        int    back_child;        // index of the back child node or leaf

        Point3s  bbox_min;          // minimum x, y and z of the bounding box
        Point3s  bbox_max;          // maximum x, y and z of the bounding box

        unsigned short   first_face;        // index of the first face (in the face array)
        unsigned short   num_faces;         // number of consecutive edges (in the face array)

    } Node;

    /**
     * The Plane structure defines a splitting plane in the BSP Tree, which allows
     * for traversal of the BSP tree. The Plane structure is used in the Node structure.
     */
    typedef struct {
        Point3f   normal;      // A, B, C components of the plane equation
        float     distance;    // D component of the plane equation
        unsigned int    type;        // ?

    } Plane;

    // These structures make a set of indices into a vertex array for the faces
    // which are drawn by the BSPMap class.
    typedef vector< LeafFace > ClusterLeaf;
    typedef vector< ClusterLeaf > Cluster;
};

#endif

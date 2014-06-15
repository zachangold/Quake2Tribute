//---------------------------------------------------------------------------

#ifndef FaceInfoH
#define FaceInfoH


#include "BSPCommon.h"
#include "Lump.h"
#include "D3DFace.h"
#include "TextureInfo.h"
#include "LightMapInfo.h"


using namespace std;

/**
 * The FaceInfo class handles loading in and processing the vertex data of a BSP
 * Map. The BSP Map renderer can access a Direct3D vertex buffer object, which is
 * the collection of all of the map's vertices.
 */
class FaceInfo {
    public:

        /**
         * Constructor prepares the object to be loaded
         */
        FaceInfo() {
            vertexBuffer = NULL;
        };

        /**
         * Destructor makes sure that all memory has been de-allocated
         */
        ~FaceInfo() {
            unload();
        };

        /**
         * Returns the Direct3D vertex buffer so the BSP Map can be rendered
         */
        LPDIRECT3DVERTEXBUFFER9 getVertexBuffer() {
            return vertexBuffer;
        };

        /**
         * returns the number of vertices in the vertex buffer
         */
        int getNumVertices() {
            return vertices.size();
        };

        /**
         * Returns the index of the first vertex of a face
         */
        int getFaceStartIndex( int faceNum ) {
            return startIndices[ faceNum ];
        };

        /**
         * Returns the number of faces in the BSP Map
         */
        int getNumFaces() {
            return faceLump.getSize();
        };

        /**
         * Returns which texture a face uses, indicated by faceNum
         */
        int getTextureNum( int faceNum ) {
            return faceLump.getData( faceNum )->texture_info;
        };             

        /**
         * load() loads in the vertex information.
         * load() with extra parameters loads in the vertex information, then calls
         * setupFaces().
         */
        void load( BSP::Header *header, FILE *file );
        void load( BSP::Header *header, FILE *file, TextureInfo *texInfo, LightMapInfo *lightMaps, LPDIRECT3DDEVICE9 device );

        /**
         * Deletes all memory allocated by load() and setupFaces()
         */
        void unload();

        /**
         * Sets up the rest of the rendering information, including triangulating
         * the BSP faces, and setting up the Direct3D vertex buffer.
         */
        void setupFaces( TextureInfo *texInfo, LightMapInfo *lightMaps, LPDIRECT3DDEVICE9 device );


    private:
        // Transforms the quake 2 coordinates (z is up-down) to DirectX coordinates (y is up-down)
        void transformVertices();

        // Creates DirectX's vertex buffer objects
        void setupVertexBuffer( LPDIRECT3DDEVICE9 device );


        // The vertices of the faces in the map
        vector< D3D::Vertex > vertices;

        // The index of the first vertex of each face
        vector< int > startIndices;

        // The Direct3D vertex buffer
        LPDIRECT3DVERTEXBUFFER9 vertexBuffer;

        // The lumps associated with the vertex information.
        Lump< BSP::Vertex, BSP_VERTEX_LUMP > vertexLump;
        Lump< BSP::Edge, BSP_EDGE_LUMP > edgeLump;
        Lump< BSP::Face, BSP_FACE_LUMP > faceLump;
        Lump< BSP::FaceEdge, BSP_FACE_EDGE_LUMP > faceEdgeLump;
};


//---------------------------------------------------------------------------
#endif
 
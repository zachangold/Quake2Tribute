//---------------------------------------------------------------------------

#ifndef MD2H
#define MD2H

#include <vector.h>
/**
 * MD2.h, MD2.cpp:
 *  These modules were not entirely written by Zach Angold. The only modifications
 * made were specializing the code to fit in with Direct3D, rather than OpenGL.
 *  The original code was found in the book "Beginning OpenGL Game Programming,
 * Second Edition" by Luke Benstead.
 */


#include <math.h>
#include <stdio.h>
#include <iostream.h>

#include "Texture.h"

#define ANIMATION_FPS 8.0f

using namespace std;
class MD2Instance;

#define MD2FVF ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )
typedef struct {
    float x, y, z;

    float nx, ny, nz;

    float u, v;
} D3DMD2Vertex;


typedef struct {
    unsigned char v[3];
    unsigned char lightNormalIndex;
} MD2Vertex;

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    float scale[3];
    float translate[3];
    char name[16];
    vector< MD2Vertex > MD2verts;
    vector< Vector3 > verts;
    vector< Vector3 > normals;
} MD2Frame;

typedef struct {
    short vertexIndex[3];
    short texCoordIndex[3];
} Triangle;

typedef struct {
    short vertexIndex[3];
} D3DTriangle;

typedef struct {
    short u, v;
} TexCoord;

typedef struct {
    char name[64];
} Skin;


typedef struct {
    char ident[4];             // Must be equal to "IDP2"
    int version;           // MD2 version

    int skinWidth;         // texture width
    int skinHeight;        // height of the texture
    int frameSize;         // size of one frame in bytes

    int numSkins;          // number of textures
    int numVertices;       // number of vertices
    int numTextureCoords;  // number of texture coordinates
    int numTriangles;      // number of triangles
    int numGLCmds;         // number of opengl commands
    int numFrames;         // total number of frames

    int skinOffset;        // offset to skin names (64 bytes each)
    int texCoordOffset;    // offset to s-t texture coordinates
    int triangleOffset;    // offset to triangles
    int frameOffset;       // offset to frame data
    int GLCmdOffset;       // offset to opengl commands
    int eofOffset;         // offset to end of file
} MD2Header;


class MD2Model {
    public:

        // Header
        MD2Header header;

        MD2Model();
        ~MD2Model();

        bool load( std::string fileName, LPDIRECT3DDEVICE9 device );
        bool loadTexture( std::string fileName, LPDIRECT3DDEVICE9 device );
        void generateBuffers( LPDIRECT3DDEVICE9 device );

        void setAnimation( int start, int end ) {
            frameNum = start;
            nextFrame = start + 1;

            startFrame = start;
            endFrame = end;
        };

        void setSkinNum( int skinN );

        void unload( LPDIRECT3DDEVICE9 device );
        void unloadTexture( LPDIRECT3DDEVICE9 device );
        void deleteBuffers( void );

        void update( float dt );
        void render( LPDIRECT3DDEVICE9 device );

        LPDIRECT3DVERTEXBUFFER9 normalVertexBuffer;
        void renderNormals( LPDIRECT3DDEVICE9 device );

        vector<Triangle> triangles;
        vector<Texture> skins;

    private:
        // Direct3D objects
        LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
        Texture *skin;


        // The frames of animation
        vector<MD2Frame> frames;

        vector<TexCoord> texCoords;

        void reorganizeVertices();
        static Vector3 normals[162];

        int skinNum;
        int frameNum;
        int nextFrame;
        int startFrame, endFrame;

        float interpolation;
};

//---------------------------------------------------------------------------
#endif



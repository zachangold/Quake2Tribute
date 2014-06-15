//---------------------------------------------------------------------------

#ifndef SkyBoxH
#define SkyBoxH

#include "Texture.h"
#include "BSPCommon.h"
#include <DirectX/d3d9.h>
#include <iostream.h>


/**
 * The Skybox has a special kind of vertex associated with it. This vertex has
 * two properties - the vertex's position, and how the Skybox texture is applied
 * on that vertex.
 */
#define SKYBOX_FVF ( D3DFVF_XYZ | D3DFVF_TEX1 )
typedef struct {
    // The vertex's position
    float x, y, z;

    // How the skybox texture is applied to that texture
    float u, v;
} SkyBoxVertex;

/**
 * The sky that surrounds the user/player is a set of six textures pasted onto the
 * faces of a cube. These six textures are basically the scenery of the game world.
 * This class is defined to create the skybox around the player and load it in,
 * with a rendering method that allows the skybox to be drawn easily.
 */
class SkyBox {
    public:
        /**
         * Destructor makes sure the textures and vertex buffer have been de-allocated.
         */
        ~SkyBox() {
            release();
        };

        /**
         * load() loads in the skybox from the Directory "Q2/env/", and creates
         * the skybox vertex buffer. After this method is called, the skybox can
         * be rendered to the screen.
         */
        void load( LPDIRECT3DDEVICE9 device, string baseName );

        /**
         * release() makes sure the Textures and vertex buffer have been de-allocated
         */
        void release();

        /**
         * show() renders the Skybox around the Camera, adding scenery to the outside
         * part of the BSP Map
         */
        void show( LPDIRECT3DDEVICE9 device );

    private:

        // The skybox textures - ordered from front, back, left, right, top, bottom
        Texture sides[ 6 ];

        // The Direct3D Vertex buffer for the vertices of the cube that the
        // Skybox textures are mapped onto
        LPDIRECT3DVERTEXBUFFER9 vBuffer;
};

//---------------------------------------------------------------------------
#endif

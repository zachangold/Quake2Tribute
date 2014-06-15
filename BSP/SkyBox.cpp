//---------------------------------------------------------------------------

#pragma hdrstop

#include "SkyBox.h"

const float SKYBOX_SIZE = 5000.0 * BSP::MAP_SCALE;


/**
 * load() loads in the skybox from the Directory "Q2/env/", and creates
 * the skybox vertex buffer. After this method is called, the skybox can
 * be rendered to the screen.
 */
void SkyBox::load( LPDIRECT3DDEVICE9 device, string baseName ) {

    // The vertices of the skybox
    float vertices[ 24 ][ 5 ] = {
        // front face
        {-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE, 1.0, 0.0},
        {SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE, 0.0, 0.0},
        {-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE, 1.0, 1.0},
        {SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE, 0.0, 1.0},

        // back face
        {-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE, 0.0, 0.0},
        {SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE, 1.0, 0.0},
        {-SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE, 0.0, 1.0},
        {SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE, 1.0, 1.0},

        // left face
        {SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE, 1.0, 0.0},
        {SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE, 1.0, 1.0},
        {SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE, 0.0, 0.0},
        {SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE, 0.0, 1.0},

        // right face
        {-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE, 0.0, 0.0},
        {-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE, 0.0, 1.0},
        {-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE, 1.0, 0.0},
        {-SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE, 1.0, 1.0},

        // top face
        {-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE, 1.0, 1.0},
        {SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE, 1.0, 0.0},
        {-SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE, 0.0, 1.0},
        {SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE, 0.0, 0.0},

        // bottom face
        {-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE, 1.0, 0.0},
        {SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE, 1.0, 1.0},
        {-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE, 0.0, 0.0},
        {SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE, 0.0, 1.0}
    };

    // Create a vertex buffer for the skybox vertices
    device->CreateVertexBuffer(sizeof(SkyBoxVertex) * 24,
                               0,                               SKYBOX_FVF,
                               D3DPOOL_MANAGED,
                               &vBuffer,
                               NULL);

    // Send the vertex information to the Direct3D vertex Buffer
    VOID* pVoid;
    vBuffer->Lock(0, 0, (void **)&pVoid, 0);    // locks v_buffer, the buffer we made earlier
    memcpy( pVoid, vertices, sizeof( SkyBoxVertex ) * 24 );
    vBuffer->Unlock();

    // load in each of the textures for the skybox
    sides[ 0 ].loadImage( ( baseName + string( "ft.pcx" ) ).c_str(), device );
    sides[ 1 ].loadImage( ( baseName + string( "bk.pcx" ) ).c_str(), device );
    sides[ 2 ].loadImage( ( baseName + string( "rt.pcx" ) ).c_str(), device );
    sides[ 3 ].loadImage( ( baseName + string( "lf.pcx" ) ).c_str(), device );
    sides[ 4 ].loadImage( ( baseName + string( "up.pcx" ) ).c_str(), device );
    sides[ 5 ].loadImage( ( baseName + string( "dn.pcx" ) ).c_str(), device );


};


/**
 * show() renders the Skybox around the Camera, adding scenery to the outside
 * part of the BSP Map
 */
void SkyBox::show( LPDIRECT3DDEVICE9 device ) {

    // Disable lighting
    device->SetRenderState( D3DRS_LIGHTING, FALSE );

    // setup rendering the skybox
    device->SetFVF( SKYBOX_FVF );
    device->SetStreamSource( 0, vBuffer, 0, sizeof( SkyBoxVertex ) );

    device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
    device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );


    // Go through each face of the skybox and render it
    for ( unsigned int c = 0; c < 6; ++c ) {
        device->SetTexture( 0, sides[ c ].getTexture() );        device->DrawPrimitive( D3DPT_TRIANGLESTRIP,                               c * 4, 2 );    }
    // Setup normal rendering
    device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
    device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
};


/**
 * release() makes sure the Textures and vertex buffer have been de-allocated
 */
void SkyBox::release() {

    // unload each of the textures
    for ( int i = 0; i < 6; ++i ) {
        sides[ i ].unload();
    }

    // delete the vertex Buffer.
    if ( vBuffer != NULL ) {
        vBuffer->Release();
    }
};

//---------------------------------------------------------------------------
#pragma package(smart_init)

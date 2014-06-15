//---------------------------------------------------------------------------

#pragma hdrstop

#include "D3DContext.h"


/**
 * D3DContext::init() creates the Direct3D device object in memory and sets
 * it up so the DirectX device can be drawn to by the main application.
 * After this, the device is ready to use for rendering
 */
void D3DContext::init( HWND hWnd, int width, int height ) {

    // Create the Direct3D structure
    d3d = Direct3DCreate9( D3D_SDK_VERSION );

    // Set the window handle and the screen dimensions
    this->hWnd = hWnd;
    screenWidth = width;
    screenHeight = height;

    // clear out direct3d's parameter list, then set all of the necessary attributes
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );

    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;

    // Set up the format of how DirectX draws to the screen
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = screenWidth;
    d3dpp.BackBufferHeight = screenHeight;

    // Setup depth buffering
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;


    // allow for fullscreen antialiasing
    //d3dpp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;


    // create a device using this information and the info from the d3dpp struct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &device);

    reset();

};

/**
 * reset() simply resets all of DirectX's states and behaviours to the
 * default values for this application.
 */
void D3DContext::reset() {
    device->ShowCursor( 0 );

    device->SetRenderState( D3DRS_ZENABLE, TRUE );    // turn on the z-buffer

    // Tell D3D to combine the lighting and the texture colours
    device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

    // Setup anisotropic filtering
    device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC );
    device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC );
    device->SetSamplerState( 0, D3DSAMP_MAXANISOTROPY, 16 );

    // Change a few of Direct3D's render states - for lighting and backface culling
    device->SetRenderState( D3DRS_LIGHTING, FALSE );    device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
    device->SetRenderState( D3DRS_NORMALIZENORMALS, FALSE );
};


/**
 * setupProjection() sets up Direct3D's projection matrix.
 * The projection matrix is just a way of transforming a point in 3d space
 * onto the user's screen, creating an illusion of a 3D world.
 */
void D3DContext::setupProjection( float fov ) {
    // The projection matrix
    D3DXMATRIX proj;

    // Setup the projection matrix
    D3DXMatrixPerspectiveFovLH(&proj,
                               D3DXToRadian( fov ),    // the horizontal field of view
                               ( FLOAT ) screenWidth / ( FLOAT ) screenHeight,    // aspect ratio
                               0.001f,    // the near view-plane
                               9.0f);    // the far view-plane

    // Set the projection matrix to the perspective matrix that we just made
    device->SetTransform( D3DTS_PROJECTION, &proj );

};

/**
 * setupWorldProjection() modifies Direct3D's world transformation matrix.
 * The world matrix modifies how objects in the world are transformed. This
 * could include where the object is, how it is rotated, and how big it is.
 */
void D3DContext::setupWorldTransform( float x, float y, float z,
                                    float rx, float ry, float rz,
                                    float sx, float sy, float sz ) {
    // The matrix variables for rotation, translation, and scale
    D3DXMATRIX entityMat, rotX, rotY, rotZ, trans, scale;

    // setup the rotations
    D3DXMatrixRotationX( &rotX, D3DXToRadian( rx ) );
    D3DXMatrixRotationY( &rotY, D3DXToRadian( ry ) );
    D3DXMatrixRotationZ( &rotZ, D3DXToRadian( rz ) );

    // Setup the scale
    D3DXMatrixScaling( &scale, sx, sy, sz );

    // setup the translation
    D3DXMatrixTranslation( &trans, x, y, z );

    // Setup the entity's final transform
    entityMat = rotX * rotY * rotZ * scale * trans;

    // Set the world matrix to the entity's matrix
    device->SetTransform( D3DTS_WORLD, &entityMat );
};


/**
 * updateScreen() Sends Direct3D a message that tells it to update the
 * screen by sending the image that was rendered earlier by the game engine.
 */
void D3DContext::updateScreen() {
    device->Present( NULL, NULL, NULL, NULL );
};

/**
 * clearScreen() refreshes DirectX's Depth buffer. Clearing the depth
 * buffer prevents objects that are behind other objects from being
 * rendered in front of the object that should be in the foreground.
 */
void D3DContext::clearScreen() {
    device->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
    //device->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
};

//---------------------------------------------------------------------------
#pragma package(smart_init)

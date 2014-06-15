//---------------------------------------------------------------------------

#ifndef RenderTargetH
#define RenderTargetH

#include <DirectX/d3d9.h>
#include <DirectX/d3dx9.h>
#include <math.h>
#include "BSPCommon.h"

#define RT_FVF ( D3DFVF_XYZ | D3DFVF_TEX1 )

class RenderTarget {
    public:
        RenderTarget() {
            surface = NULL;
            backBuffer = NULL;
            data = NULL;
            surfaceTexture = NULL;
            screenQuad = NULL;

            t = 0.0;
        };

        ~RenderTarget() {
            if ( surface != NULL ) {
                surface->Release();
                surface = NULL;
            }
            if ( backBuffer != NULL ) {
                backBuffer->Release();
                backBuffer = NULL;
            }
            if ( data != NULL ) {
                delete[] data;
                data = NULL;
            }
            if ( surfaceTexture != NULL ) {
                surfaceTexture->Release();
                surfaceTexture = NULL;
            }
            if ( screenQuad != NULL ) {
                screenQuad->Release();
                screenQuad = NULL;
            }
        };

        void init( LPDIRECT3DDEVICE9 device, int screenWidth, int screenHeight ) {
            device->GetRenderTarget( 0, &backBuffer );

            device->CreateRenderTarget( screenWidth, screenHeight, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0, FALSE, &surface, NULL );
            this->screenWidth = screenWidth;
            this->screenHeight = screenHeight;

            device->CreateTexture(screenWidth,
                                  screenHeight,
                                  1,
                                  D3DUSAGE_RENDERTARGET,
                                  D3DFMT_A8R8G8B8,
                                  D3DPOOL_DEFAULT,
                                  &surfaceTexture,
                                  NULL);

            //surfaceTexture->GetSurfaceLevel( 0, &surface );

            float vertices[ 4 ][ 5 ] = {
                { -screenWidth / 2, -screenHeight / 2, 3, 0.0, 1.0 },
                { -screenWidth / 2, screenHeight / 2, 3, 0.0, 0.0 },
                { screenWidth / 2, -screenHeight / 2, 3, 1.0, 1.0 },
                { screenWidth / 2, screenHeight / 2, 3, 1.0, 0.0 }
                /*
                {-5000.0 * BSP::MAP_SCALE, -5000.0 * BSP::MAP_SCALE, -5000.0 * BSP::MAP_SCALE, 1.0, 1.0},
                {5000.0 * BSP::MAP_SCALE, -5000.0 * BSP::MAP_SCALE, -5000.0 * BSP::MAP_SCALE, 0.0, 1.0},
                {-5000.0 * BSP::MAP_SCALE, 5000.0 * BSP::MAP_SCALE, -5000.0 * BSP::MAP_SCALE, 1.0, 0.0},
                {5000.0 * BSP::MAP_SCALE, 5000.0 * BSP::MAP_SCALE, -5000.0 * BSP::MAP_SCALE, 0.0, 0.0}
                */
            };

            // Create a vertex buffer for the skybox vertices
            device->CreateVertexBuffer(sizeof( float ) * 4 * 5,
                                       0,                                       RT_FVF,
                                       D3DPOOL_MANAGED,
                                       &screenQuad,
                                       NULL);

            // Send the vertex information to the Direct3D vertex Buffer
            VOID* pVoid;
            screenQuad->Lock( 0, 0, ( void ** ) &pVoid, 0 );    // locks v_buffer, the buffer we made earlier
            memcpy( pVoid, vertices, sizeof( float ) * 4 * 5 );
            screenQuad->Unlock();

            //data = new unsigned char[ screenWidth * screenHeight * 4 ];
        };

        void switchToRT( LPDIRECT3DDEVICE9 device ) {
            device->SetRenderTarget( 0, surface );
        };

        void switchToBB( LPDIRECT3DDEVICE9 device ) {
            device->SetRenderTarget( 0, backBuffer );
        };

        void drawRT( LPDIRECT3DDEVICE9 device ) {
            RECT DstRect;

            DstRect.left = 0;
            DstRect.top = 0;
            DstRect.right = screenWidth;
            DstRect.bottom = screenHeight;

            unsigned long dataSize = 0L;

            /*
            D3DXMATRIX proj;
            D3DXMATRIX rotx, roty, rotz;
            D3DXMatrixOrthoLH( &proj, ( ( float ) screenWidth ) + 1.6, screenHeight + 1, 0.0, 100.0 );
            //D3DXMatrixOrthoLH( &proj, screenWidth, screenHeight, 0.0, 100.0 );
            device->SetTransform( D3DTS_PROJECTION, &proj );

            D3DXMatrixRotationX( &rotx, 0 );
            D3DXMatrixRotationY( &roty, 0 );
            D3DXMatrixRotationZ( &rotz, 0 );
            device->SetTransform( D3DTS_VIEW, &( rotx * roty * rotz ) );

            // setup rendering the skybox
            device->SetFVF( RT_FVF );
            device->SetStreamSource( 0, screenQuad, 0, sizeof( float ) * 5 );
            device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

            //device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
            //device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
            device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
            device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );


            device->SetTexture( 0, surfaceTexture );
            device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );            */

            device->StretchRect( surface, NULL, backBuffer, &DstRect, D3DTEXF_LINEAR );

            DstRect.left += screenWidth / 2;
            DstRect.right += screenWidth / 2;
            //device->StretchRect( surface, NULL, backBuffer, &DstRect, D3DTEXF_LINEAR );
            DstRect.top += screenHeight / 2;
            DstRect.bottom += screenHeight / 2;
            //device->StretchRect( surface, NULL, backBuffer, &DstRect, D3DTEXF_LINEAR );
            DstRect.left -= screenWidth / 2;
            DstRect.right -= screenWidth / 2;
            //device->StretchRect( surface, NULL, backBuffer, &DstRect, D3DTEXF_LINEAR );
        };

    private:
        IDirect3DSurface9 *surface;
        IDirect3DSurface9 *backBuffer;

        LPDIRECT3DTEXTURE9 surfaceTexture;
        LPDIRECT3DVERTEXBUFFER9 screenQuad;


        unsigned char *data;

        // temporary
        float t;

        int screenWidth, screenHeight;
};


//---------------------------------------------------------------------------
#endif
 
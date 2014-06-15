//---------------------------------------------------------------------------

#pragma hdrstop

#include "Light.h"


// The light object is within the D3D namespace.
namespace D3D {

    /**
     * Resets the light structure by setting all of its fields to 0
     */
    void Light::reset() {
        ZeroMemory( &light, sizeof( light ) );    // clear out the light struct for use
        d3dLightNum = 0;
    };


    /**
     * enable() method tells the Direct3D device to use this light
     */
    void Light::enable( LPDIRECT3DDEVICE9 device, int lightNum ) {
        device->SetLight( lightNum, &light );    // send the light struct properties to light #lightNum
        device->LightEnable( lightNum, TRUE );    // turn on light #lightNum

        d3dLightNum = lightNum;
    };

    /**
     * Sends the light's information to Direct3D, updating any changes
     * the application has made to this light
     */
    void Light::refresh( LPDIRECT3DDEVICE9 device ) {
        device->SetLight( d3dLightNum, &light );    // send the light struct properties to light #lightNum
    };

    /**
     * disable() method tells Direct3D to stop using this light
     */
    void Light::disable( LPDIRECT3DDEVICE9 device ) {
        device->LightEnable( d3dLightNum, FALSE );
    };

    /**
     * Returns a pointer to the Direct3D light object so the programmer can
     * modify its properties.
     */
    D3DLIGHT9 *Light::getLight() {
        return &light;
    };
};
//---------------------------------------------------------------------------
#pragma package( smart_init )


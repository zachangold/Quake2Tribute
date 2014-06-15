//---------------------------------------------------------------------------

#pragma hdrstop

#include "Material.h"

namespace D3D {

    // Sets all data in the material structure to 0
    void Material::reset() {
        ZeroMemory( &material, sizeof( D3DMATERIAL9 ) );
    };

    /**
     * Enables the material with Direct3D. This helps with lighting - without
     * a material object, lighting in Direct3D is not possible.
     */
    void Material::enable( LPDIRECT3DDEVICE9 device ) {
        device->SetMaterial( &material );
    };

    /**
     * Sends the material's information to direct3d
     */
    void Material::refresh( LPDIRECT3DDEVICE9 device ) {
        device->SetMaterial( &material );
    };


    /**
     * Returns the Direct3D material object so its properties can be
     * modified in any way the programmer needs.
     */
    D3DMATERIAL9 *Material::getMaterial() {
        return &material;
    };
};


//---------------------------------------------------------------------------
#pragma package( smart_init )


//---------------------------------------------------------------------------

#pragma hdrstop

#include "Shader.h"


// The Shader class is part of the D3D namespace
namespace D3D {

    /**
     * Creates a pixel shader from the Technique "techniqueName" from the
     * file "fileName", using Direct3D Device "device"
     */
    void Shader::createEffect( LPDIRECT3DDEVICE9 device, char *fileName, char *techniqueName ) {
        // Create the FX from a .fx file.
        ID3DXBuffer* errors = 0;

        // load in the effect file
        D3DXCreateEffectFromFile( device, fileName,
                0, 0, 0, 0, &effect, &errors );

        // Report any errors with the effect file
        if ( errors ) {
            MessageBox( NULL, ( char * ) errors->GetBufferPointer(), 0, 0 );
        }

        // Set the rendering technique to the one specified by parameter techniqueName
        effect->SetTechnique( techniqueName );
    };

    
    /**
     * This method is used to get a pointer to the effect so the effect can
     * be used to render a model in the game world.
     */
    ID3DXEffect *Shader::getEffect() {
        return effect;
    };
};
//---------------------------------------------------------------------------
#pragma package( smart_init )


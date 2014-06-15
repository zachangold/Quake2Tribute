//---------------------------------------------------------------------------

#ifndef ShaderH
#define ShaderH

#include <DirectX/d3d9.h>
#include <DirectX/d3dx9.h>

#pragma hdrstop

/**
 * An explanation on the Shader File:
 *      Shaders are programs that run on the graphics card directly. The kind of
 *  shaders used in this application is the Pixel shader (others include the
 *  Vertex and Geometry shaders). The pixel shader is simply a function defined
 *  in a file with the ".fx" extension that returns a pixel's colour. The code
 *  in one of these looks almost exactly like C/C++, and that file includes
 *  an explanation of how the pixel shader operates on each pixel.
 */

// The Shader is an object to simplify the calls to create Pixel shaders, so
// the shader object is in the D3D namespace, accessible by "D3D::Shader".
namespace D3D {

    /**
     * The main purpose of using a pixel shader in this application is to combine
     * the regular texture and the lightmap texture of a face in the BSP Map.
     * Shaders can also be used for some unique visual effects, but in this
     * application, they are only used for combining the two textures.
     * For the purposes of this program, the terms "shader" and "effect" are
     * interchangeable.
     */
    class Shader {
        public:

            /**
             * Constructor that sets the shader object pointer to null, so it can be
             * instantiated later in the createEffect() routine.
             */
            Shader() {
                effect = NULL;
            };

            /**
             * Destructor that simply deletes the Direct3D shader object
             */
            ~Shader() {
                if ( effect != NULL ) {
                    effect->Release();
                }
            };

            /**
             * Creates a pixel shader from the Technique "techniqueName" from the
             * file "fileName", using Direct3D Device "device"
             */
            void createEffect( LPDIRECT3DDEVICE9 device, char *fileName, char *techniqueName );

            /**
             * This method is used to get a pointer to the effect so the effect can
             * be used to render a model in the game world.
             */
            ID3DXEffect *getEffect();

        private:

            // The Direct3D effect object
            ID3DXEffect *effect;


    };
};
//---------------------------------------------------------------------------
#endif
 


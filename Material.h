//---------------------------------------------------------------------------

#ifndef MaterialH
#define MaterialH


#include <DirectX/d3d9.h>

#pragma hdrstop


/**
 * The material class is mainly to add abstraction to the Direct3D material objects.
 * Therefore, it is placed in the D3D namespace, and is accessible by D3D::Material.
 */
namespace D3D {
    class Material {
        public:
            // Sets all data in the material structure to 0
            void reset();

            /**
             * Enables the material with Direct3D. This helps with lighting - without
             * a material object, lighting in Direct3D is not possible.
             */
            void enable( LPDIRECT3DDEVICE9 device );

            /**
             * Sends the material's information to direct3d
             */
            void refresh( LPDIRECT3DDEVICE9 device );


            /**
             * Returns the Direct3D material object so its properties can be
             * modified in any way the programmer needs.
             */
            D3DMATERIAL9 *getMaterial();

            /**
             * Constructor and Destructor clear the Direct3D material structure.
             */
            Material(){
                memset( &material, 0, sizeof( D3DMATERIAL9 ) );
            };
            ~Material(){
                memset( &material, 0, sizeof( D3DMATERIAL9 ) );
            };

        private:

            // The Direct3D material object
            D3DMATERIAL9 material;
    };
};
//---------------------------------------------------------------------------
#endif
 


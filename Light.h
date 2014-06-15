//---------------------------------------------------------------------------

#ifndef LightH
#define LightH

#include <DirectX/d3d9.h>

#pragma hdrstop

// The Light class is an object to add abstraction to a Direct3D object, and is
// added to the D3D namespace. This Light object is accessible by using "D3D::Light"
namespace D3D {

    /**
     * The light class adds a layer of abstraction to the Direct3D lighting object.
     * The Direct3D lighting object is accessible by using getLight(). The light
     * can then be enabled with Direct3D by using the enable() method.
     */
    class Light {
        public:
            /**
             * Constructor that resets the lighting structure by setting all
             * of its fields to 0
             */
            Light() {
                memset( &light, 0, sizeof( D3DLIGHT9 ) );
            };


            // Empty destructor does nothing
            ~Light() {};

            /**
             * Resets the light structure by setting all of its fields to 0
             */
            void reset();

            /**
             * enable() method tells the Direct3D device to use this light
             */
            void enable( LPDIRECT3DDEVICE9 device, int lightNum );

            /**
             * disable() method tells Direct3D to stop using this light
             */
            void disable( LPDIRECT3DDEVICE9 device );

            /**
             * Sends the light's information to Direct3D, updating any changes
             * the application has made to this light
             */
            void refresh( LPDIRECT3DDEVICE9 device );

            /**
             * Returns a pointer to the Direct3D light object so the programmer can
             * modify its properties.
             */
            D3DLIGHT9 *getLight();


        private:
            // The Direct3D light object
            D3DLIGHT9 light;

            // The light "slot" that this light uses - DirectX has a set of lights,
            // all of which fit into an array of lights in DirectX.
            int d3dLightNum;

    };
};
//---------------------------------------------------------------------------
#endif
 

//---------------------------------------------------------------------------

#ifndef LightMapInfoH
#define LightMapInfoH

#include "BSPCommon.h"
#include "D3DFace.h"

/**
 * Pixel structure is used to copy information from the loaded lightmap to the
 * Direct3D texture that the lightmap uses
 */
typedef struct {
    unsigned char r, g, b, a;
} Pixel;

/**
 * A Lightmap is an alternate texture used for static world lighting in Quake 2.
 * The lightmaps are found in the LightMap lump of the BSP Map. The lightmap's
 * pixel information is stored in a 24 bits per pixel format (8 bits for red,
 * green, and blue). Lightmap colours are combined with the plain texture of a
 * face in the BSP Map to achieve world lighting with the map's textures.
 */
class LightMap {
    public:

        /**
         * Constructor that prepares the lightmap for use. load() must be called
         * before this lightmap can be used to texture an object in Direct3D.
         */
        LightMap();

        /**
         * Destructor that makes sure all memory allocated by this class has been
         * de-allocated.
         */
        ~LightMap();

        /**
         * load() method loads in a lightmap from pixel buffer "data", using the
         * information found in face. The d3dFace parameter is the face used for
         * triangulating a BSP Face. The d3dFace also keeps track of the texture
         * coordinates used for the lightmap, which is why it is needed to load
         * the lightmap. The "device" parameter is used in making a texture from
         * the lightmap with Direct3D.
         */
        void load( char *data, BSP::Face *face, D3D::Face *d3dFace, LPDIRECT3DDEVICE9 device );


        /**
         * Returns the Direct3D texture object associated with this lightmap
         */
        LPDIRECT3DTEXTURE9 getTexture() {
            return texture;
        };

        /**
         * Returns the width of the lightmap
         */
        int getWidth() {
            return width;
        };

        /**
         * Returns the height of the lightmap
         */
        int getHeight() {
            return height;
        };

        /**
         * unload() method makes sure the Direct3D texture object has been deleted
         * properly.
         */
        void unload() {
            if ( texture != NULL ) {
                texture->Release();
                texture = NULL;
            }
        };

    private:
        // The dimensions of the lightmap
        int width;
        int height;

        // The Direct3D texture object associated with this lightmap
        LPDIRECT3DTEXTURE9 texture;

};


/**
 * The LightMapInfo object deals with storing all of the lightmaps from a BSP map.
 * The LightMaps loaded in separately, with each BSP face that the lightmaps are
 * used for (each lightmap is specific to 1 bsp face)
 */
class LightMapInfo {
    public:

        /**
         * Constructor initialises the object and prepares it for use.
         */
        LightMapInfo();

        /**
         * Destructor makes sure that all lightmaps that have been loaded are
         * properly disposed of.
         */
        ~LightMapInfo();

        /**
         * load() method prepares the lightmaps so they can be loaded individually
         * with loadLightMap(). It also loads in the lightmap lump of the BSP file
         * so the lightmaps can be loaded easily.
         */
        void load( BSP::Header *header, FILE *mapFile );

        /**
         * loadLightMap() loads in the lightmap that belongs to parameter face.
         * It then modifies the lightmap texture coordinates, sending the lightmap
         * texture coordinates to parameter d3dFace. Parameter device is to help
         * create the Direct3D texture object that is associated with the lightmap.
         */
        void loadLightMap( LPDIRECT3DDEVICE9 device, BSP::Face *face, D3D::Face *d3dFace );


        /**
         * Returns the Direct3D texture object for face #texNum. This is so that
         * the BSP Map renderer can use the texture for lighting.
         */
        LPDIRECT3DTEXTURE9 getTexture( int texNum ) {
            return lightMaps[ texNum ].getTexture();
        };

        /**
         * unload() method unloads all of the BSP map's lightmaps
         */
        void unload();

    private:

        // The array of BSP Lightmaps
        vector< LightMap > lightMaps;

        // The lightmap data loaded in from the BSP map
        char *lightMapData;

        // Which lightmap is to be loaded next by loadLightMap()
        int lightMapNum;
};


//---------------------------------------------------------------------------
#endif
 
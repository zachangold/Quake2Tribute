//---------------------------------------------------------------------------

#ifndef D3DFaceH
#define D3DFaceH


#include "BSPCommon.h"
#include "BSPMath.h"
#include "WALImage.h"

using namespace std;


namespace D3D {

    /**
     * Handles triangulation and coordinate generation for a bsp face structure.
     * After this class triangulates a face, its vertices are appended to a vertex
     * array, then those vertices are sent to Direct3D.
     */
    class Face {
        public:

            /**
             * Constructor that prepares the object for triangulating the BSP Faces
             */
            Face() {
                texture = NULL;
            };

            // Empty destructor does nothing
            ~Face() {};

            /**
             * Loads in the vertex information from BSP::Face structure. The other
             * arguments in this call are map information vital for making the face
             * into a set of usable triangles for DirectX to use.
             *
             * vertexArray: pointer to the array of BSP vertices
             * edgeArray: pointer to the array of BSP edges
             * faceEdgeArray: pointer to the array of BSP Face Edges
             * face: pointer to the BSP Face to be triangulated
             * texInfoArray: pointer the array of BSP Texture Info structures
             * textures: pointer to the array of WALImages
             */
            void load( Point3f *vertexArray, BSP::Edge *edgeArray, BSP::FaceEdge *faceEdgeArray,
                        BSP::Face *face, BSP::TexInfo *texInfoArray, WALImage *textures );


            /**
             * appendTo() puts the vertex information onto the end of vertexBuffer.
             */
            void appendTo( vector< D3D::Vertex > *vertexBuffer );


            /**
             * The following four methods are involved in lightmap generation.
             * The size of a lightmap is based on the maximum and minimum texture
             * coordinates of the vertices of a face, so these four methods return
             * the maximum and minimum U and V values accordingly ( U
             * and V are basically the X and Y of a 2-dimensional texture)
             */
            // For max. and min. U value
            float getMinU();
            float getMaxU();

            // For max. and min. V value
            float getMinV();
            float getMaxV();

            /**
             * Recall that Lightmaps are specific to a single face in the .bsp map.
             * Since they are applied once and only once to that face, the lightmap
             * texture coordinates must be transformed such that the texture is
             * not repeated multiple times across a face.
             */
            void divideLMTexCoords( float width, float height );

            /**
             * After the texture coordinates for a face's lightmaps are scaled properly,
             * the lightmap texture coordinates need to be "shifted" such that the minimum
             * texture coordinate for a lightmap is 0.0. Without this, problems like this
             * could happen:
             *
             * For example, the minimum lightmap texture coordinate could be ( -0.5, -0.5 )
             * Intended Result:             Error Result:
             *  A | B                        D | C
             *  -   -                        -   -
             *  C | D                        B | A
             */
            void shiftLMTexCoords( float u, float v );


            /**
             * Texture coordinates in Quake 2 are done where an image goes from
             * 0 to the image's width or height, whereas texture coordinates in
             * Direct3D range from 0.0 to 1.0. Calling transformTexCoords() modifies
             * the texture coordinates so they conform to how Direct3D uses its
             * texture coordinates. Without this, the textures would repeat many
             * times on a face, rather than the few times that they are supposed to.
             */
            void transformTexCoords();


        private:

            // The vertex information for this face
            vector< D3D::Vertex > vertices;

            // The texture for this face
            WALImage *texture;

    };


};
//---------------------------------------------------------------------------
#endif

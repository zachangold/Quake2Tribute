//---------------------------------------------------------------------------

#pragma hdrstop

#include "D3DFace.h"

namespace D3D {
    /**
     * Appends the vertex information of this face to the end of the vector
     * pointed to by vertexBuffer.
     */
    void Face::appendTo( vector< D3D::Vertex > *vertexBuffer ) {
        // Add the number of vertices needed
        int startIndex = vertexBuffer->size();
        vertexBuffer->resize( vertexBuffer->size() + vertices.size() );

        // Copy the vertex information
        for ( unsigned int i = startIndex; i < vertexBuffer->size(); ++i ) {
            ( *vertexBuffer )[ i ] = vertices[ i - startIndex ];
        }
    };


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
    void Face::load( Point3f *vertexArray, BSP::Edge *edgeArray, BSP::FaceEdge *faceEdgeArray,
                        BSP::Face *face, BSP::TexInfo *texInfoArray, WALImage *texture ) {

        // This information is to get which edges a face uses, and from that
        // which vertices it uses
        BSP::FaceEdge *firstFaceEdge = faceEdgeArray + face->first_edge;
        BSP::Edge *polygonEdges = new BSP::Edge[ face->num_edges ];

        // Remember the texture for later, when the lightmaps are being generated.
        this->texture = texture;

        // Copy the edges indexed by the face edges pointed to by firstFaceEdge to polygonEdges
        for ( int i = 0; i < face->num_edges; ++i ) {
            // if the index is negative, switch p1 and p2
            if ( firstFaceEdge[ i ] < 0 ) {
                polygonEdges[ i ].p1 = edgeArray[ -firstFaceEdge[ i ] ].p2;
                polygonEdges[ i ].p2 = edgeArray[ -firstFaceEdge[ i ] ].p1;
            } else {
                polygonEdges[ i ] = edgeArray[ firstFaceEdge[ i ] ];
            }

        }



        // Copy the final vertex information to vertices
        vertices.resize( ( face->num_edges - 2 ) * 3 );

        for ( int i = 0; i < face->num_edges - 2; ++i ) {

            // The first point of the triangle is always the first point of the polygon
            vertexCopy( &vertices[ i * 3 ], &vertexArray[ polygonEdges[ 0 ].p1 ] );
            getTexCoord( &vertices[ i * 3 ], &texInfoArray[ face->texture_info ], texture );

            // The other two vertices are on the polygon
            vertexCopy( &vertices[ i * 3 + 1 ], &vertexArray[ polygonEdges[ i + 1 ].p1 ] );
            getTexCoord( &vertices[ i * 3 + 1 ], &texInfoArray[ face->texture_info ], texture );

            vertexCopy( &vertices[ i * 3 + 2 ], &vertexArray[ polygonEdges[ i + 2 ].p1 ] );
            getTexCoord( &vertices[ i * 3 + 2 ], &texInfoArray[ face->texture_info ], texture );

            // Get the normals for each of the three points of the triangle
            getNormal( &vertices[ i * 3 ] );
        }


        delete[] polygonEdges;

    };

    /**
     * Returns the minimum U coordinate of the vertices of a face
     */
    float Face::getMinU() {
        float rtn = vertices[ 0 ].lmu;

        // Go through each of the vertices to find the appropriate value to
        // return from this method
        for ( unsigned int i = 1; i < vertices.size(); ++i ) {
            if ( vertices[ i ].lmu < rtn ) {
                rtn = vertices[ i ].lmu;
            }
        }

        return rtn;
    };

    /**
     * Returns the maximum U coordinate of the vertices of a face
     */
    float Face::getMaxU() {
        float rtn = vertices[ 0 ].lmu;

        // Go through each of the vertices to find the appropriate value to
        // return from this method
        for ( unsigned int i = 1; i < vertices.size(); ++i ) {
            if ( vertices[ i ].lmu > rtn ) {
                rtn = vertices[ i ].lmu;
            }
        }

        return rtn;
    };

    /**
     * Returns the minimum V coordinate of the vertices of a face
     */
    float Face::getMinV() {
        float rtn = vertices[ 0 ].lmv;

        // Go through each of the vertices to find the appropriate value to
        // return from this method
        for ( unsigned int i = 1; i < vertices.size(); ++i ) {
            if ( vertices[ i ].lmv < rtn ) {
                rtn = vertices[ i ].lmv;
            }
        }

        return rtn;
    };

    /**
     * Returns the maximum V coordinate of the vertices of a face
     */
    float Face::getMaxV() {
        float rtn = vertices[ 0 ].lmv;

        // Go through each of the vertices to find the appropriate value to
        // return from this method
        for ( unsigned int i = 1; i < vertices.size(); ++i ) {
            if ( vertices[ i ].lmv > rtn ) {
                rtn = vertices[ i ].lmv;
            }
        }

        return rtn;
    };

    /**
     * Divides each of the face's vertices' lightmap texture coordinates by
     * the values specified (width, height).
     * Recall that Lightmaps are specific to a single face in the .bsp map.
     * Since they are applied once and only once to that face, the lightmap
     * texture coordinates must be transformed such that the texture is
     * not repeated multiple times across a face.
     */
    void Face::divideLMTexCoords( float width, float height ) {

        // Divide each of the texture coordinates by width and height
        for ( unsigned int i = 0; i < vertices.size(); ++i ) {
            vertices[ i ].lmu /= width;
            vertices[ i ].lmv /= height;
        }
    };

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
    void Face::shiftLMTexCoords( float u, float v ) {

        // Shifts the lightmap texture by ( u, v )
        for ( unsigned int i = 0; i < vertices.size(); ++i ) {
            vertices[ i ].lmu += u;
            vertices[ i ].lmv += v;
        }
    };

    
    /**
     * Texture coordinates in Quake 2 are done where an image goes from
     * 0 to the image's width or height, whereas texture coordinates in
     * Direct3D range from 0.0 to 1.0. Calling transformTexCoords() modifies
     * the texture coordinates so they conform to how Direct3D uses its
     * texture coordinates. Without this, the textures would repeat many
     * times on a face, rather than the few times that they are supposed to.
     */
    void Face::transformTexCoords() {

        // Divide each of the texture coordinates by the width and height
        // of the texture of that face.
        for ( unsigned int i = 0; i < vertices.size(); ++i ) {
            vertices[ i ].u /= texture->getWidth();
            vertices[ i ].v /= texture->getHeight();
        }
    };
};
//---------------------------------------------------------------------------
#pragma package(smart_init)

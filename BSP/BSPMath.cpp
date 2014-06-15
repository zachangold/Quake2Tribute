//---------------------------------------------------------------------------

#pragma hdrstop

#include "BSPMath.h"


/**
 * Computes the texture coordinate of a given point (vtx) on the plane specified by texInfo
 */
void getTexCoord( D3D::Vertex *vtx, BSP::TexInfo *texInfo, WALImage *texture ) {

    // Translate to Quake 2 coordinates with this:
    // point( x, y, z ) = vtx( y, z, -x )

    // make a point that is the quake coordinate version of vtx
    Point3f point;
    point.x = -vtx->z;
    point.y =  vtx->x;
    point.z =  vtx->y;

    // calculate the texture coordinates
    vtx->u = point.x * texInfo->u_axis.x + point.y * texInfo->u_axis.y + point.z * texInfo->u_axis.z + texInfo->u_offset;
    vtx->v = point.x * texInfo->v_axis.x + point.y * texInfo->v_axis.y + point.z * texInfo->v_axis.z + texInfo->v_offset;

    // Set the lightmap texture coordinates to the same values.
    vtx->lmu = vtx->u;
    vtx->lmv = vtx->v;
};

/**
 * Subtracts the values in parameter a minus the values in parameter b
 * (sets a to a - b)
 */
void vertexSubtract( D3D::Vertex *a, D3D::Vertex *b ) {
    a->x -= b->x;
    a->y -= b->y;
    a->z -= b->z;
};

/**
 * Subtracts the values in parameter a minus the values in parameter b
 * (sets a to a - b)
 */
void vertexSubtract( Point3f *a, Point3f *b ) {
    a->x -= b->x;
    a->y -= b->y;
    a->z -= b->z;
};

/**
 * Copies the vertex location at point to the bspVertex vtx
 */
void vertexCopy( D3D::Vertex *vtx, Point3f *point ) {
    vtx->x = point->x;
    vtx->y = point->y;
    vtx->z = point->z;
};

/**
 * Copies the normal from b to a
 */
void normalCopy( D3D::Vertex *a, D3D::Vertex *b ) {
    a->nx = b->nx;
    a->ny = b->ny;
    a->nz = b->nz;
};

/**
 * sets the length of the normal in vtx to 1
 */
void normalize( D3D::Vertex *vtx ) {
    // Find the normal's length
    float length = sqrt( vtx->nx * vtx->nx + vtx->ny * vtx->ny + vtx->nz * vtx->nz );

    // Divide each of the components of the normal by that length.
    vtx->nx /= length;
    vtx->ny /= length;
    vtx->nz /= length;
};

/**
 * sets the length of the normal in vtx to 1
 */
void normalize( Point3f *vtx ) {
    // Find the normal's length
    float length = sqrt( vtx->x * vtx->x + vtx->y * vtx->y + vtx->z * vtx->z );

    // Divide each of the components of the normal by that length.
    vtx->x /= length;
    vtx->y /= length;
    vtx->z /= length;
};

/**
 * Uses the winding of a polygon to compute the normal for lighting.
 */
void getNormal( D3D::Vertex *triangle ) {

    // The two vertices of the triangle necessary for the cross-product
    D3D::Vertex v1 = triangle[ 1 ], v2 = triangle[ 2 ];

    // translate the triangle to the origin
    vertexSubtract( &v1, &triangle[ 0 ] );
    vertexSubtract( &v2, &triangle[ 0 ] );


    // Calculate the normal of the first vertex by its winding
    //a × b = (a2b3 - a3b2) i + (a3b1 - a1b3) j + (a1b2 - a2b1) k = (a2b3 - a3b2, a3b1 - a1b3, a1b2 - a2b1)
    triangle[ 0 ].nx = v1.y * v2.z - v1.z * v2.y;    triangle[ 0 ].ny = v1.z * v2.x - v1.x * v2.z;
    triangle[ 0 ].nz = v1.x * v2.y - v1.y * v2.x;
    // set the length of the normal to 1    normalize( &triangle[ 0 ] );    // Set the normals of the other two vertices equal to the first one
    normalCopy( &triangle[ 1 ], &triangle[ 0 ] );
    normalCopy( &triangle[ 2 ], &triangle[ 0 ] );

};


/**
 * Simple routine that constructs a point3f and returns it
 */
Point3f getPoint( float _x, float _y, float _z ) {
    Point3f rtn;
    rtn.x = _x;
    rtn.y = _y;
    rtn.z = _z;

    return rtn;
};

//---------------------------------------------------------------------------
#pragma package(smart_init)

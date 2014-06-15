//---------------------------------------------------------------------------

#ifndef BSPMathH
#define BSPMathH

#include <math.h>

#include "BSPCommon.h"
#include "WALImage.h"

// Fills in the texture coordinate of a bspVertex
void getTexCoord( D3D::Vertex *vtx, BSP::TexInfo *texInfo, WALImage *texture );

// Copies the vertex location from point to vtx
void vertexCopy( D3D::Vertex *vtx, Point3f *point );

// Computes the normal of an array of three vertices pointed to by triangle.
void getNormal( D3D::Vertex *triangle );

// Function that returns a Point3f at (_x, _y, _z)
Point3f getPoint( float _x, float _y, float _z );


//---------------------------------------------------------------------------
#endif

#include <math>
#include "frustum.h"

/**
 * Sets the length of the line segment from (0, 0, 0) to (a, b, c) to 1.0
 */
Plane Frustum::extractPlane( float a, float b, float c, float d )
{
	float t = sqrt( a * a + b * b + c * c );

	Plane p;
	p.a = a / t;
	p.b = b / t;
	p.c = c / t;
	p.d = d / t;

	return p;
}

bool Frustum::leafInFrustum( BSP::Leaf *leaf )
{
    // Get the middle point of the bsp leaf's bounding box
    Point3f middle = getPoint( ( leaf->bbox_min.y + leaf->bbox_max.y ) / 2.0 * BSP::MAP_SCALE,
                               ( leaf->bbox_min.z + leaf->bbox_max.z ) / 2.0 * BSP::MAP_SCALE,
                               -( leaf->bbox_min.x + leaf->bbox_max.x ) / 2.0 * BSP::MAP_SCALE );


    // Find the distance from the middle point to the corner of the leaf's bounding box.
    float distToCorner = sqrt( ( leaf->bbox_max.x - leaf->bbox_min.x ) * ( leaf->bbox_max.x - leaf->bbox_min.x ) +
                               ( leaf->bbox_max.y - leaf->bbox_min.y ) * ( leaf->bbox_max.y - leaf->bbox_min.y ) +
                               ( leaf->bbox_max.z - leaf->bbox_min.z ) * ( leaf->bbox_max.z - leaf->bbox_min.z ) ) / 2.0 * BSP::MAP_SCALE;

    // If the leaf counts as being outside of one of the planes defining
    // the viewing frustum, the leaf is not visible. Otherwise, the leaf
    // is visible.
	for ( int p = 0; p < 6; p++ )
    {
		if ( ( m_planes[ p ].a * middle.x +
               m_planes[ p ].b * middle.y +
               m_planes[ p ].c * middle.z +
               m_planes[ p ].d + distToCorner) <= 0 )
        {
			return false;
        }
    }
	return true;
}


void Frustum::updateFrustum( LPDIRECT3DDEVICE9 device )
{

    // Direct3D's view and projection matrices
    D3DXMATRIX view, proj;
	D3DXMATRIX mvp;

    // Get the view and projection matrices
    device->GetTransform( D3DTS_VIEW, &view );
    device->GetTransform( D3DTS_PROJECTION, &proj );

    // multiply the view and projection matrices together to extract the planes
    // of the viewing frustum.
    mvp = view * proj;


	/* Extract the RIGHT plane */
	m_planes[ PLANE_RIGHT ] = extractPlane(mvp._14 - mvp._11,
	                                       mvp._24 - mvp._21,
	                                       mvp._34 - mvp._31,
	                                       mvp._44 - mvp._41);

	/* Extract the LEFT plane */
	m_planes[ PLANE_LEFT ] = extractPlane(mvp._14 + mvp._11,
	                                      mvp._24 + mvp._21,
	                                      mvp._34 + mvp._31,
	                                      mvp._44 + mvp._41);

	/* Extract the TOP plane */
	m_planes[ PLANE_TOP ] = extractPlane(mvp._14 - mvp._12,
	                                     mvp._24 - mvp._22,
	                                     mvp._34 - mvp._32,
	                                     mvp._44 - mvp._42);

	/* Extract the BOTTOM plane */
	m_planes[ PLANE_BOTTOM ] = extractPlane(mvp._14 + mvp._12,
	                                        mvp._24 + mvp._22,
	                                        mvp._34 + mvp._32,
	                                        mvp._44 + mvp._42);

	/* Extract the FAR plane */
	m_planes[ PLANE_NEAR ] = extractPlane(mvp._14 - mvp._13,
	                                      mvp._24 - mvp._23,
	                                      mvp._34 - mvp._33,
	                                      mvp._44 - mvp._43);

	/* Extract the NEAR plane */
	m_planes[ PLANE_FAR ] = extractPlane(mvp._14 + mvp._13,
	                                     mvp._24 + mvp._23,
	                                     mvp._34 + mvp._33,
	                                     mvp._44 + mvp._43);

}
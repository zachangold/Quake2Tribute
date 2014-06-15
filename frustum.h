/**
 * Frustum.cpp, Frustum.h:
 * THIS MODULE NOT ENTIRELY MADE BY ZACH ANGOLD
 * Some of this code was found in the book "Beginning OpenGL Game Programming,
 * Second Edition" by Luke Benstead. However, the original code was adapted
 * to fit in with Direct3D and have functionality that fit the BSP Map
 * requirements for frustum culling.
 */

#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <DirectX/d3d9.h>
#include <DirectX/d3dx9.h>
#include "BSPCommon.h"
#include "BSPMath.h"

/**
 * The parts of the plane equation (ax + by + cz + d = 0)
 */
struct Plane {
	float a, b, c, d;
};


/**
 * The Frustum object, associated with a camera object, provides an easy way to
 * determine whether or not an object is being looked at. If not looked at, an
 * object does not have to be drawn, improving performance.
 */
class Frustum {
    public:

        /**
         * updateFrustum() takes the Direct3D device and gets exactly how the
         * world is being looked at. Later, this data is used to determine which
         * BSP leaves are being looked at, and which ones to draw or not to draw.
         */
        void updateFrustum( LPDIRECT3DDEVICE9 device );

        /**
         * leafInFrustum() tests to see if parameter leaf is visible with the
         * viewing frustum. If it is, this method returns true. If it is not,
         * then this method returns false.
         */
        bool leafInFrustum( BSP::Leaf *leaf );

    private:

        /**
         * The planes defining the viewing frustum
         */
        Plane m_planes[ 6 ];

        /**
         * enumeration showing which plane is which
         */
        enum {
            PLANE_LEFT = 0,
            PLANE_RIGHT,
            PLANE_TOP,
            PLANE_BOTTOM,
            PLANE_FAR,
            PLANE_NEAR
        };

        /**
         * Sets the length of the line segment from (0, 0, 0) to (a, b, c) to 1.0
         */
        Plane extractPlane( float a, float b, float c, float d );
};

#endif
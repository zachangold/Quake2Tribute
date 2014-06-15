//---------------------------------------------------------------------------

#ifndef CameraH
#define CameraH

#include "BSPCommon.h"
#include "BSPMath.h"
#include "Frustum.h"

/**
 * The point structure keeps track of the camera's position and rotation.
 */
typedef struct {
    // The coordinates of the camera in 3D space
    float x, y, z;

    // The camera's rotation on the x, y, and z axis
    float rx, ry, rz;

    /**
     * Simple method that resets all of the fields of the camera
     */
    void reset() {
        x = 0; y = 0; z = 0;
        rx = 0; ry = 0; rz = 0;
    };
} Point;


/**
 * The Camera class is designed to allow the user to move freely in the game
 * world, moving the world around the user properly, without adding large amounts
 * of code to the main rendering section of the Engine class
 */
class Camera {
    private:

        // The Frustum of the camera, used for Frustum culling when drawing
        // The BSP Map
        Frustum *frustum;

    public:
        // The Camera's position and orientation
        Point *pos;

        /**
         * Constructor that initialises the entire Camera object. No other
         * initialisation methods need to be called.
         */
        Camera();

        /**
         * Destructor that deletes all allocated memory.
         */
        ~Camera();

        /**
         * Allows the camera to set up the viewing transformation matrix. This
         * is just how the objects are translated around the camera. Effectively,
         * the camera isn't moving around in the map, rather the map is moving
         * and rotating around a camera that stays still.
         */
        void setupTransform( LPDIRECT3DDEVICE9 device );

        /**
         * Moves the camera based on the states of the keys on the keyboard.
         * Holding down:
         *  W - moves forward
         *  A - moves to the left
         *  S - moves backwards
         *  D - moves to the right
         *  C - moves down
         *  Space - moves up
         *  Shift - causes all movements to double in speed
         */
        void move( bool *keys );

        /**
         * Updates the camera based on how the mouse moved.
         */
        void update( float mouseX, float mouseY );

        /**
         * Tests to see if a BSP Leaf is within the viewing frustum.
         * Returns true if the leaf is visible,
         * Returns false if not visible.
         */
        bool leafInFrustum( BSP::Leaf *leaf ) {
            return frustum->leafInFrustum( leaf );
        };
};

//---------------------------------------------------------------------------
#endif
 


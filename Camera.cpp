//---------------------------------------------------------------------------

#pragma hdrstop

#include "Camera.h"

// constant floating point number used to convert radians into degrees
const float PI_OVER_180 = 3.141592f / 180.0f;

/**
 * Constructor that initialises the entire Camera object. No other
 * initialisation methods need to be called.
 */
Camera::Camera () {
    // Create the point object that stores the camera's position
    pos = new Point();
    pos->reset();

    // Create the viewing frustum object that helps with determining what parts
    // of the BSP map are visible and what parts are not visible.
    frustum = new Frustum();

};

/**
 * Destructor that deletes all allocated memory.
 */
Camera::~Camera() {
    if ( pos != NULL ) {
        delete pos;
    }

    if ( frustum != NULL ) {
        delete frustum;
    }


};

// constant for how fast the camera moves
const float MOVE_SPEED = 3.5f * BSP::MAP_SCALE;

/**
 * moves the camera:
 *  w and s for moving forward and backward
 *  a and d for moving left to right
 *  space and c for moving up and down
 *  shift to double movement speed
 */
void Camera::move( bool *keys ) {
    float speedModifier = keys[ 16 ] ? 2.0 : 1.0;


    if ( keys[ 'W' ] ) {
        // If the 'w' key is being pressed, move forward
        pos->x -= sin( pos->rx * PI_OVER_180 ) * MOVE_SPEED * speedModifier;
        pos->z -= cos( pos->rx * PI_OVER_180 ) * MOVE_SPEED * speedModifier;
    }
    if ( keys[ 'S' ] ) {
        // If the 's' key is being pressed, move backward
        pos->x += sin( pos->rx * PI_OVER_180 ) * MOVE_SPEED * speedModifier;
        pos->z += cos( pos->rx * PI_OVER_180 ) * MOVE_SPEED * speedModifier;
    }
    if ( keys[ 'A' ] ) {
        // If the 'a' key is being pressed, move to the left
        pos->x += cos( pos->rx * PI_OVER_180 ) * MOVE_SPEED * speedModifier;
        pos->z -= sin( pos->rx * PI_OVER_180 ) * MOVE_SPEED * speedModifier;
    }
    if ( keys[ 'D' ] ) {
        // If the 'd' key is being pressed, move to the right
        pos->x -= cos( pos->rx * PI_OVER_180 ) * MOVE_SPEED * speedModifier;
        pos->z += sin( pos->rx * PI_OVER_180 ) * MOVE_SPEED * speedModifier;
    }
    if ( keys[ 'C' ] ) {
        // if the 'c' key is being pressed, move downwards
        pos->y += MOVE_SPEED * speedModifier;
    }
    if ( keys[ ' ' ] ) {
        // if the space bar is being pressed, move upwards
        pos->y -= MOVE_SPEED * speedModifier;
    }
};

/**
 * Updates the camera based on how the mouse moved.
 */
void Camera::update( float mouseX, float mouseY ) {

    // Rotate the camera
    pos->rx = mouseX;
    pos->ry = mouseY;

    // Make sure the rotation doesn't exceed 90 degrees up or down, so the camera
    // doesn't tilt
    if ( pos->ry > 90.0 ) {
        pos->ry = 90.0;
    } else if ( pos->ry < -90.0 ) {
        pos->ry = -90.0;
    }

};

/**
 * Allows the camera to set up the viewing transformation matrix. This
 * is just how the objects are translated around the camera. Effectively,
 * the camera isn't moving around in the map, rather the map is moving
 * and rotating around a camera that stays still.
 */
void Camera::setupTransform( LPDIRECT3DDEVICE9 device ) {
    // The transformation matrices
    D3DXMATRIX rotX, rotY, trans, eyeTrans;

    // Rotate the world around the camera
    D3DXMatrixRotationX( &rotX, -D3DXToRadian( pos->ry ) );
    D3DXMatrixRotationY( &rotY, -D3DXToRadian( pos->rx ) );

    // move the world around the camera
    D3DXMatrixTranslation( &trans, pos->x, pos->y, pos->z );

    // send the viewing transformation to Direct3D
    device->SetTransform( D3DTS_VIEW, &( trans * rotY * rotX ) );

    // update the frustum object.
    frustum->updateFrustum( device );
};

//---------------------------------------------------------------------------
#pragma package(smart_init)

 

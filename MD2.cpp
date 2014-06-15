//---------------------------------------------------------------------------

#pragma hdrstop

#include "MD2.h"


/**
 * MD2.h, MD2.cpp:
 *  These modules were not entirely written by Zach Angold. The only modifications
 * made were specializing the code to fit in with Direct3D, rather than OpenGL.
 *  The original code was found in the book "Beginning OpenGL Game Programming,
 * Second Edition" by Luke Benstead.
 */


// precalculated normal vectors
Vector3 MD2Model::normals[ 162 ] = {
    #include    "anorms.h"
};


MD2Model::MD2Model() {
    vertexBuffer = NULL;
    skin = NULL;
    frameNum = 0;
    normalVertexBuffer = NULL;

};

MD2Model::~MD2Model() {
    deleteBuffers();
    if ( skin ) {
        delete skin;
    }
};



bool MD2Model::load( string fileName, LPDIRECT3DDEVICE9 device ) {
    FILE *fh = 0;

    if ( ( fh = fopen( ( fileName + string( "tris.md2" ) ).c_str(), "rb" ) ) == NULL ) {
        return false;
    }

    // Read in the header
    fseek( fh, 0, 0 );
    fread( &header, sizeof( MD2Header ), 1, fh );

    // Resize the MD2Frame array
    frames.resize( header.numFrames );

    for ( int f = 0; f < header.numFrames; ++f ) {
        frames[f].MD2verts.resize( header.numVertices );
    }

    triangles.resize( header.numTriangles );
    fseek( fh, header.triangleOffset, 0 );
    fread( &triangles[ 0 ], header.numTriangles * sizeof( Triangle ), 1, fh );

    texCoords.resize( header.numTextureCoords );
    fseek( fh, header.texCoordOffset, 0 );
    fread( &texCoords[ 0 ], header.numTextureCoords * sizeof( TexCoord ), 1, fh );

    vector< Skin > skinNames;
    skinNames.resize( header.numSkins );
    fseek( fh, header.skinOffset, 0 );
    fread( &skinNames[ 0 ], header.numSkins * sizeof( Skin ), 1, fh );


    skins.resize( 1 );
    skins[0].loadImage( ( fileName + string( "skin.pcx" ) ).c_str(), device );

    // Read in the frames
    fseek( fh, header.frameOffset, 0 );

    for (int i = 0; i < header.numFrames; ++i) {
        MD2Frame* f = &frames[ i ];

        fread( f->scale, sizeof( float ) * 3, 1, fh );
        fread( f->translate, sizeof( float ) * 3, 1, fh );
        fread( f->name, sizeof( char ) * 16, 1, fh );
        fread( &f->MD2verts[0], sizeof( MD2Vertex ) * header.numVertices, 1, fh );
    }

    generateBuffers( device );
    reorganizeVertices();

    if ( fh ) {
        fclose( fh );
    }

    interpolation = 0.0f;
    frameNum = 0;

    startFrame = 0;
    endFrame = header.numFrames - 1;

    //loadTexture( skinName, device );

    return true;
};

void MD2Model::generateBuffers( LPDIRECT3DDEVICE9 device ) {
    device->CreateVertexBuffer(sizeof(D3DMD2Vertex) * triangles.size() * 3,
                               0,                               MD2FVF,
                               D3DPOOL_MANAGED,
                               &vertexBuffer,
                               NULL);


};

bool MD2Model::loadTexture( string fileName, LPDIRECT3DDEVICE9 device ) {

    if ( !skin ) {
        skin = new Texture();
    }

    skin->loadImage( fileName.c_str(), device );

    return true;
};

void MD2Model::unloadTexture( LPDIRECT3DDEVICE9 device ) {
    if ( skin ) {
        skin->unload();
    }
};

void MD2Model::deleteBuffers( void ) {
    vertexBuffer->Release();
};

const float SIZE_SCALE = 1.0f;

void MD2Model::update( float dt ) {

    interpolation += dt * ANIMATION_FPS;

    if ( interpolation > 1.0 ) {
        frameNum = nextFrame;
        nextFrame++;
        interpolation = 0.0f;
    }


    //nextFrame = frameNum + 1;
    if ( nextFrame > endFrame ) {
        nextFrame = startFrame;
    }

    VOID* pVoid;

    vertexBuffer->Lock(0, 0, (void **)&pVoid, 0);    // locks v_buffer, the buffer we made earlier

    D3DMD2Vertex *vCopy = (D3DMD2Vertex *) pVoid;

    for ( unsigned int i = 0; i < triangles.size(); ++i ) {
        for ( int j = 0; j < 3; ++j ) {
            float v1, v2;

            v1 = frames[frameNum].verts[i * 3 + j].y;
            v2 = frames[nextFrame].verts[i * 3 + j].y;
            vCopy[i * 3 + j].x = v1 + interpolation * (v2 - v1);



            v1 = frames[frameNum].verts[i * 3 + j].z;
            v2 = frames[nextFrame].verts[i * 3 + j].z;
            vCopy[i * 3 + j].y = v1 + interpolation * (v2 - v1);

            v1 = -frames[frameNum].verts[i * 3 + j].x;
            v2 = -frames[nextFrame].verts[i * 3 + j].x;
            vCopy[i * 3 + j].z = v1 + interpolation * (v2 - v1);



            v1 = frames[frameNum].normals[i * 3 + j].y;
            v2 = frames[nextFrame].normals[i * 3 + j].y;
            vCopy[i * 3 + j].nx = v1 + interpolation * (v2 - v1);

            v1 = frames[frameNum].normals[i * 3 + j].z;
            v2 = frames[nextFrame].normals[i * 3 + j].z;
            vCopy[i * 3 + j].ny = v1 + interpolation * (v2 - v1);

            v1 = -frames[frameNum].normals[i * 3 + j].x;
            v2 = -frames[nextFrame].normals[i * 3 + j].x;
            vCopy[i * 3 + j].nz = v1 + interpolation * (v2 - v1);

        }
        // For the normal of each triangle
        //getNormal( &vCopy[ i * 3 ] );
    }

    vertexBuffer->Unlock();
};

void MD2Model::render( LPDIRECT3DDEVICE9 device ) {

    setSkinNum( 0 );

    device->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
    device->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
    device->SetRenderState( D3DRS_LIGHTING, TRUE );

    device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

    device->SetFVF( MD2FVF );

    device->SetTexture( 0, skins[ skinNum ].getTexture() );

    device->SetStreamSource( 0, vertexBuffer, 0, sizeof( D3DMD2Vertex ) );

    device->DrawPrimitive( D3DPT_TRIANGLELIST, 0, header.numTriangles );

    device->SetRenderState( D3DRS_NORMALIZENORMALS, FALSE );
    device->SetRenderState( D3DRS_LIGHTING, FALSE );
};

void MD2Model::reorganizeVertices() {
    vector<D3DMD2Vertex> tempVertices;

    tempVertices.resize( triangles.size() * 3 );

    //Then go through the triangles
    for (unsigned int i = 0; i < triangles.size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            //Push back the 3 vertices for this triangle
            tempVertices[i * 3 + j].x = (float(frames[0].MD2verts[triangles[i].vertexIndex[j]].v[0]) * frames[0].scale[0] + frames[0].translate[0]);
            tempVertices[i * 3 + j].y = (float(frames[0].MD2verts[triangles[i].vertexIndex[j]].v[1]) * frames[0].scale[1] + frames[0].translate[1]);
            tempVertices[i * 3 + j].z = (float(frames[0].MD2verts[triangles[i].vertexIndex[j]].v[2]) * frames[0].scale[2] + frames[0].translate[2]);

            tempVertices[i * 3 + j].nx = normals[ frames[0].MD2verts[triangles[i].vertexIndex[j]].lightNormalIndex ].x;
            tempVertices[i * 3 + j].ny = normals[ frames[0].MD2verts[triangles[i].vertexIndex[j]].lightNormalIndex ].y;
            tempVertices[i * 3 + j].nz = normals[ frames[0].MD2verts[triangles[i].vertexIndex[j]].lightNormalIndex ].z;

            tempVertices[i * 3 + j].u = (float(texCoords[triangles[i].texCoordIndex[j]].u)) / (float(header.skinWidth));
            tempVertices[i * 3 + j].v = 1.0 - (float(texCoords[triangles[i].texCoordIndex[j]].v)) / (float(header.skinHeight));
        }
    }

    for (int f = 0; f < header.numFrames; ++f) {
        frames[f].verts.resize( triangles.size() * 3 );
        frames[f].normals.resize( triangles.size() * 3 );
        for (unsigned int i = 0; i < triangles.size(); ++i) {
            for (int j = 0; j < 3; ++j) {
                //Push back the 3 vertices for this triangle
                frames[f].verts[i * 3 + j].x = ( float( frames[f].MD2verts[ triangles[i].vertexIndex[j] ].v[0] ) * frames[f].scale[0] + frames[f].translate[0] );
                frames[f].verts[i * 3 + j].y = ( float( frames[f].MD2verts[ triangles[i].vertexIndex[j] ].v[1] ) * frames[f].scale[1] + frames[f].translate[1] );
                frames[f].verts[i * 3 + j].z = ( float( frames[f].MD2verts[ triangles[i].vertexIndex[j] ].v[2] ) * frames[f].scale[2] + frames[f].translate[2] );

                frames[f].normals[i * 3 + j].x = normals[ frames[f].MD2verts[triangles[i].vertexIndex[j]].lightNormalIndex ].x;
                frames[f].normals[i * 3 + j].y = normals[ frames[f].MD2verts[triangles[i].vertexIndex[j]].lightNormalIndex ].y;
                frames[f].normals[i * 3 + j].z = normals[ frames[f].MD2verts[triangles[i].vertexIndex[j]].lightNormalIndex ].z;
            }
        }
    }

	//Copy the new texture coordinate array over the original
    //m_texCoords = tempTexCoords;
    // Copy in the new vertex information
    VOID* pVoid;

    vertexBuffer->Lock(0, 0, (void **)&pVoid, 0);    // locks v_buffer, the buffer we made earlier

    memcpy( pVoid, &tempVertices[ 0 ], tempVertices.size() * sizeof( D3DMD2Vertex ) );

    vertexBuffer->Unlock();
};

void MD2Model::setSkinNum( int skinN ) {
    skinNum = skinN;
};


//---------------------------------------------------------------------------
#pragma package(smart_init)


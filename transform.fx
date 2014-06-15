// The plain texture of a face, loaded in from a WAL Image
texture modelTexture;

// The lightmap texture loaded in from the lightMap lump of the BSP Map
texture lightMap;

texture normalMap;

float4x4 worldViewProj : WORLDVIEWPROJ;
float4x4 world;
float4x4 view;
float4x4 proj;

float camPosX;
float camPosY;
float camPosZ;

float vsTest;

// Integer to control whether or not to use lightmaps.
// This may be used to temporarily turn off light maps to render parts of the
// map that do not use lightmaps (for example, the water in the map)
int useLightMap = 0;


#define PI_OVER_180 3.141592 / 180.0

float time = 0;

// Texture sampler: This allows the Pixel shader to get the colour of a point on the
// plain texture.
sampler2D modelTextureSampler = sampler_state
{
    // Tell DirectX that the texture should be the plain texture in the map
    Texture = ( modelTexture );

    // Tell DirectX to use linear filtering to get the texture colour (this is the
    // maximum quality the target machine (the school computers) can use).
    MIPFILTER = ANISOTROPIC;
    MAGFILTER = ANISOTROPIC;
    MAXANISOTROPY = 16;
};


sampler2D screenSampler = sampler_state
{
    Texture = ( modelTexture );
    MIPFILTER = LINEAR;
    MAGFILTER = LINEAR;
    AddressU = wrap;
    AddressV = wrap;
};

sampler2D normalMapSampler = sampler_state
{
    Texture = ( normalMap );
    MIPFILTER = ANISOTROPIC;
    MAGFILTER = ANISOTROPIC;
    AddressU = wrap;
    AddressV = wrap;
};

// Texture sampler: This allows the Pixel shader to get the colour of a point on the
// lightmap texture
sampler2D lightMapSampler = sampler_state
{
    // Tell DirectX that the texture it should use is the lightmap texture
    Texture = ( lightMap );

    MIPFILTER = ANISOTROPIC;
    MAGFILTER = ANISOTROPIC;
    MINFILTER = ANISOTROPIC;
    MAXANISOTROPY = 16;

    // Tell DirectX to NOT repeat the texture multiple times.
    AddressU = clamp;
    AddressV = clamp;
};
                   
// Parameters for pixel shader. The pixel shader receives this structure
// as its parameter.
struct psIn {

    // The texture coordinate of the plain texture of the pixel
    float2 baseTexCoord : TEXCOORD0;

    // The texture coordinate of the lightmap of the pixel
    float2 lightMapCoord : TEXCOORD1;

    float3 normal : TEXCOORD2;
    float3 pos3D : TEXCOORD3;
};

float4 getSample( float2 baseCoord, float uOffset, float vOffset ) {
    baseCoord[ 0 ] += uOffset;
    baseCoord[ 1 ] += vOffset;

    if ( baseCoord[ 0 ] > 1.0 ) {
        baseCoord[ 0 ] = 1.0;
    } else if ( baseCoord[ 0 ] < 0.0 ) {
        baseCoord[ 0 ] = 0.0;
    }

    if ( baseCoord[ 1 ] > 1.0 ) {
        baseCoord[ 1 ] = 1.0;
    } else if ( baseCoord[ 1 ] < 0.0 ) {
        baseCoord[ 1 ] = 0.0;
    }

    return (float4) 0;//tex2D( screenSampler, baseCoord );
};


//////////////////////////////////////////////////////////////
//Pixel Shader
//////////////////////////////////////////////////////////////

/**
 * This is the main procedure in the pixel shader. It takes in
 * the texture coordinates of the pixel, combines the lightmap
 * and the base texture, and returns the final colour of the pixel.
 */
float4 ps( in psIn In ) : COLOR {
    float4 pos3D = ( float4 ) 0;
    pos3D.x = In.pos3D.x;
    pos3D.y = In.pos3D.y;
    pos3D.z = In.pos3D.z;

    /*
    float4 normal = tex2D( normalMapSampler, In.baseTexCoord );
    //return normal;
    normal.x *= In.normal.x;
    normal.y *= In.normal.y;
    normal.z *= In.normal.z;
    */

    //normal *= 10.0;
    //normal = normalize( normal );

    float range = 3.3;
    float vtxDist = pow( In.pos3D.x * In.pos3D.x + In.pos3D.y * In.pos3D.y + In.pos3D.z * In.pos3D.z, 0.5 );

    float4 lightDir = -1.0 * normalize( pos3D );

    float3 cameraDir = -normalize( In.pos3D );

    float3 Half = normalize( lightDir + normalize( cameraDir ) );
    float specular = pow( saturate( dot( In.normal, Half ) ), sin( vsTest / 10.0 ) * 45 + 50 ) * ( 1.0 - vtxDist / range );

    float color = ( dot( In.normal, lightDir ) * ( 1.0 - vtxDist / ( range ) ) );// + specular;// * 0.4 );
    //return ( float4 ) color;


    // If the shader is supposed to use the lightmaps,
    if ( useLightMap == 1 ) {
        // Use both the lightmap and the base texture, brightening the result
        // by multiplying it by 2.

        return tex2D( lightMapSampler, In.lightMapCoord ) * tex2D( modelTextureSampler, In.baseTexCoord ) * 2.0;// * color;// + specular;
    } else {
        // Or, if the shader is not supposed to use lightmaps, then
        // just use the base texture.
        return tex2D( modelTextureSampler, In.baseTexCoord );// * color;// + specular;
    }
};

struct vsIn {
    float4 pos : POSITION;
    float2 baseTexCoord : TEXCOORD0;
    float2 lightMapCoord : TEXCOORD1;
    float4 normal : NORMAL;
};
struct vsOut {
    float4 pos : POSITION;
    float2 baseTexCoord : TEXCOORD0;
    float2 lightMapCoord : TEXCOORD1;

    //float4 color : COLOR0;

    float3 normal : TEXCOORD2;
    float3 pos3D : TEXCOORD3;
};

vsOut vs( in vsIn In ) {
    vsOut Out;


    Out.pos = mul( In.pos, world );
    Out.baseTexCoord = In.baseTexCoord;
    Out.lightMapCoord = In.lightMapCoord;



    /*
    float fogDist = 0.15f + sin( 10.0 * vsTest * ( 3.14159265 / 180.0 ) ) * 0.05;
    float vtxDist = pow( Out.pos.x * Out.pos.x + Out.pos.y * Out.pos.y + Out.pos.z * Out.pos.z, 0.5 );

    if ( fogDist < vtxDist ) {
        Out.color = ( float4 ) 0;
    } else {
        Out.color = 1.0 - ( vtxDist / fogDist );// * 0.5;
    }
    */

    Out.normal = ( float3 ) In.normal;

    float3 pos3D;
    pos3D.x = Out.pos.x - camPosX;
    pos3D.y = Out.pos.y - camPosY;
    pos3D.z = Out.pos.z - camPosZ;

    Out.pos3D = pos3D;

    In.pos = mul( In.pos, mul( world, view ) );

    //Out.color = ( float4 ) 0;
    float Time = vsTest / 3.5;// * 10.0;
    //float4 animationPos = float4((cos(Time+In.pos.y)*.5)+In.pos.x ,In.pos.y,(sin(Time-In.pos.y) * .5)+In.pos.z,In.pos.w);
    //float4 animationPos = float4( In.pos.x, In.pos.y * ( 1.0 + ( sin( Time ) ) ) * In.pos.z / 2.0, In.pos.z + ( 1.0 + ( sin( Time ) ) ) * In.pos.z / 2.0, In.pos.w );
    float4 animationPos = ( float4 ) 0;

    In.pos += animationPos;

    Out.pos = mul( In.pos, proj );//mul( Out.pos, mul( view, proj ) );


    return Out;
};


struct bbIn {
    float2 tex : TEXCOORD0;
};

float4 bbPS( in bbIn In ) : COLOR {
    //In.tex.x += time;

    float distX = In.tex.x - 0.5;
    float distY = In.tex.y - 0.5;

    float dist = pow( distX * distX + distY * distY, 0.5 );
    float4 Color = tex2D( screenSampler, In.tex );

    float4 sampleCol = ( float4 ) 0;

    // CIRCLE-BASED SAMPLING
    for ( float f = 0.0f; f < 360; f += 55.0 ) {
        sampleCol += getSample( In.tex, sin( f / PI_OVER_180 ) / 100.0, cos( f / PI_OVER_180 ) / 100.0 );
    }

    sampleCol /= 6.0;

    return sampleCol;
    //return tex2D( screenSampler, In.tex );
};



//////////////////////////////////////////////////////////////
//Technique
//////////////////////////////////////////////////////////////
/**
 * Pixel, Vertex, and Geometry shaders operate by using a technique
 * in the HLSL program. The technique is a set of procedures that
 * modify the colour of a pixel. The technique used in the BSP map
 * shader is called "MapShader".
 */
technique MapShader
{
    /**
     * Techniques in HLSL consist of sets of procedures called "passes".
     * Passes define exactly which procedures are used with the pixel shader.
     * A technique can have as many passes as it needs (or up to a preset
     * number of passes), but for this application, only one pass is needed,
     * so only one pass is used.
     */
    pass p0
    {
        vertexshader = compile vs_3_0 vs();
        pixelshader = compile ps_3_0 ps();
    }
}

technique BBShader
{
    pass p0
    {
        pixelshader = compile ps_3_0 bbPS();
    }
}


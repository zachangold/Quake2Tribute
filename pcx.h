/**
 * pcx.cpp and pcx.h,
 * NOT CREATED BY ZACH ANGOLD
 * These two files take care of loading in a .pcx (Paintbrush Extreme) image
 * file. The LoadFilePCX function takes care of loading the file, storing the
 * image data in parameter pixels, recording the width and height to the addresses
 * width and height. The rest of the image loading is handled in Texture.h and
 * Texture.cpp.
 */

//
//	pcx.h - header file / freeware
//
//	David Henry - tfc_duke@club-internet.fr
//


#ifndef		__PCX_H_
#define		__PCX_H_



//#pragma warning( disable : 4103 ) // used #pragma pack to change alignment



// --------------------------------------------
// PCXHEADER - pcx header structure.
// --------------------------------------------

#pragma pack( push, 1 )
//#pragma pack( 1 )

typedef struct tagPCXHEADER
{
	unsigned char	manufacturer;		// fabriquant
	unsigned char	version;			// version
	unsigned char	encoding;			// encodage
	unsigned char	bitsPerPixel;		// nombre de bits par pixel

	unsigned short	x, y;
	unsigned short	width, height;		// dimensions
	unsigned short	horzRes, vertRes;

	unsigned char	palette[ 48 ];		// palette couleur
	unsigned char	reserved;			// reservé
	unsigned char	numColorPlanes;		// nombre de plans

	unsigned short	bytesPerScanLine;	// octets par ligne
	unsigned short	paletteType;
	unsigned short	horzSize, vertSize;

	unsigned char	padding[ 54 ];

} PCXHEADER, *PPCXHEADER;

//#pragma pack( 4 )
#pragma pack( pop )



#ifdef WIN32
#include	<windows.h>
#else

// --------------------------------------------
// RGBTRIPLE - pixel 24 bits.
// --------------------------------------------

typedef struct			// rgbt
{
	unsigned char	rgbRed;			// rouge
	unsigned char	rgbGreen;		// vert
	unsigned char	rgbBlue;		// bleu

} RGBTriple, *PRGBTriple;

#endif	// WIN32


// prototype
int LoadFilePCX( const char *filename, unsigned char **pixels, int *width, int *height, bool flipvert );



#endif // __PCX_H_



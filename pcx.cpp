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
//	pcx.cpp - source file / freeware
//
//	David Henry - tfc_duke@club-internet.fr
//


#include	<fstream>
#include	"pcx.h"



// --------------------------------------------------
// LoadFilePCX() -charge une image Zsoft PCX [.pcx]
//
// param�tres :
//    - filename [in]  : nom de fichier
//    - pixels	 [out] : donn�es image rgba 32 bits
//    - width    [out] : largeur en pixels
//    - height   [out] : hauteur en pixels
//    - flipvert [in]  : flag retournement vertical
//
// valeur de retour :
//    - -1 : pas de donn�es pixels
//    -  0 : �chec
//    -  1 : succ�s
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// formats accept�s:
//     # RLE 8 bits / version 5
// --------------------------------------------------

int LoadFilePCX( const char *filename, unsigned char **pixels, int *width, int *height, bool flipvert )
{
	std::ifstream		file;			// fichier
	PCXHEADER			*header;		// header PCX
	RGBTriple			*palette;		// palette
	unsigned char		*data;			// donn�es images RLE
	unsigned char		*ptr;			// pointeur donn�es pixels
	unsigned char		c;				// variable temporaire
	char				*buffer;		// buffer stockant l'int�gralit� du fichier
	int					idx = 0;		// variable temporaire
	int					numRepeat;		// variable temporaire
	int					i, j;			// variable temporaire



	/////////////////////////////////////////////////////
	// lit l'int�gralit� du fichier dans le buffer

	file.open( filename, std::ios::in | std::ios::binary );

	if( file.fail() )
		return 0;

	file.seekg( 0, std::ios::end );
	long flen = file.tellg();
	file.seekg( 0, std::ios::beg );

	buffer = new char[ flen + 1 ];
	file.read( buffer, flen );
	char *pBuff = buffer;

	file.close();

	/////////////////////////////////////////////////////

	// on lit le header
	header = (PCXHEADER *)pBuff;

	// v�rification de l'authenticit� du PCX
	if( (header->manufacturer	!= 10)	||
		(header->version		!= 5)	||
		(header->encoding		!= 1)	||
		(header->bitsPerPixel	!= 8) )
	{
		delete [] buffer;
		return 0;
	}


	header->width	= header->width	 - header->x + 1;
	header->height	= header->height - header->y + 1;


	if( width )
		*width = header->width;

	if( height )
		*height = header->height;

	if( !pixels )
	{
		delete [] buffer;
		return (-1);
	}


	// allocatation m�moire pour les donn�es image
	data = new unsigned char[ header->width * header->height * 3 ];
	pBuff = (char *)&buffer[ 128 ];

	// d�code l'image compress�e (RLE)
	while( idx < (header->width * header->height) )
	{
		if( (c = *(pBuff++)) > 0xbf )
		{
			numRepeat = 0x3f & c;
			c = *(pBuff++);

			for( i = 0; i < numRepeat; i++ )
				data[ idx++ ] = c;
		}
		else
			data[ idx++ ] = c;
	}

	// la palette se trouve au 769 dernier octet du fichier
	pBuff = &buffer[ flen - 769 ];

	// on v�rifie la palette ; le premier char doit �tre �gal � 12
	if( *(pBuff++) != 12 )
	{
		delete [] buffer;
		delete [] data;
		return 0;
	}

	// on lit la palette
	palette = (RGBTriple *)pBuff;

	// allocatation m�moire pour les donn�es pixels 32 bits
	*pixels = new unsigned char[ header->width * header->height * 4 ];
	ptr = &(*pixels)[0];

	// conversion pixel index couleur bgr en pixel rgba 32 bits
	for( j = header->height - 1; j >= 0; j-- )
	{
		if( flipvert )
			ptr = &(*pixels)[ j * header->width * 4 ];

		for( i = 0; i < header->width; i++, ptr += 4 )
		{
			int color = data[ j * header->width + i ];

			ptr[ 0 ] = ( unsigned char ) palette[ color ].rgbBlue;	// b->r
			ptr[ 1 ] = ( unsigned char ) palette[ color ].rgbGreen;	// g->g
			ptr[ 2 ] = ( unsigned char ) palette[ color ].rgbRed; 	// r->b
			ptr[ 3 ] = ( unsigned char ) 255;					    // alpha
		}
	}


	// d�sallocation m�moire tampon
	delete [] data;
	delete [] buffer;
    
	// succ�s
	return 1;
}




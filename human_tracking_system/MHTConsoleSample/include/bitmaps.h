#ifndef _BITMAPS_H
#define _BITMAPS_H

/****************************************************************************************
 * Definitions & Header Files
 ****************************************************************************************/
#include	<time.h>
#include	<math.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"libpsm.h"

#ifdef		GLOBAL_VARI
#define		GLOBAL_VARI
#else
#define		GLOBAL_VARI			extern
#endif		/* GLOBAL_VARI */

#ifdef		CPLUSPLUS
#define		GLOBAL_FUNC			extern "C"
#else
#define		GLOBAL_FUNC			extern
#endif		/* CPLUSPLUS */

#ifdef		WIN
#include	<windows.h>
#include	<mmsystem.h>
#include	<mbstring.h>
#pragma		comment( lib, "winmm.lib" )
#define		DLLEXPORT			__declspec( dllexport )
#define		DLLIMPORT			__declspec( dllimport )
#else
#define		DLLEXPORT
#define		DLLIMPORT
#endif		/* WIN */

#define		VOID				void
#define		CNST				const
#define		DBFL				float

/****************************************************************************************
 * Macroinstructions
 ****************************************************************************************/

/****************************************************************************************
 * Type Definitions and Symbol Constant Definitions
 ****************************************************************************************/
typedef		double				DOUB ;
typedef		float				FLOA ;
typedef		signed   int		BOOL ;
typedef		signed   int		SINT32 ;
typedef		signed   char		CHAR ;
typedef		signed   short		SHRT ;
typedef		unsigned int		UINT ;
typedef		unsigned int		UINT32 ;
typedef		unsigned char		BYTE ;
typedef		unsigned short		WORD ;

#ifndef		TRUE
#define		TRUE				( BOOL )(   1 )
#endif		/* TRUE */

#ifndef		FALSE
#define		FALSE				( BOOL )(   0 )
#endif		/* FALSE */

#define		ERR100				( BOOL )( 100 )
#define		ERR101				( BOOL )( 101 )
#define		ERR102				( BOOL )( 102 )
#define		ERR103				( BOOL )( 103 )
#define		ERR104				( BOOL )( 104 )
#define		ERR105				( BOOL )( 105 )
#define		ERR106				( BOOL )( 106 )
#define		ERR107				( BOOL )( 107 )
#define		ERR108				( BOOL )( 108 )
#define		ERR109				( BOOL )( 109 )
#define		ERR110				( BOOL )( 110 )
#define		ERR111				( BOOL )( 111 )
#define		ERR112				( BOOL )( 112 )
#define		ERR113				( BOOL )( 113 )
#define		ERR114				( BOOL )( 114 )
#define		ERR115				( BOOL )( 115 )
#define		ERR116				( BOOL )( 116 )
#define		ERR117				( BOOL )( 117 )
#define		ERR118				( BOOL )( 118 )
#define		ERR119				( BOOL )( 119 )
#define		ERR120				( BOOL )( 120 )

/****************************************************************************************
 * Structure Definitions
 ****************************************************************************************/
typedef	struct {

	unsigned short		bfType;
	UINT32		bfSize;
	unsigned short		bfRsv1;
	unsigned short		bfRsv2;
	UINT32		bfBits;

} BMFH;

typedef	struct {

	UINT32		biSize;
	UINT32		biWdth;
	UINT32		biHigh;
	unsigned short		biPlan;
	unsigned short		biBitc;
	UINT32		biComp;
	UINT32		biImag;
	UINT32		biXppm;
	UINT32		biYppm;
	UINT32		biCuse;
	UINT32		biCimp;

} BMIH;

typedef struct {

	unsigned char		rgbBlu;
	unsigned char		rgbGrn;
	unsigned char		rgbRed;
	unsigned char		rgbRsv;

} RGBQ;

typedef	struct {

	BMIH			bmiHead  ;
	RGBQ			*rgbQuad ;

} BMPI ;

typedef struct {

	UINT32		Wdth;			/* Width  of the image (pixel)					*/
	UINT32		High;			/* Height of the image (pixel)					*/
	unsigned short		Bitc;			/* Number of bit  (bits/pixel)					*/
	UINT32		Maps;			/* Number of color    (/pixel)					*/
	UINT32		Xppm;			/* X resolution (pixels/meter)					*/
	UINT32		Yppm;			/* Y resolution (pixels/meter)					*/

} IMGI;

typedef struct {

	SINT32			xSize  ;
	SINT32			bSize  ;
	SINT32			ySize  ;
	SINT32			depth  ;
	WORD			Xdpi   ;
	WORD			Ydpi   ;
	BYTE			*pbImg ;

} BIMG ;

/****************************************************************************************
 * Global Variables
 ****************************************************************************************/

/****************************************************************************************
 * Constants and Declarations
 ****************************************************************************************/

/****************************************************************************************
 * Global Functions
 ****************************************************************************************/
GLOBAL_FUNC int		CHECK_BMPFILE	(char *filename, IMGI *imgInfo);
GLOBAL_FUNC int		LOAD_PSMIMG		(char *filename, IMGI *imgInfo, RGBQ *rgbq, PSBImg *bimg, PSGImg *gimg, PSMImg *cimg);
GLOBAL_FUNC int		SAVE_PSMIMG		(char *filename, RGBQ *rgbQuad, PSBImg *bimg, PSGImg *gimg, PSMImg *cimg);

#endif /* _BITMAPS_H */
/*	End of File *************************************************************************/

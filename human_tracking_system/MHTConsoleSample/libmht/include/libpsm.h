#ifndef		_LIBPSM_H
#define		_LIBPSM_H

/****************************************************************************************
 *  Definitions & Header Files
 ****************************************************************************************/
#define		PSMERR_TRUE				(     0   )			/* Process Succeeded			*/
#define		PSMERR_MEMR				(    -1   )			/* Invalid Memory Pointer		*/
#define		PSMERR_ARGU				(    -2   )			/* Invalid Argument				*/
#define		PSMERR_IMAG				(    -3   )			/* Invalid Image				*/
#define		PSMERR_PROC				(    -4   )			/* Internal Error				*/
#define		PSMERR_RECG				(    -5   )			/* Recognition Failed			*/

/****************************************************************************************
 *  Macroinstructions
 ****************************************************************************************/

/****************************************************************************************
 *  Type Definitions and Symbol Constant Definitions
 ****************************************************************************************/
#define		PSMIMG_FORMAT_Y			( 0x00000 )			/* Gray Image					*/
#define		PSMIMG_FORMAT_BGR		( 0x00001 )			/* Color Image BGR				*/
#define		PSMIMG_FORMAT_ZBGR		( 0x00002 )			/* Color Image ZBGR				*/
#define		PSMIMG_FORMAT_RGB		( 0x00004 )			/* Color Image RGB				*/
#define		PSMIMG_FORMAT_YRB		( 0x00008 )			/* Color Image YCrCb			*/

/****************************************************************************************
 *  Structure Definitions
 ****************************************************************************************/
typedef struct {

	int				x		;						/* X location[pixel]			*/
	int				y		;						/* Y location[pixel]			*/

} PSMPoint ;

typedef struct {

	int				x		;						/* X location[pixel]			*/
	int				y		;						/* Y location[pixel]			*/
	int				z		;						/* Z location[pixel]			*/

} PSMPoint3D ;

typedef struct {

	int				sx		;						/* Rect. Left-Top X [pixel]		*/
	int				sy		;						/* Rect. Left-Top Y [pixel]		*/
	int				ex		;						/* Rect. Right-Bottom X [pixel]	*/
	int				ey		;						/* Rect. Right-Bottom Y [pixel]	*/

} PSMRect ;

typedef struct {

	unsigned int		format	;						/* Image Format					*/
	unsigned char*		data	;						/* Pointer of Images Data		*/
	int					width	;						/* Width  of Image				*/
	int					height	;						/* Height of Image	  			*/

} PSMImg ;

typedef struct {

	int				xSize	;						/* Width  of Image				*/
	int				bSize	;						/* Width  of Image(1/8 size)	*/
	int				ySize	;						/* Height of Image				*/
	unsigned char		*pbImg	;						/* Pointer of Images Data		*/

} PSBImg ;

typedef struct {

	int				xSize	;						/* Width  of Image				  */
	int				ySize	;						/* Height of Image				  */
	unsigned char		*pgImg	;						/* Pointer of Images Data		  */

} PSGImg ;


/****************************************************************************************
 *  Global Variables
 ****************************************************************************************/

/****************************************************************************************
 *  Constants and Declarations
 ****************************************************************************************/

/****************************************************************************************
 *  Global Functions
 ****************************************************************************************/

#endif   /* _LIBPSM_H */
/*	End of File *************************************************************************/

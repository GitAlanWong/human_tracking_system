/****************************************************************************************
 *
 *		Multi Human Tracking Library (MHTLib)
 *
 *		MHTLib Native Header File
 *
 ****************************************************************************************/

#ifndef	_LIBMHT_API_H
#define _LIBMHT_API_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************************
 *  Definitions & Header Files
 ****************************************************************************************/
#include	"libmht_interface.h"
#include	"libpsm.h"

/*--------------------------------------------------------------------------------------*/
#define		MHT_IMAGE_WIDMIN			(    320 )	/* Minimum width of input image		*/
#define		MHT_IMAGE_WIDMAX 			(   1280 )	/* Maximum width of input image		*/
#define		MHT_IMAGE_HEIMIN			(    240 )	/* Minimum height of input image	*/
#define		MHT_IMAGE_HEIMAX			(    960 )	/* Maximum height of input image	*/
#define		MHT_IMAGE_BITC				(     24 )	/* Number of bit					*/
#define		MHT_IMAGE_MAPS				(      3 )	/* Number of color					*/
/*--------------------------------------------------------------------------------------*/
#define		MHT_HUMANSIZEMIN			(     32 )	/* Minimum Size of detection person	*/
#define		MHT_HUMANSIZEMAX			(    480 )	/* Maximum Size of detection person	*/
/*--------------------------------------------------------------------------------------*/
#define		MHT_HMD_DETTHRMIN			(      1 )	/* Minimum threshold of detection	*/
#define		MHT_HMD_DETTHRMAX			(     50 )	/* Maximum threshold of detection	*/
/*--------------------------------------------------------------------------------------*/
#define		MHT_HMD_SCANSTEPMIN			(      1 )	/* Minimum step of scan step		*/
#define		MHT_HMD_SCANSTEPMAX			(      3 )	/* Maximum step of scan step		*/
/*--------------------------------------------------------------------------------------*/
#define     MHT_HMD_MDFLAGOFF	        (      0 )  /* Off flag for motion detection	*/
#define     MHT_HMD_MDFLAGON	        (      1 )  /* On  flag for motion detection	*/
#define     MHT_HMD_MDSENSMIN	        (      0 )  /* Minimum sensibity of motion detection */                                 
#define     MHT_HMD_MDSENSMAX	        (     10 )  /* Maximum sensibity of motion detection */                                 
/*--------------------------------------------------------------------------------------*/
#define		MHT_HMD_INTERVALMIN			(      1 )	/* Minimum frame interval			*/
#define		MHT_HMD_INTERVALMAX			(     60 )	/* Maximum frame interval			*/
/*--------------------------------------------------------------------------------------*/
#define		MHT_OBJT_OBJECT_NUMMIN		(      1 )	/* Minimum object number			*/
#define		MHT_OBJT_OBJECT_NUMMAX		(     50 )	/* Maximum object number			*/
/*--------------------------------------------------------------------------------------*/
#define		MHT_OBJT_OBJECT_IDMIN		(      1 )	/* Minimum object ID				*/
#define		MHT_OBJT_OBJECT_IDMAX		(     50 )	/* Maximum object ID				*/
/*--------------------------------------------------------------------------------------*/
#define		MHT_OBJT_OBJECT_WIDMIN		(     32 )	/* Minimum object width				*/
#define		MHT_OBJT_OBJECT_WIDMAX		(    480 )	/* Maximum object width				*/
#define		MHT_OBJT_OBJECT_HEIMIN		(     32 )	/* Minimum object height			*/
#define		MHT_OBJT_OBJECT_HEIMAX		(    480 )	/* Maximum object height			*/
/*--------------------------------------------------------------------------------------*/
#define		MHT_OBJT_LEARN_FSTPMIN 		(      3 )	/* Minimum learning feature step	*/
#define		MHT_OBJT_LEARN_FSTPMAX 		(      7 )	/* Maximum learning feature step	*/
#define		MHT_OBJT_LEARN_FTYPMIN		(      0 )	/* Minimum learning feature type	*/
#define		MHT_OBJT_LEARN_FTYPMAX		(      2 )	/* Maximum learning feature	type	*/
#define		MHT_OBJT_LEARN_NUMSMIN		(      2 )	/* Minimum learning frame number	*/
#define		MHT_OBJT_LEARN_NUMSMAX		(     10 )	/* Maximum learning frame number	*/
#define		MHT_OBJT_LEARN_INTRMIN		(      1 )	/* Minimum learning update interval	*/
#define		MHT_OBJT_LEARN_INTRMAX		(     30 )	/* Maximum learning update interval	*/
#define		MHT_OBJT_LEARN_KEYSMIN		(     10 )	/* Minimum learning key points		*/
#define		MHT_OBJT_LEARN_KEYSMAX		(    150 )	/* Maximum learning key points		*/
/*--------------------------------------------------------------------------------------*/
#define		MHT_OBJT_TRACK_AREAMIN		(      2 )	/* Minimum tracking scan area		*/
#define		MHT_OBJT_TRACK_AREAMAX		(     16 )	/* Maximum tracking scan area		*/
#define		MHT_OBJT_TRACK_STEPMIN		(      1 )	/* Minimum tracking scan step		*/
#define		MHT_OBJT_TRACK_STEPMAX		(      5 )	/* Maximum tracking scan step		*/
#define		MHT_OBJT_TRACK_THRSMIN		(      1 )	/* Minimum tracking threshold		*/
#define		MHT_OBJT_TRACK_THRSMAX		(    100 )	/* Maximum tracking threshold		*/
/*--------------------------------------------------------------------------------------*/
#define		MHT_OBJT_DELETE_FRAMEMIN	(      0 )	/* Minimum number of frames 
                                                                           for deletion */
#define		MHT_OBJT_DELETE_FRAMEMAX	(    120 )	/* Maximum number of frames 
																		   for deletion */
#define		MHT_OBJT_DELETE_OVERLAPMIN	(      0 )	/* Minimum threshold(rate) 
																		   for deletion */
#define		MHT_OBJT_DELETE_OVERLAPMAX	(    100 )	/* Maximum threshold(rate) 
																		   for deletion	*/
#define		MHT_OBJT_TRACK_CANCELFLAGMIN	(  0 )	/* Minimum value of 
                                                                   tracking cancel flag */
#define		MHT_OBJT_TRACK_CANCELFLAGMAX	(  1 )	/* Maximum value of
																   tracking cancel flag */
/*--------------------------------------------------------------------------------------*/
#define		MHT_OBJT_STATUS_NEW			( 0x0001 )	/* Set area and start tracking		*/
#define		MHT_OBJT_STATUS_CONTINUE	( 0x0002 )	/* Continue tracking				*/
#define		MHT_OBJT_STATUS_DELETE		( 0x0004 )	/* Delete (stop tracking)			*/
/*--------------------------------------------------------------------------------------*/
#define		MHT_OBJT_RESULT_TRACK		( 0x0010 )	/* Tracking succeeded				*/
#define		MHT_OBJT_RESULT_LOST		( 0x0020 )	/* Tracking lost					*/
#define		MHT_OBJT_RESULT_ERROR		( 0x0040 )	/* Tracking error					*/
#define		MHT_OBJT_RESULT_HMDADD		( 0x0001 )	/* Human detection succeeded		*/
/*--------------------------------------------------------------------------------------*/

/****************************************************************************************
 *  Macroinstructions
 ****************************************************************************************/

/****************************************************************************************
 *  Type Definitions and Symbol Constant Definitions
 ****************************************************************************************/

/****************************************************************************************
 *  Structure Definitions
 ****************************************************************************************/
typedef struct {

	int				id					;			/* Object ID						*/
	int				state				;			/* Statement						*/
	int				sx					;			/* Tracking area upper left point x	*/
	int				sy					;			/* Tracking area upper left point y */
	int				ex					;			/* Tracking area lower right point x*/
	int				ey					;			/* Tracking area lower right point y*/

} MHTObject ;

typedef struct {
	unsigned char	learnFeatureStep	;			/* Learning feature step			*/
	unsigned char	learnFeatureType	;			/* Learning feature type			*/
	unsigned char	learnFrameNums		;			/* Learning number of frames		*/
	unsigned char	learnFrameInterval	;			/* Learning frame interval			*/
	unsigned char	learnKeypoints		;			/* Learning key points				*/

	unsigned char	trackArea			;			/* Tracking scan area				*/
	unsigned char	trackStep			;			/* Tracking scan steps				*/
	unsigned char	trackThreshold		;			/* Tracking threshold				*/

	int				objnum				;			/* Number of objects				*/
	MHTObject		object[MHT_OBJT_OBJECT_NUMMAX]	;	/* Objects						*/

	int				deletionFrame		;			/* Threshold of stop tracking		*/
	int				deletionOverlap		;			/* Threshold of overlap				*/
	int				trackCancelFlag		;			/* 0: Tracking priority mode
														   1: Detection priority mode	*/

} MHTObjtConfig ;

typedef struct {

	int				min					;			/* Min Size of detection person		*/
	int				minY				;			/* Min Y	of detection person		*/
	int				max					;			/* Max Size of detection person		*/
	int				maxY				;			/* Max Y	of detection person		*/

	int				reserve00			;			/* Reserve							*/
	int				reserve01			;			/* Reserve							*/
	int				reserve02			;			/* Reserve							*/
	int				reserve03			;			/* Reserve							*/

} MHTHumanSize ;

typedef struct {

	int				detThreshold		;			/* Threshold of detection			*/
	int				scanStep			;			/* To step through scan window(pixel)*/
	int				execInterval		;			/* Interval of humandetection running*/
	int				mdFlag				;			/* Flag of motion detection 0:OFF, 1:ON	*/
	int				mdSensitivity		;			/* Sensitivity of motion detection 0~10 	*/

} MHTHmdConfig ;

typedef struct {

	unsigned char*	pextBuff			;			/* Work memory area					*/
	unsigned char*	dic_mem				;			/* Dictionary Memory area			*/
	
	int				srcXsize			;			/* Source image width				*/
	int				srcYsize			;			/* Source image height				*/
	int				bitc				;			/* Number of bit  (bits/pixel)		*/
	int				maps				;			/* Number of color    (/pixel)		*/

	MHTHumanSize	humanSize			;			/* Min/Max Size of detection person	*/
	PSMRect 		targetArea			;			/* Processing target area			*/

	MHTHmdConfig	hmdConfig			;			/* HumanDetection config			*/
	MHTObjtConfig	objtConfig			;			/* ObjectTracking config			*/

} MHTConfig ;

typedef struct {

	int				objnum				;			/* Number of objects				*/
	MHTObject		object[MHT_OBJT_OBJECT_NUMMAX]	;	/* Objects						*/

} MHTResult ;

/****************************************************************************************
 *  Global Variables
 ****************************************************************************************/

/****************************************************************************************
 *  Constants and Declarations
 ****************************************************************************************/
#ifndef MHT_DLLEXPORT
#ifdef _WINDLL
#define		MHT_DLLEXPORT	__declspec( dllexport )
#else
#define		MHT_DLLEXPORT
#endif
#endif

/****************************************************************************************
 *  Global Functions
 ****************************************************************************************/

extern int	MHT_DLLEXPORT MHT_GetWorkSize	( MHTConfig*, unsigned int*				);
extern int	MHT_DLLEXPORT MHT_ProcessInit	( MHTConfig*				);
extern int	MHT_DLLEXPORT MHT_ProcessExec	( MHTConfig*, PSMImg*, PSBImg*, MHTResult*	);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif
/*	End of File *************************************************************************/

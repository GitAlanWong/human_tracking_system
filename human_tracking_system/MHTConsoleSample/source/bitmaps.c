/****************************************************************************************
 * Definitions & Header Files
 ****************************************************************************************/
#ifdef WIN32
#include <windows.h>
#endif // WIN32
#include <stdio.h>
#ifdef LINUX
#include <string.h>
#endif // LINUX

#ifdef WIN32
#pragma warning(disable:4996)
#endif // WIN32

#include "libpsm.h"
#include "bitmaps.h"

/****************************************************************************************
 * Macroinstructions
 ****************************************************************************************/

/****************************************************************************************
 * Type Definitions and Symbol Constant Definitions
 ****************************************************************************************/

/****************************************************************************************
 * Structure Definitions
 ****************************************************************************************/

/****************************************************************************************
 * Global Variables
 ****************************************************************************************/

/****************************************************************************************
 * Constants and Declarations
 ****************************************************************************************/

/****************************************************************************************
 * Global Functions
 ****************************************************************************************/


/****************************************************************************************
 *	Purpose		 : Open bitmap file and check, and get some parameters.
 *----------------------------------------------------------
 *	Input		 : filename		: file name
 *----------------------------------------------------------
 *	Output		 : imgInfo		: bitmap parameters (structure)
 *----------------------------------------------------------
 *	Return Value : TRUE(1)		: success 
 *				   error number	: otherwise
 *				   (Returns TRUE on success, an error number on failure.)
 *----------------------------------------------------------
 *	Date		 : 2001.08.10
 *----------------------------------------------------------
 *	Modified	 : ****.**.**
 ****************************************************************************************/
int CHECK_BMPFILE(char *filename, IMGI *imgInfo)
{
	BMFH	bmfHead;
	BMIH	bmiHead;
	FILE	*rfp;
	#ifdef	VC2K
	errno_t err     ;
	#endif//VC2K

	/***********************************************************
	 * Open file.
	 ***********************************************************/
	#ifdef	VC2K
	if( ( err=fopen_s( &rfp, filename, "rb" ) )!=0    ){
	#else
	if( ( rfp=fopen  (       filename, "rb" ) )==NULL ){
	#endif//VC2K
		return ERR100 ;
	}

	/***********************************************************
	 * Get header information (BITMAPFILEHEADER).
	 ***********************************************************/
	if ((fread(&bmfHead.bfType, sizeof(bmfHead.bfType), 1, rfp) != 1) ||
	    (fread(&bmfHead.bfSize, sizeof(bmfHead.bfSize), 1, rfp) != 1) ||
	    (fread(&bmfHead.bfRsv1, sizeof(bmfHead.bfRsv1), 1, rfp) != 1) ||
	    (fread(&bmfHead.bfRsv2, sizeof(bmfHead.bfRsv2), 1, rfp) != 1) ||
	    (fread(&bmfHead.bfBits, sizeof(bmfHead.bfBits), 1, rfp) != 1)){
		fclose(rfp);
		return ERR101 ;
	}
	#ifdef NP_BIG_ENDIAN
	(void) SwapData((char*)&bmfHead.bfType, sizeof(bmfHead.bfType));
	(void) SwapData((char*)&bmfHead.bfSize, sizeof(bmfHead.bfSize));
	(void) SwapData((char*)&bmfHead.bfRsv1, sizeof(bmfHead.bfRsv1));
	(void) SwapData((char*)&bmfHead.bfRsv2, sizeof(bmfHead.bfRsv2));
	(void) SwapData((char*)&bmfHead.bfBits, sizeof(bmfHead.bfBits));
	#endif//NP_BIG_ENDIAN

	/***********************************************************
	 * Check BITMAPFILEHEADER.
	 ***********************************************************/
	/*if ((bmfHead.bfType != 0x4d42) ||
		(bmfHead.bfRsv1 != 0x0000) ||
		(bmfHead.bfRsv2 != 0x0000)){
		fclose(rfp);
		return ERR102 ;
	}*/
	if ((bmfHead.bfRsv1 != 0x0000)){
		fclose(rfp);
		return ERR102 ;
	}
		
	/***********************************************************
	 * Get header information (BITMAPINFOHEADER).
	 ***********************************************************/
	if ((fread(&bmiHead.biSize, sizeof(bmiHead.biSize), 1, rfp) != 1) ||
		(fread(&bmiHead.biWdth, sizeof(bmiHead.biWdth), 1, rfp) != 1) ||
		(fread(&bmiHead.biHigh, sizeof(bmiHead.biHigh), 1, rfp) != 1) ||
		(fread(&bmiHead.biPlan, sizeof(bmiHead.biPlan), 1, rfp) != 1) ||
		(fread(&bmiHead.biBitc, sizeof(bmiHead.biBitc), 1, rfp) != 1) ||
		(fread(&bmiHead.biComp, sizeof(bmiHead.biComp), 1, rfp) != 1) ||
		(fread(&bmiHead.biImag, sizeof(bmiHead.biImag), 1, rfp) != 1) ||
		(fread(&bmiHead.biXppm, sizeof(bmiHead.biXppm), 1, rfp) != 1) ||
		(fread(&bmiHead.biYppm, sizeof(bmiHead.biYppm), 1, rfp) != 1) ||
		(fread(&bmiHead.biCuse, sizeof(bmiHead.biCuse), 1, rfp) != 1) ||
		(fread(&bmiHead.biCimp, sizeof(bmiHead.biCimp), 1, rfp) != 1)){
		fclose(rfp);
		return ERR103 ;
	}
	#ifdef NP_BIG_ENDIAN
	(void) SwapData((char*)&bmiHead.biSize, sizeof(bmiHead.biSize));
	(void) SwapData((char*)&bmiHead.biWdth, sizeof(bmiHead.biWdth));
	(void) SwapData((char*)&bmiHead.biHigh, sizeof(bmiHead.biHigh));
	(void) SwapData((char*)&bmiHead.biPlan, sizeof(bmiHead.biPlan));
	(void) SwapData((char*)&bmiHead.biBitc, sizeof(bmiHead.biBitc));
	(void) SwapData((char*)&bmiHead.biComp, sizeof(bmiHead.biComp));
	(void) SwapData((char*)&bmiHead.biImag, sizeof(bmiHead.biImag));
	(void) SwapData((char*)&bmiHead.biXppm, sizeof(bmiHead.biXppm));
	(void) SwapData((char*)&bmiHead.biYppm, sizeof(bmiHead.biYppm));
	(void) SwapData((char*)&bmiHead.biCuse, sizeof(bmiHead.biCuse));
	(void) SwapData((char*)&bmiHead.biCimp, sizeof(bmiHead.biCimp));
	#endif//NP_BIG_ENDIAN
	
	/***********************************************************
	 * Check BITMAPINFOHEADER.
	 ***********************************************************/
	if ((bmiHead.biPlan != 0x0001) ||
	   ((bmiHead.biBitc != 0x0001) &&
	    (bmiHead.biBitc != 0x0004) &&
	    (bmiHead.biBitc != 0x0008) &&
	    (bmiHead.biBitc != 0x0010) &&
	    (bmiHead.biBitc != 0x0018) &&
	    (bmiHead.biBitc != 0x0020))){
		fclose(rfp);
		return ERR104 ;
	}

	/***********************************************************
	 * Input parameters.
	 ***********************************************************/
	imgInfo->Bitc = bmiHead.biBitc;
	imgInfo->Wdth = bmiHead.biWdth;
	imgInfo->High = bmiHead.biHigh;
	imgInfo->Maps = (bmfHead.bfBits - 54) / 4;
	imgInfo->Xppm = bmiHead.biXppm;
	imgInfo->Yppm = bmiHead.biYppm;

	/***********************************************************
	 * Close file & return.
	 ***********************************************************/
	fclose(rfp);

	return TRUE ;
}

/****************************************************************************************
 *	Purpose		 : Open bitmap file and get bitmap data.
 *----------------------------------------------------------
 *	Input		 : filename		: file name
 *				   imgInfo		: bitmap parameters (structure)
 *----------------------------------------------------------
 *	Output		 : rgbQuad		: RGB color pallet
 *				   bimg			: binary image data
 *				   gimg			: gray image data
 *				   cimg			: full color image data
 *----------------------------------------------------------
 *	Return Value : TRUE(1)		: success 
 *				   error number	: otherwise
 *				   (Returns TRUE on success, an error number on failure.)
 *----------------------------------------------------------
 *	Date		 : 2002.08.15
 *----------------------------------------------------------
 *	Modified	 : ****.**.**
 ****************************************************************************************/
int LOAD_PSMIMG(char *filename, IMGI *imgInfo, RGBQ *rgbq, PSBImg *bimg, PSGImg *gimg, PSMImg *cimg)
{
	UINT32	i		= 0;
	UINT32	j		= 0;
	UINT32	tmp1	= 0;
	UINT32	tmp2	= 0;
	UINT32	tmp3	= 0;
	UINT32	tmp4	= 0;
	UINT32	tmp5	= 0;
	unsigned char	dum		= 0;
	unsigned char	px1[8];
	UINT32	bmpType	= 0;
	FILE			*rfp	= NULL;
	#ifdef	VC2K
	errno_t err                          ;
	#endif//VC2K

	UINT32	width	= 0;
	UINT32	height	= 0;
	unsigned char*	data	 = NULL;

	/***********************************************************
	 * Check arguments.
	 ***********************************************************/
	if( (filename==NULL)                 ||
		(imgInfo ==NULL)                 ||
		((gimg!=NULL)&&(rgbq ==NULL)) ||
		((bimg!=NULL)&&(gimg!=NULL))     ||
		((bimg!=NULL)&&(cimg!=NULL))     ||
		((gimg!=NULL)&&(cimg!=NULL))     ||
		((bimg==NULL)&&(gimg==NULL)&&(cimg==NULL)) ){
		return ERR100 ;
	}

	/***********************************************************
	 * Open file.
	 ***********************************************************/
	#ifdef	VC2K
	if( ( err=fopen_s( &rfp, filename, "rb" ) )!=0    ){
	#else
	if( ( rfp=fopen  (       filename, "rb" ) )==NULL ){
	#endif//VC2K
		return ERR101 ;
	}

	/***********************************************************
	 * Skip.
	 ***********************************************************/
	for(i=0; i<54; i++){
		if ((fread(&dum,1,1,rfp)!=1)){
			fclose(rfp);
			return ERR102 ;
		}
	}

	/***********************************************************
	 * Get header information (RGBQUAD).
	 ***********************************************************/
	if (imgInfo->Bitc<=8 ){
		for (i = 0; i < imgInfo->Maps; i++ ){
			if ((fread(&rgbq[i].rgbBlu,1,1,rfp)!=1) ||
				(fread(&rgbq[i].rgbGrn,1,1,rfp)!=1) ||
				(fread(&rgbq[i].rgbRed,1,1,rfp)!=1) ||
				(fread(&rgbq[i].rgbRsv,1,1,rfp)!=1) ){
				fclose(rfp);
				return ERR103 ;
			}
		}
	}
	
	/***********************************************************
	 * Check BMP file type.
	 ***********************************************************/
	if (imgInfo->Bitc<=8 ){
		bmpType  = (1 * imgInfo->Wdth / (32 /(UINT32)imgInfo->Bitc));
		bmpType += (1 * imgInfo->Wdth % (32 /(UINT32)imgInfo->Bitc) !=0 );
		bmpType *= 4 ;
		bmpType *= 8 / imgInfo->Bitc;
		bmpType -= imgInfo->Wdth;
	}
	else{
		bmpType  = (3 * imgInfo->Wdth / 4);
		bmpType += (3 * imgInfo->Wdth % 4 != 0);
		bmpType *= 4;
		bmpType -= 3 * imgInfo->Wdth;
	}

	/***********************************************************
	 * Set parameters of image data.
	 ***********************************************************/
	if     ( bimg!=NULL ){
		bimg->xSize = (SINT32)imgInfo->Wdth ;
		bimg->bSize = (SINT32)(imgInfo->Wdth/(8/(UINT32)imgInfo->Bitc))+(SINT32)(imgInfo->Wdth%(8/(UINT32)imgInfo->Bitc)!=0) ;
		bimg->ySize = (SINT32)imgInfo->High ;
	}
	else if( gimg!=NULL ){
		gimg->xSize = (SINT32)imgInfo->Wdth ;
		gimg->ySize = (SINT32)imgInfo->High ;
	}
	else if( cimg!=NULL ){
		cimg->width = (SINT32)imgInfo->Wdth ;
		cimg->height = (SINT32)imgInfo->High ;
	}

	/***********************************************************
	 * Get bitmap data.
	 ***********************************************************/
	if     ( bimg!=NULL ){
		tmp1 = ((UINT32)bimg->xSize+bmpType)/(8/(UINT32)imgInfo->Bitc) ;
		tmp2 = (UINT32)bimg->bSize ;
		tmp3 = ((UINT32)bimg->ySize-1)*tmp2 ;
		tmp4 = tmp1-tmp2 ;
		for( j=0 ; j<(UINT32)bimg->ySize ; j++ ){
			if( fread(bimg->pbImg+tmp3,tmp2,1,rfp)!=1 ){
				fclose( rfp ) ;
				return ERR104 ;
			}
			if( fread(px1,1,tmp4,rfp)!=tmp4 ){
				fclose( rfp ) ;
				return ERR105 ;
			}
			tmp3 -= tmp2 ;
		}
	}
	else if( gimg!=NULL ){
		tmp1 = ((UINT32)gimg->xSize+bmpType)/(8/(UINT32)imgInfo->Bitc) ;
		tmp2 = (UINT32)gimg->xSize ;
		tmp3 = ((UINT32)gimg->ySize-1)*tmp2 ;
		tmp4 = tmp1-tmp2 ;
		if( tmp4>0 ){
			for( j=0 ; j<(UINT32)gimg->ySize ; j++ ){
				if( fread(gimg->pgImg+tmp3,1,tmp2,rfp)!=tmp2 ){
					fclose( rfp ) ;
					return ERR106 ;
				}
				if( fread(px1,1,tmp4,rfp)!=tmp4 ){
					fclose( rfp ) ;
					return ERR107 ;
				}
				tmp3 -= tmp2 ;
			}
		}
		else{
			for( j=0 ; j<(UINT32)gimg->ySize ; j++ ){
				if( fread(gimg->pgImg+tmp3,1,tmp2,rfp)!=tmp2 ){
					fclose( rfp ) ;
					return ERR106 ;
				}
				tmp3 -= tmp2 ;
			}
		}
	}
	else if( cimg!=NULL ){
		//tmp1 = (UINT32)cimg->width+(UINT32)bmpType ;
		//tmp2 = (UINT32)cimg->width ;
		//tmp3 = ((UINT32)cimg->height-1)*tmp2 ;
		//tmp4 = 3*tmp2 ;
		//tmp5 = tmp1-tmp2 ;
		//if( tmp5>0 ){
		//	for( j=0 ; j<(UINT32)cimg->height ; j++ ){
		//		for( i=0 ; i<(UINT32)cimg->width ; i++ ){
		//			if( fread(cimg->data+tmp3+i,1,3,rfp)!=3 ){
		//				fclose( rfp ) ;
		//				return ERR108 ;
		//			}
		//		}
		//		if( fread(px1,1,tmp5,rfp)!=tmp5 ){
		//			fclose( rfp ) ;
		//			return ERR109 ;
		//		}
		//		tmp3 -= tmp2 ;
		//	}
		//}
		//else{
		//	for( j=0 ; j<(UINT32)cimg->height ; j++ ){
		//		for( i=0 ; i<(UINT32)cimg->width ; i++ ){
		//			if( fread(cimg->data+tmp3+i,1,3,rfp)!=3 ){
		//				fclose( rfp ) ;
		//				return ERR108 ;
		//			}
		//		}
		//		tmp3 -= tmp2 ;
		//	}
		//}


		// ---------------------------
		// old code
		
		width	= cimg->width;
		height	= cimg->height;
		data	= cimg->data;

		tmp1 = width * 3 + (UINT32)bmpType;	// 1 line size
		tmp2 = width * 3;							// data line size
		tmp3 = (height - 1) * tmp2;					// final line
		tmp4 = 0;
		tmp5 = tmp1-tmp2;							// padding size
		if (tmp5 > 0){
			for(j = 0; j < height; j++){
				// read data
				if (fread(data + tmp3, tmp2, 1, rfp) != 1){
					fclose(rfp);
					return -6;
				}
				// read padding
				if (fread(px1, 1, tmp5, rfp) != tmp5){
					fclose(rfp);
					return -7;
				}
				tmp3 -= tmp2;
			}
		}
		else{
			for(j = 0; j < height; j++){
				// read data
				if (fread(data + tmp3, tmp2, 1, rfp) != 1){
					fclose(rfp);
					return -6;
				}
				tmp3 -= tmp2;
			}
		}
	}

	/***********************************************************
	 * Close file & return.
	 ***********************************************************/
	fclose(rfp);

	return TRUE ;
}

/****************************************************************************************
 *	Purpose		 : Create new bitmap file.
 *----------------------------------------------------------
 *	Input		 : filename		: file name
 *				   rgbQuad		: RGB color pallet
 *				   bimg			: binary image data
 *				   gimg			: gray image data
 *				   cimg			: full color image data
 *----------------------------------------------------------
 *	Output		 : bitmap file "filename"
 *----------------------------------------------------------
 *	Return Value : TRUE(1)		: success 
 *				   error number	: otherwise
 *				   (Returns TRUE on success, an error number on failure.)
 *----------------------------------------------------------
 *	Date		 : 2012.11.05
 *----------------------------------------------------------
 *	Modified	 : ****.**.**
 ****************************************************************************************/
int SAVE_PSMIMG(char *filename, RGBQ *rgbQuad, PSBImg *bimg, PSGImg *gimg, PSMImg *cimg)
{
	BMFH			bmfHead;
	BMIH			bmiHead;
	FILE			*wfp	= NULL;
	UINT32	i                            ;
	UINT32	j		= 0;
	UINT32	tmp1	= 0;
	UINT32	tmp2	= 0;
	UINT32	tmp3	= 0;
	UINT32	tmp4	= 0;
	UINT32	tmp5	= 0;
	UINT32	bmpType	= 0;
	BYTE	px1[8]                       ;
	WORD	bitc                         ;
	#ifdef	VC2K
	errno_t err                          ;
	#endif//VC2K
	#ifdef	ZBGRZBGR
	SINT32	*k, z                        ;
	#endif//ZBGRZBGR

	UINT32	width	= 0;
	UINT32	height	= 0;
	unsigned char*	data	= NULL;

	/***********************************************************
	 * Check arguments.
	 ***********************************************************/
	if( ((bimg!=NULL)&&(gimg!=NULL)) ||
		((bimg!=NULL)&&(cimg!=NULL)) ||
		((gimg!=NULL)&&(cimg!=NULL)) ||
		((bimg==NULL)&&(gimg==NULL)&&(cimg==NULL)) ){
		return ERR100 ;
	}

	/***********************************************************
	 * Open file.
	 ***********************************************************/
	#ifdef	VC2K
	if( ( err=fopen_s( &wfp, filename, "wb" ) )!=0    ){
	#else
	if( ( wfp=fopen  (       filename, "wb" ) )==NULL ){
	#endif//VC2K
		return ERR101 ;
	}

	/***********************************************************
	 * Calculate parameters of header.
	 ***********************************************************/
	bitc    = 0 ; // VC++ WARNING
	bmpType = 0 ; // VC++ WARNING
	memset( &bmfHead, 0, sizeof(BMFH) ); // VC++ WARNING
	memset( &bmiHead, 0, sizeof(BMIH) ); // VC++ WARNING
	if( bimg!=NULL ){
		bitc     = 1 ;
		bmpType  = (UINT32)bimg->xSize/32 ;
		bmpType += ((UINT32)bimg->xSize%32!=0) ;
		bmpType *= 4 ;
		bmiHead.biImag = bmpType*(UINT32)bimg->ySize ;
		bmpType *= 8;
		bmpType -= (UINT32)bimg->xSize ;
		bmiHead.biWdth = (UINT32)bimg->xSize ;
		bmiHead.biHigh = (UINT32)bimg->ySize ;
		bmfHead.bfBits = 62 ;
		bmiHead.biBitc = bitc ;
		bmiHead.biXppm = 0;//(UINT32)(10000.0*(DOUB)bimg->Xdpi/254.0+.5) ;
		bmiHead.biYppm = 0;//(UINT32)(10000.0*(DOUB)bimg->Ydpi/254.0+.5) ;
	}
	else if( gimg!=NULL ){
		tmp1 = 256;//(UINT32)gimg->depth ;
		if( tmp1<2 ){
			fclose( wfp ) ;
			return ERR102 ;
		}
		bitc = 0 ;
		while( tmp1!=1 ){
			tmp1 /= 2 ;
			bitc++ ;
		}
		bmpType  = (UINT32)gimg->xSize/(32/(UINT32)bitc) ;
		bmpType += ((UINT32)gimg->xSize%(32/(UINT32)bitc)!=0) ;
		bmpType *= 4 ;
		bmiHead.biImag = bmpType*(UINT32)gimg->ySize ;
		bmpType *= 8/(UINT32)bitc ;
		bmpType -= (UINT32)gimg->xSize ;
		bmiHead.biWdth = (UINT32)gimg->xSize ;
		bmiHead.biHigh = (UINT32)gimg->ySize ;
		bmfHead.bfBits = 54+(4*256/*(UINT32)gimg->depth*/) ;
		bmiHead.biBitc = bitc ;
		bmiHead.biXppm = 0;//(UINT32)(10000.0*(DOUB)gimg->Xdpi/254.0+.5) ;
		bmiHead.biYppm = 0;//(UINT32)(10000.0*(DOUB)gimg->Ydpi/254.0+.5) ;
	}
	else if( cimg!=NULL ){
		bmpType  = 3*(UINT32)cimg->width/4 ;
		bmpType += (3*(UINT32)cimg->width%4!=0) ;
		bmpType *= 4 ;
		bmiHead.biImag = bmpType*(UINT32)cimg->height ;
		bmpType -= 3*(UINT32)cimg->width ;
		bmiHead.biWdth = (UINT32)cimg->width ;
		bmiHead.biHigh = (UINT32)cimg->height ;
		bmfHead.bfBits = 54 ;
		bmiHead.biBitc = 24 ;
		bmiHead.biXppm = 0;//(UINT32)(10000.0*(DOUB)cimg->Xdpi/254.0+.5) ;
		bmiHead.biYppm = 0;//(UINT32)(10000.0*(DOUB)cimg->Ydpi/254.0+.5) ;
	}
	bmfHead.bfSize = bmfHead.bfBits+bmiHead.biImag ;
	bmfHead.bfType = 0x4d42 ;
	bmfHead.bfRsv1 = 0 ;
	bmfHead.bfRsv2 = 0 ;
	bmiHead.biSize = 40 ;
	bmiHead.biPlan = 1 ;
	bmiHead.biComp = 0 ;
	bmiHead.biCuse = 0 ;
	bmiHead.biCimp = 0 ;

	/***********************************************************
	 * Write header information (BITMAPFILEHEADER).
	 ***********************************************************/
	if ((fwrite(&bmfHead.bfType, sizeof(bmfHead.bfType), 1, wfp) != 1) ||
		(fwrite(&bmfHead.bfSize, sizeof(bmfHead.bfSize), 1, wfp) != 1) ||
		(fwrite(&bmfHead.bfRsv1, sizeof(bmfHead.bfRsv1), 1, wfp) != 1) ||
		(fwrite(&bmfHead.bfRsv2, sizeof(bmfHead.bfRsv2), 1, wfp) != 1) ||
		(fwrite(&bmfHead.bfBits, sizeof(bmfHead.bfBits), 1, wfp) != 1) ){
		fclose(wfp);
		return ERR103 ;
	}

	/***********************************************************
	 * Write header information (BITMAPINFOHEADER).
	 ***********************************************************/
	if ((fwrite(&bmiHead.biSize, sizeof(bmiHead.biSize), 1, wfp) != 1) ||
		(fwrite(&bmiHead.biWdth, sizeof(bmiHead.biWdth), 1, wfp) != 1) ||
		(fwrite(&bmiHead.biHigh, sizeof(bmiHead.biHigh), 1, wfp) != 1) ||
		(fwrite(&bmiHead.biPlan, sizeof(bmiHead.biPlan), 1, wfp) != 1) ||
		(fwrite(&bmiHead.biBitc, sizeof(bmiHead.biBitc), 1, wfp) != 1) ||
		(fwrite(&bmiHead.biComp, sizeof(bmiHead.biComp), 1, wfp) != 1) ||
		(fwrite(&bmiHead.biImag, sizeof(bmiHead.biImag), 1, wfp) != 1) ||
		(fwrite(&bmiHead.biXppm, sizeof(bmiHead.biXppm), 1, wfp) != 1) ||
		(fwrite(&bmiHead.biYppm, sizeof(bmiHead.biYppm), 1, wfp) != 1) ||
		(fwrite(&bmiHead.biCuse, sizeof(bmiHead.biCuse), 1, wfp) != 1) ||
		(fwrite(&bmiHead.biCimp, sizeof(bmiHead.biCimp), 1, wfp) != 1) ){
		fclose(wfp);
		return ERR104 ;
	}

	/***********************************************************
	 * Write header information (RGBQUAD).
	 ***********************************************************/
	if( bimg!=NULL ){
		tmp1 = 0 ;
		tmp2 = 0 ;
		for( i=0 ; i<2 ; i++ ){
			if( fwrite(&tmp2,1,1,wfp)!=1 ){
				fclose( wfp ) ;
				return ERR105 ;
			}
			if( fwrite(&tmp2,1,1,wfp)!=1 ){
				fclose( wfp ) ;
				return ERR105 ;
			}
			if( fwrite(&tmp2,1,1,wfp)!=1 ){
				fclose( wfp ) ;
				return ERR105 ;
			}
			if( fwrite(&tmp1,1,1,wfp)!=1 ){
				fclose( wfp ) ;
				return ERR105 ;
			}
			tmp2 += 255 ;
		}
	}
	else if( gimg!=NULL ){
		for( i=0 ; i<256/*gimg->depth*/ ; i++ ){
			if( fwrite(rgbQuad+i,4,1,wfp)!=1 ){
				fclose( wfp ) ;
				return ERR106 ;
			}
		}
	}
	else if( cimg!=NULL ){}

	/***********************************************************
	 * Write bitmap data.
	 ***********************************************************/
	if( bimg!=NULL ){
		tmp1 = (bmiHead.biWdth+bmpType)/8 ;
		tmp2 = (UINT32)bimg->bSize ;
		tmp3 = (bmiHead.biHigh-1)*tmp2 ;
		tmp4 = tmp1-tmp2 ;
		for( j=0 ; j<bmiHead.biHigh ; j++ ){
			if( fwrite(bimg->pbImg+tmp3,tmp2,1,wfp)!=1 ){
				fclose( wfp ) ;
				return ERR107 ;
			}
			if( fwrite(px1,1,tmp4,wfp)!=tmp4 ){
				fclose( wfp ) ;
				return ERR108 ;
			}
			tmp3 -= tmp2 ;
		}
	}
	else if( gimg!=NULL ){
		tmp1 = (bmiHead.biWdth+bmpType)/(8/(UINT32)bitc) ;
		tmp2 = bmiHead.biWdth/(8/(UINT32)bitc)+(UINT32)(bmiHead.biWdth%(8/(UINT32)bitc)!=0) ;
		tmp3 = (bmiHead.biHigh-1)*tmp2 ;
		tmp4 = tmp1-tmp2 ;
		for( j=0 ; j<bmiHead.biHigh ; j++ ){
			if( fwrite(gimg->pgImg+tmp3,tmp2,1,wfp)!=1 ){
				fclose( wfp ) ;
				return ERR109 ;
			}
			if( fwrite(px1,1,tmp4,wfp)!=tmp4 ){
				fclose( wfp ) ;
				return ERR110 ;
			}
			tmp3 -= tmp2 ;
		}
	}
	else if( cimg!=NULL ){
		//tmp1 = bmiHead.biWdth+(UINT32)bmpType ;
		//tmp2 = bmiHead.biWdth ;
		//tmp3 = (bmiHead.biHigh-1)*tmp2 ;
		//tmp4 = 3*tmp2 ;
		//tmp5 = tmp1-tmp2 ;
		//for( j=0 ; j<bmiHead.biHigh ; j++ ){
		//	for( i=0 ; i<bmiHead.biWdth ; i++ ){
		//		#ifdef	ZBGRZBGR
		//		////cimg->pcImg[tmp3+i].B = 0xaa ;
		//		////cimg->pcImg[tmp3+i].G = 0xbb ;
		//		////cimg->pcImg[tmp3+i].R = 0xcc ;
		//		////cimg->pcImg[tmp3+i].Z = 0xdd ;
		//		k = (SINT32*)(cimg->data+tmp3+i) ;
		//		z = (*k)>>8                     ;
		//		if( fwrite(&z,3/*bytes*/,1,wfp)!=1 ){
		//			fclose( wfp ) ;
		//			return ERR111 ;
		//		}
		//		#else
		//		if( fwrite(cimg->data+tmp3+i,3/*bytes*/,1,wfp)!=1 ){
		//			fclose( wfp ) ;
		//			return ERR111 ;
		//		}
		//		#endif//ZBGRZBGR
		//	}
		//	/*if( fwrite(cimg->pcImg+tmp3,tmp4,1,wfp)!=1 ){
		//		fclose( wfp ) ;
		//		return ERR112 ;
		//	}*/
		//	if( fwrite(px1,1,tmp5,wfp)!=tmp5 ){
		//		fclose( wfp ) ;
		//		return ERR112 ;
		//	}
		//	tmp3 -= tmp2 ;
		//}


		// -----------------------------------
		// old code
		
		width	= cimg->width;
		height	= cimg->height;
		data	= cimg->data;

		tmp1 = width * 3 + (UINT32)bmpType;	// 1 line size
		tmp2 = width * 3;							// data 1line size
		tmp3 = (height - 1) * tmp2;					// Final line
		tmp4 = 0;
		tmp5 = tmp1 - tmp2;							// padding size
		for(j = 0; j < height; j++){
			// write data
			if (fwrite(data + tmp3, tmp2, 1, wfp)!=1 ){
				fclose(wfp);
				return ERR111;
			}

			// write padding
			if (fwrite(px1, 1, tmp5, wfp)!=tmp5 ){
				fclose(wfp);
				return ERR112;
			}
			tmp3 -= tmp2;
		}
	}

	/***********************************************************
	 * Close file & return.
	 ***********************************************************/
	fclose( wfp );

	return TRUE ;
}

/*	End of File *************************************************************************/


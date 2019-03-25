#include "MHTConsole.h"

unsigned char*		pextBuff;
unsigned char*		dicBuff;
MHTConfig			mhtConfig;
MHTResult			mhtResult;

PSMImg				psmSrcImg;
PSMImg				psmDstImg; 
IMGI				imagInfo;
PSBImg				psmMskImg;
IMGI				imagInfoMask;

int MHT_Init(void)
{
	int					ret							= 0;
	UINT32		workSize					= 0;
	long		dicSize						= 0;
	size_t		rsize						= 0;

	char				str[MAX_PATH_LENGTH];
	FILE				*rfp						= NULL;

	char				appRootDir[MAX_PATH_LENGTH];		// application root dir
	char				szImgFilePath[MAX_PATH_LENGTH];  //不要
	char				szImgListPath[MAX_PATH_LENGTH];

	//fprintf(stdout, "Program Start\n");

	memset(&(mhtConfig.hmdConfig),  0x00, sizeof(MHTHmdConfig));
	memset(&(mhtConfig.objtConfig), 0x00, sizeof(MHTObjtConfig));

	memset(&psmSrcImg, 0x00, sizeof(PSMImg));
	memset(&imagInfo,  0x00, sizeof(IMGI));
	SetAppRootDir(appRootDir);

	/************************************************************************************
 	 * Cheak BmpImage in Listfile
	 ************************************************************************************/
	sprintf(szImgListPath,"%s%s", appRootDir, IMAGE_LIST_FILE);
	rfp = fopen(szImgListPath, "r");
	if (rfp == NULL) {
		fprintf(stderr, "text file open (r) NG\n");
		return -1;
	}

	while(fgets(str, MAX_PATH_LENGTH, rfp) != NULL) {
		sprintf(szImgFilePath, "%s%s", appRootDir, str);
		if( CheckBMPfile(szImgFilePath) == -1){
			fclose(rfp);
			return -1;
		}
		break;
	}
	fclose(rfp);
	/************************************************************************************
 	 * Cheak BmpImage in Listfile
	 ************************************************************************************/


	
	/************************************************************************************
	 * Malloc "psmSrcImg" "psmDstImg".
	 ************************************************************************************/
	psmSrcImg.width	= imagInfo.Wdth;
	psmSrcImg.height= imagInfo.High;
	printf("width:%d height:%d\n",imagInfo.Wdth,imagInfo.High);
	psmSrcImg.format= PSMIMG_FORMAT_BGR;
	psmSrcImg.data	= (unsigned char *)malloc(imagInfo.Wdth * imagInfo.High * 
		                                                      sizeof(unsigned char) * 3);
	if (psmSrcImg.data == NULL) {
		fprintf( stderr,"malloc psmSrcImg NG\n");
		MHT_Free();
		return -1;
	}

	psmDstImg.width = imagInfo.Wdth;
	psmDstImg.height= imagInfo.High;
	psmDstImg.format= PSMIMG_FORMAT_BGR;
	psmDstImg.data  = (unsigned char *)malloc(imagInfo.Wdth * imagInfo.High *
		                                                      sizeof(unsigned char) * 3);
	if (psmDstImg.data == NULL) {
		fprintf( stderr,"malloc psmDstImg NG\n");
		MHT_Free();
		return -1;
	}

	/************************************************************************************
	 * Malloc "psmMskImg".
	 ************************************************************************************/
	psmMskImg.xSize	= imagInfo.Wdth;
	psmMskImg.ySize = imagInfo.High;
	psmMskImg.bSize = imagInfo.Wdth/8;
	psmMskImg.pbImg	= (unsigned char *)malloc(imagInfo.Wdth * imagInfo.High *
		                                              sizeof(unsigned char) / 8); // 1bpp
	memset(psmMskImg.pbImg,0xFF,imagInfo.Wdth * imagInfo.High * sizeof(unsigned char) / 8);

	/************************************************************************************
	 * Read Dictionary File.
	 ************************************************************************************/
	rfp = fopen(DIC_FILE, "rb");
	if (rfp == NULL) {
		fprintf(stderr, "dictionary file open (r) NG\n");
		return -1;
	}
	fseek( rfp, 0, SEEK_END );
	dicSize = ftell( rfp );
	fseek( rfp, 0, SEEK_SET );
	
	dicBuff = ( unsigned char * )malloc( dicSize );
	rsize = fread( dicBuff, dicSize, 1, rfp );
	if( rsize != 1 ){
		fprintf(stderr, "dictionary can't Read (r) NG\n" );
		return -1;
	}
	fclose( rfp );
	
	/************************************************************************************
	 * Initialize main parameters.
	 ************************************************************************************/
	mhtConfig.srcXsize						= imagInfo.Wdth;
	mhtConfig.srcYsize						= imagInfo.High;
	mhtConfig.bitc							= MHT_IMAGE_BITC;
	mhtConfig.maps							= MHT_IMAGE_MAPS;
	mhtConfig.dic_mem						= dicBuff;

	mhtConfig.humanSize.min					= 70;
	mhtConfig.humanSize.minY				= 70;
	mhtConfig.humanSize.max					= 400;
	mhtConfig.humanSize.maxY				= 400;
	mhtConfig.humanSize.reserve00			= 0;
	mhtConfig.humanSize.reserve01			= 0;
	mhtConfig.humanSize.reserve02			= 0;
	mhtConfig.humanSize.reserve03			= 0;
	
	mhtConfig.targetArea.sx					= 0;
	mhtConfig.targetArea.sy					= 0;
	mhtConfig.targetArea.ex					= mhtConfig.srcXsize - 1;
	mhtConfig.targetArea.ey					= mhtConfig.srcYsize - 1;

//	mhtConfig.hmdConfig.detThreshold		= 2;
//	mhtConfig.hmdConfig.scanStep			= 1;
//	mhtConfig.hmdConfig.execInterval		= 1;
	mhtConfig.hmdConfig.detThreshold		= 1;
	mhtConfig.hmdConfig.scanStep			= 3;
	mhtConfig.hmdConfig.execInterval		= 60;

	mhtConfig.hmdConfig.mdFlag              = 0;
	mhtConfig.hmdConfig.mdSensitivity       = 10;

	mhtConfig.objtConfig.learnFeatureStep	= 7;
	mhtConfig.objtConfig.learnFeatureType	= 1;
	mhtConfig.objtConfig.learnFrameNums		= 10;
	mhtConfig.objtConfig.learnFrameInterval	= 30;
	mhtConfig.objtConfig.learnKeypoints		= 10;
	mhtConfig.objtConfig.trackArea			= 2;
	mhtConfig.objtConfig.trackStep			= 5;
	mhtConfig.objtConfig.trackThreshold		= 100;
	mhtConfig.objtConfig.objnum				= MHT_OBJT_OBJECT_NUMMAX;
	mhtConfig.objtConfig.deletionFrame		= 0;
	mhtConfig.objtConfig.deletionOverlap	= 0;
	mhtConfig.objtConfig.trackCancelFlag	= 1;

	/************************************************************************************
	 * MHT_GetWorkSize
	 ************************************************************************************/
	ret = MHT_GetWorkSize(&mhtConfig, &workSize);
	if (ret != PSMERR_TRUE) {
		fprintf(stderr, "MHT_GetWorkSize NG:(%d)\n", ret);
		MHT_Free();
		return -1;
	}
	fprintf(stdout, "MHT_GetWorkSize OK - workSize:%d\n", workSize );
	pextBuff = NULL;
	pextBuff = (unsigned char*)malloc(workSize);
	if (pextBuff == NULL) {
		fprintf(stderr, "malloc pextBuff NG\n");
		MHT_Free();
		return -1;
	}

	/************************************************************************************
	 * Set work memory
	 ************************************************************************************/
	mhtConfig.pextBuff = pextBuff;

	/************************************************************************************
	 * MHT_ProcessInit
	 ************************************************************************************/
	ret = MHT_ProcessInit(&mhtConfig);

	if (ret != PSMERR_TRUE) {
		fprintf(stderr, "MHT_ProcessInit NG:(%d)\n", ret);
		MHT_Free();
		return -1;
	}
	fprintf(stdout, "MHT_ProcessInit OK\n");

	return 0;
}

void MHT_Free(void)
{
	/************************************************************************************
	 * Free all memory areas.
	 ************************************************************************************/
	if (pextBuff != NULL) {
		free(pextBuff);
		pextBuff = NULL;
	}

	if (dicBuff != NULL) {
		free(dicBuff);
		dicBuff = NULL;
	}

	if (psmSrcImg.data != NULL) {
		free(psmSrcImg.data);
		psmSrcImg.data = NULL;
	}

	if (psmDstImg.data != NULL) {
		free(psmDstImg.data);
		psmDstImg.data = NULL;
	}

	if (psmMskImg.pbImg != NULL) {
		free(psmMskImg.pbImg);
		psmMskImg.pbImg = NULL;
	}
}

int MHT_Exec(const char *pImageFileName,int *humnanNum,int *humnanId,int *humnanState,int *ox,int *oy,int *dx,int *dy)
{
	int				objnum						= 0;
	int 			i							= 0;
	int				ret							= 0;
	static	SINT32	state						= 0;
	static	SINT32	image_count					= 0;
	FILE			*rfp						= NULL;

	int				x							= 0;
	int				y							= 0;
	int				drw_sx						= 0;
	int				drw_sy						= 0;
	int				drw_ex						= 0;
	int				drw_ey						= 0;
	MHTObject*		object						= NULL;


	char			str[MAX_PATH_LENGTH];
	char			appRootDir[MAX_PATH_LENGTH]	; // application root dir
	char			szImgListPath[MAX_PATH_LENGTH];
	char			szImgFilePath[MAX_PATH_LENGTH];

	char			mskImgPath[MAX_PATH_LENGTH];

	char			*p;
	char			dstImgPath[MAX_PATH_LENGTH];
	char			tmpFileName[MAX_PATH_LENGTH];
	
	unsigned char m_drawColor[] = {255, 159, 59, 59, 159, 255, 59, 220, 255, 59, 255, 224,
				 59, 255, 159,  59, 255,  93,  90, 255,  59, 220, 255,  59, 255, 224,  59,
				255,  93,  59, 255,  59,  90, 155,  59, 255, 255,  59, 220, 224,  59, 255,
				159,  59, 255,  93,  59, 255,  59,  90, 255, 206, 109, 108, 108, 206, 141,
				206, 108, 172, 255, 159,  59,  59, 159, 255,  59, 220, 255,  59, 255, 224,
				 59, 255, 159,  59, 255,  93,  90, 255,  59, 220, 255,  59, 255, 224,  59,
				255,  93,  59, 255,  59,  90, 155,  59, 255, 255,  59, 220, 224,  59, 255,
				159,  59, 255,  93,  59, 255,  59,  90, 255, 206, 109, 108, 108, 206, 141,
				206, 108, 172, 255, 159,  59,  59, 159, 255,  59, 220, 255,  59, 255, 224,
				 59, 255, 159,  59, 255,  93,  90, 255,  59, 220, 255,  59, 255, 224,  59,
				255,  93,  59, 255, 159,  59,  59, 159, 255,  59, 220, 255,  59, 255, 224,
				 59, 255, 159,  59, 255,  93,  90, 255,  59, 220, 255,  59, 255, 224,  59,
				255,  93,  59, 255,  59,  90, 155,  59, 255, 255,  59, 220, 224,  59, 255,
				159,  59, 255,  93,  59, 255,  59,  90, 255, 206, 109, 108, 108, 206, 141,
				206, 108, 172, 255, 159,  59,  59, 159, 255,  59, 220, 255,  59, 255, 224,
				 59, 255, 159,  59, 255,  93,  90, 255,  59, 220, 255,  59, 255, 224,  59,
				255,  93,  59, 255,  59,  90, 155,  59, 255, 255,  59, 220, 224,  59, 255,
				159,  59, 255,  93,  59, 255,  59,  90, 255, 206, 109, 108, 108, 206, 141,
				206, 108, 172, 255, 159,  59,  59, 159, 255,  59, 220, 255,  59, 255, 224,
				 59, 255, 159,  59, 255,  93,  90, 255,  59, 220, 255,  59, 255, 224,  59,
				255,  93,  59
	};
		
	int				m_color[3];

	RGBQ			rgbq[1024];

	double			dwStTime					= 0.0;
	double			dwEnTime					= 0.0;
	
	/************************************************************************************
	 * Now start Tracking processing.
	 ************************************************************************************/
	sprintf(szImgFilePath,"%s", pImageFileName);
	if(CheckBMPfile(szImgFilePath) == -1){
		return -1;
	}

	/********************************************************************************
	* Load BITMAP data.
	********************************************************************************/
	ret = LOAD_PSMIMG(szImgFilePath, &imagInfo, rgbq, NULL, NULL, &psmSrcImg);
	if (ret != TRUE) {
		fprintf(stderr, "LOAD_BMPDATA NG\n");
		return -1;
	}

	/********************************************************************************
	* Set Output BMP filename
	********************************************************************************/
	strncpy(tmpFileName, szImgFilePath, MAX_PATH_LENGTH);
	p = strrchr(tmpFileName, (int)'/');
	if(p != NULL){
		*(p+1) = '\0';
	}else{}
	sprintf(dstImgPath, "%s%s%s", 
			            tmpFileName, "dst_", szImgFilePath + (p - tmpFileName + 1));
		
	/********************************************************************************
	 * MHT_ProcessExec
	********************************************************************************/
	dwStTime = get_time();

	//fprintf(stdout,"MHT_ProcessExec start\n");
	ret = MHT_ProcessExec(&mhtConfig, &psmSrcImg, &psmMskImg, &mhtResult);
	//fprintf(stdout,"MHT_ProcessExec end\n");
	dwEnTime = get_time();

	if (ret != PSMERR_TRUE){
		fprintf(stderr, "MHT_ProcessExec NG[%d] %s\n", ret, szImgFilePath);
	}else{
		//fprintf(stdout, "MHT_ProcessExec OK\n");
	}

	/********************************************************************************
	* Continue Tracking or Delete Object
	********************************************************************************/
	objnum = MHT_OBJT_OBJECT_NUMMAX;
	for(i=0; i< objnum; i++){
		if(mhtResult.object[i].state != MHT_OBJT_RESULT_ERROR && 
				                                         mhtResult.object[i].state != 0){
			mhtConfig.objtConfig.object[i].state = MHT_OBJT_STATUS_CONTINUE;
			mhtConfig.objtConfig.object[i].sx = 0;
			mhtConfig.objtConfig.object[i].sy = 0;
			mhtConfig.objtConfig.object[i].ex = 0;
			mhtConfig.objtConfig.object[i].ey = 0;
		}else{
			mhtConfig.objtConfig.object[i].state = MHT_OBJT_STATUS_DELETE;
			mhtConfig.objtConfig.object[i].sx = 0;
			mhtConfig.objtConfig.object[i].sy = 0;
			mhtConfig.objtConfig.object[i].ex = 0;
			mhtConfig.objtConfig.object[i].ey = 0;
		}
	}

	/********************************************************************************
	* Output Result
	********************************************************************************/
	//fprintf(stdout, "<Result> %s\n", szImgFilePath);
	//fprintf(stdout, "time :%.2f\n", dwEnTime - dwStTime);
	//fprintf(stdout, "num :%d\n", mhtResult.objnum);

	*humnanNum = mhtResult.objnum;

	for (i = 0; i < objnum; i++) {
		if(mhtResult.object[i].state != 0){
			/*fprintf(stdout, "[%d] (%d,%d) (%d,%d) state:%d\n",
					mhtResult.object[i].id,
					mhtResult.object[i].sx,
					mhtResult.object[i].sy,
					mhtResult.object[i].ex,
					mhtResult.object[i].ey,
					mhtResult.object[i].state
					);*/
			*humnanId = mhtResult.object[i].id;
			*humnanState = mhtResult.object[i].state;
			*ox = mhtResult.object[i].sx; *oy = mhtResult.object[i].sy;
			*dx = mhtResult.object[i].ex - mhtResult.object[i].sx;
			*dy = mhtResult.object[i].ey - mhtResult.object[i].sy;
		}
	}

	/********************************************************************************
	* Output Bitmap
	********************************************************************************/
	memmove(psmDstImg.data, psmSrcImg.data, imagInfo.Wdth * imagInfo.High *
			                                                sizeof(unsigned char) * 3);

	/************************************************************************************
	 * Return .
	 ************************************************************************************/
	//fprintf(stdout, "Program END\n");
	return 0;
}

void SetAppRootDir(char *appRootDir)
{
	sprintf(appRootDir, "./");
}

double get_time()
{
	// マルチコア環境の場合、getusageでは複数コアの合計時間になるため
	// 実時間を取得できるgettimeofdayを使用する
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
	//struct rusage usage;
	//getrusage(RUSAGE_SELF, &usage);
	//return usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec*1e-6;
}

int CheckBMPfile(char *szImgFilePath)
{
	int i = 0;
	int ret;

	/************************************************************************************
	 * Set BMP filename from list file.
	 ************************************************************************************/
	for (i = 0; i < MAX_PATH_LENGTH; i++) {
		if (szImgFilePath[i] == 10 || szImgFilePath[i] == 13) {
			szImgFilePath[i] = 0;
			break;
		}
	}

	/************************************************************************************
	 * Check bitmap file.
	 * Get image width and height.
	 ************************************************************************************/
	if ((ret = CHECK_BMPFILE(szImgFilePath, &imagInfo)) != TRUE) {
		fprintf(stderr,"cannot open  %s \n", szImgFilePath);
		fprintf(stderr,"CHECK_BMPFILE ERR : ret =%d \n", ret);
		return -1;
	}

	/************************************************************************************
	 * Check arguments.
	 ************************************************************************************/
	if ((imagInfo.Bitc != 24)) {
		fprintf(stderr,"BITC  ERR : BITC = %d : Must be 24(Color) ! \n", imagInfo.Bitc);
		return -1;
	}

	return 0;
}
/*	End of File *************************************************************************/

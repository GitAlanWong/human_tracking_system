#ifndef	__MHTCONSOLE_H
#define __MHTCONSOLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>

#include "libmht_api.h"
#include "bitmaps.h"

#define		IMAGE_LIST_FILE		"./list.txt"
#define		_MASK_IMAGE_FILE	"./maskNone.bmp"

#define		DIC_FILE			"./MHT_dict.bin"

#define				MAX_PATH_LENGTH		256

extern unsigned char*		pextBuff;
extern unsigned char*		dicBuff;
extern MHTConfig			mhtConfig;
extern MHTResult			mhtResult;

extern PSMImg				psmSrcImg;
extern PSMImg				psmDstImg; 
extern IMGI				imagInfo;
extern PSBImg				psmMskImg;
extern IMGI				imagInfoMask;

extern int MHT_Init(void);
extern int MHT_Exec(const char *pImageFileName,int *humnanNum,int *humnanId,int *humnanState,int *ox,int *oy,int *dx,int *dy);
extern void MHT_Free(void);

extern int CheckBMPfile(char *szImgFilePath);
extern void SetAppRootDir(char *appRootDir);
extern double get_time(void);

#endif

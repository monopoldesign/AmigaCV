#ifndef _FADERCVSEQG_H
#define _FADERCVSEQG_H

/******************************************************************************
* MUI-Custom-Class
* (C)2022 M.Volkel (mario.volkel@outlook.com)
*******************************************************************************/

// Comment templates

/******************************************************************************
*
*******************************************************************************/

/*-----------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "main.h"

/******************************************************************************
* Prototypes
*******************************************************************************/
ULONG faderCvSeqG_New(struct IClass *cl, Object *obj, struct opSet *msg);
ULONG faderCvSeqG_Slider(struct IClass *cl, Object *obj, struct MUIP_faderCvSeqG_SL_Level *msg);
DISPATCHER(faderCvSeqG_Dispatcher);

/******************************************************************************
* Definitions
*******************************************************************************/
#define TAGBASE_CLASS (TAG_USER | 0x80420000)
#define MUIM_faderCvSeqG_Slider		TAGBASE_CLASS + 1

#define MUIA_faderCvSeqG_Dest		TAGBASE_CLASS + 1
#define MUIA_faderCvSeqG_Channel	TAGBASE_CLASS + 2

struct faderCvSeqG_Data
{
	Object *SL_Level, *TXT_Level;
	LONG level;
	UBYTE *dest;
	UBYTE chn;
};

struct MUIP_faderCvSeqG_SL_Level {ULONG MethodID; LONG level; };

#endif

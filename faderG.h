#ifndef _FADERG_H
#define _FADERG_H

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
ULONG faderG_New(struct IClass *cl, Object *obj, struct opSet *msg);
DISPATCHER(faderG_Dispatcher);

/******************************************************************************
* Definitions
*******************************************************************************/
#define TAGBASE_CLASS (TAG_USER | 0x80420000)
#define MUIM_faderG_Finish		TAGBASE_CLASS + 1
#define MUIM_faderG_Slider		TAGBASE_CLASS + 2

#define MUIA_faderG_Dest		TAGBASE_CLASS + 1
#define MUIA_faderG_Channel		TAGBASE_CLASS + 2

struct faderG_Data
{
	Object *SL_Level, *TXT_Level;
	LONG level;
	UBYTE chn;
	UBYTE *dest;
};

struct MUIP_faderG_SL_Tempo {ULONG MethodID; LONG level; };

#endif

#ifndef _LFOC_H
#define _LFOC_H

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
ULONG lfoC_New(struct IClass *cl, Object *obj, struct opSet *msg);
ULONG lfoC_mAskMinMax(struct IClass *cl, Object *obj, struct MUIP_AskMinMax *msg);
ULONG lfoC_mDraw(struct IClass *cl, Object *obj, struct MUIP_Draw *msg);
ULONG lfoC_mUpdate(struct IClass *cl, Object *obj, struct MUIP_Draw *msg);
int lfoC_transformX(Object *obj, int x);
int lfoC_transformY(Object *obj, int y);
DISPATCHER(lfoC_Dispatcher);

/******************************************************************************
* Definitions
*******************************************************************************/
#define TAGBASE_CLASS (TAG_USER | 0x80420000)

#define MUIA_lfoC_Channel 	TAGBASE_CLASS + 1
#define MUIM_lfoC_Update	TAGBASE_CLASS + 1

struct lfoC_Data
{
	ULONG chn;
	UBYTE oldPhase;
};

#endif

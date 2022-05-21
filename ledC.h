#ifndef _LEDC_H
#define _LEDC_H

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
ULONG ledC_mNew(struct IClass *cl, Object *obj, struct opSet *msg);
ULONG ledC_mSet(struct IClass *cl, Object *obj, struct opSet *msg);
ULONG ledC_mAskMinMax(struct IClass *cl, Object *obj, struct MUIP_AskMinMax *msg);
ULONG ledC_mDraw(struct IClass *cl, Object *obj, struct MUIP_Draw *msg);
ULONG ledC_mUpdate(struct IClass *cl, Object *obj, struct MUIP_Draw *msg);
DISPATCHER(ledC_Dispatcher);

/******************************************************************************
* Definitions
*******************************************************************************/
#define TAGBASE_CLASS (TAG_USER | 0x80420000)

#define MUIA_ledC_Channel	TAGBASE_CLASS + 1
#define MUIA_ledC_Step		TAGBASE_CLASS + 2

struct ledC_Data
{
	UBYTE chn, step;
};

#endif

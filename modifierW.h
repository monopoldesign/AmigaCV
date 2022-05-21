#ifndef _MODIFIERW_H
#define _MODIFIERW_H

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
ULONG modifierW_New(struct IClass *cl, Object *obj, struct opSet *msg);
ULONG modifierW_Finish(struct IClass *cl, Object *obj, Msg msg);
DISPATCHER(modifierW_Dispatcher);

/******************************************************************************
* Definitions
*******************************************************************************/
#define TAGBASE_CLASS (TAG_USER | 0x80420000)
#define MUIM_modifierW_Finish	TAGBASE_CLASS + 1
#define MUIM_modifierW_Speed	TAGBASE_CLASS + 2
#define MUIM_modifierW_Wave		TAGBASE_CLASS + 3
#define MUIM_modifierW_Offset	TAGBASE_CLASS + 4
#define MUIM_modifierW_Update	TAGBASE_CLASS + 5

#define MUIA_modifierW_Channel 	TAGBASE_CLASS + 1

struct modifierW_Data
{
	Object *CY_Wave, *SL_Speed, *SL_Offset;
	Object *LFO, *FG[16];
	UBYTE chn;
};

struct MUIP_modifierW_CY_Wave {ULONG MethodID; LONG wave; };
struct MUIP_modifierW_SL_Speed {ULONG MethodID; LONG speed; };
struct MUIP_modifierW_SL_Offset {ULONG MethodID; LONG offset; };

#endif

#ifndef _CTRLW_H
#define _CTRLW_H

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
ULONG ctrlW_New(struct IClass *cl, Object *obj, struct opSet *msg);
ULONG ctrlW_Finish(struct IClass *cl, Object *obj, Msg msg);
ULONG ctrlW_Status(struct IClass *cl, Object *obj, Msg msg);
ULONG ctrlW_SL_Tempo(struct IClass *cl, Object *obj, struct MUIP_ctrlW_SL_Tempo *msg);
ULONG ctrlW_STR_Tempo(struct IClass *cl, Object *obj, struct MUIP_ctrlW_STR_Tempo *msg);
DISPATCHER(ctrlW_Dispatcher);

/******************************************************************************
* Definitions
*******************************************************************************/
#define TAGBASE_CLASS (TAG_USER | 0x80420000)
#define MUIM_ctrlW_Finish		TAGBASE_CLASS + 1
#define MUIM_ctrlW_Status		TAGBASE_CLASS + 2
#define MUIM_ctrlW_SL_Tempo		TAGBASE_CLASS + 3
#define MUIM_ctrlW_STR_Tempo	TAGBASE_CLASS + 4

struct ctrlW_Data
{
	Object *BT_Status, *STR_Tempo, *SL_Tempo, *TX_Status;
};

struct MUIP_ctrlW_SL_Tempo {ULONG MethodID; LONG tempo; };
struct MUIP_ctrlW_STR_Tempo {ULONG MethodID; UBYTE *tempo; };

#endif

#ifndef _MATRIXG_H
#define _MATRIXG_H

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
ULONG matrixG_New(struct IClass *cl, Object *obj, struct opSet *msg);
DISPATCHER(matrixG_Dispatcher);

/******************************************************************************
* Definitions
*******************************************************************************/
#define TAGBASE_CLASS (TAG_USER | 0x80420000)
#define MUIM_matrixG_Finish		TAGBASE_CLASS + 1
#define MUIM_matrixG_CheckBox	TAGBASE_CLASS + 2
#define MUIM_matrixG_Modify		TAGBASE_CLASS + 3

#define MUIA_matrixG_Dest		TAGBASE_CLASS + 1
#define MUIA_matrixG_Channel	TAGBASE_CLASS + 2

struct matrixG_Data
{
	Object *bt, *cb[8];

	UBYTE *dest;
	UBYTE chn;
};

struct MUIP_matrixG_CheckBox {ULONG MethodID; LONG id; LONG state; };

#endif

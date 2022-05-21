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
#include "ledC.h"
#include "help.h"

/******************************************************************************
* Class-Methods
*******************************************************************************/

/*-----------------------------------------------------------------------------
- OM_NEW
------------------------------------------------------------------------------*/
ULONG ledC_mNew(struct IClass *cl, Object *obj, struct opSet *msg)
{
	struct ledC_Data tmp = {0};
	struct TagItem *tags;

	tags = msg->ops_AttrList;
	tmp.chn = (UBYTE)GetTagData(MUIA_ledC_Channel, 0, tags);
	tmp.step = (UBYTE)GetTagData(MUIA_ledC_Step, 0, tags);

	if (obj = (Object *)DoSuperMethodA(cl, obj, (APTR)msg))
	{
		struct ledC_Data *data = INST_DATA(cl, obj);
		*data = tmp;
		return (ULONG)obj;
	}
	CoerceMethod(cl, obj, OM_DISPOSE);
	return 0;
}

/*-----------------------------------------------------------------------------
- ledC_mSet
------------------------------------------------------------------------------*/
ULONG ledC_mSet(struct IClass *cl, Object *obj, struct opSet *msg)
{
	MUI_Redraw(obj, MADF_DRAWUPDATE);
	return DoSuperMethodA(cl, obj, (Msg)msg);
}

/*-----------------------------------------------------------------------------
- AskMinMax-Method
------------------------------------------------------------------------------*/
ULONG ledC_mAskMinMax(struct IClass *cl, Object *obj, struct MUIP_AskMinMax *msg)
{
	DoSuperMethodA(cl, obj, (Msg)msg);

	msg->MinMaxInfo->MinWidth += 16;
	msg->MinMaxInfo->DefWidth += 16;
	msg->MinMaxInfo->MaxWidth += 16;

	msg->MinMaxInfo->MinHeight += 2;
	msg->MinMaxInfo->DefHeight += 2;
	msg->MinMaxInfo->MaxHeight += 2;

	return (0);
}

/*-----------------------------------------------------------------------------
- Draw-Method
------------------------------------------------------------------------------*/
ULONG ledC_mDraw(struct IClass *cl, Object *obj, struct MUIP_Draw *msg)
{
	struct ledC_Data *data = INST_DATA(cl, obj);

	DoSuperMethodA(cl, obj, (Msg)msg);

	SetAPen(_rp(obj), _dri(obj)->dri_Pens[TEXTPEN]);

	if (seqPos[data->chn] == data->step)
		SetAPen(_rp(obj), _dri(obj)->dri_Pens[FILLPEN]);

	RectFill(_rp(obj), _mleft(obj), _mtop(obj), _mright(obj), _mbottom(obj));

	return (0);
}

/*-----------------------------------------------------------------------------
- Class-Dispatcher
------------------------------------------------------------------------------*/
DISPATCHER(ledC_Dispatcher)
{
	switch (msg->MethodID)
	{
		case OM_NEW:
			return ledC_mNew(cl, obj, (APTR)msg);

		case OM_SET:
			return ledC_mSet(cl, obj, (APTR)msg);

		case MUIM_AskMinMax:
			return ledC_mAskMinMax(cl, obj, (APTR)msg);

		case MUIM_Draw:
			return ledC_mDraw(cl, obj, (struct MUIP_Draw *)msg);
	}

	return (DoSuperMethodA(cl, obj, msg));
}

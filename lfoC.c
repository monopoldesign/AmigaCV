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
#include "lfoC.h"
#include "help.h"

#define LFO_BORDER	8

/******************************************************************************
* Class-Methods
*******************************************************************************/
const BYTE sineWave[256] = {0,2,5,7,10,12,15,17,20,22,24,27,29,31,34,36,38,41,43,45,47,49,51,53,56,58,60,62,63,65,67,69,71,72,74,76,77,79,80,82,83,84,86,87,88,89,90,91,92,93,94,95,96,96,97,98,98,99,99,99,100,100,100,100,100,100,100,100,100,99,99,99,98,98,97,96,96,95,94,93,92,91,90,89,88,87,86,84,83,82,80,79,77,76,74,72,71,69,67,65,63,62,60,58,56,53,51,49,47,45,43,41,38,36,34,31,29,27,24,22,20,17,15,12,10,7,5,2,0,-2,-5,-7,-10,-12,-15,-17,-20,-22,-24,-27,-29,-31,-34,-36,-38,-41,-43,-45,-47,-49,-51,-53,-56,-58,-60,-62,-63,-65,-67,-69,-71,-72,-74,-76,-77,-79,-80,-82,-83,-84,-86,-87,-88,-89,-90,-91,-92,-93,-94,-95,-96,-96,-97,-98,-98,-99,-99,-99,-100,-100,-100,-100,-100,-100,-100,-100,-100,-99,-99,-99,-98,-98,-97,-96,-96,-95,-94,-93,-92,-91,-90,-89,-88,-87,-86,-84,-83,-82,-80,-79,-77,-76,-74,-72,-71,-69,-67,-65,-63,-62,-60,-58,-56,-53,-51,-49,-47,-45,-43,-41,-38,-36,-34,-31,-29,-27,-24,-22,-20,-17,-15,-12,-10,-7,-5,-2};

/*-----------------------------------------------------------------------------
- OM_NEW
------------------------------------------------------------------------------*/
ULONG lfoC_New(struct IClass *cl, Object *obj, struct opSet *msg)
{
	struct lfoC_Data tmp = {0};
	struct TagItem *tags;

	tags = msg->ops_AttrList;
	tmp.chn = (UBYTE)GetTagData(MUIA_lfoC_Channel, 0, tags);

	if (obj = (Object *)DoSuperMethodA(cl, obj, (APTR)msg))
	{
		struct lfoC_Data *data = INST_DATA(cl, obj);
		*data = tmp;

		return (ULONG)obj;
	}
	CoerceMethod(cl, obj, OM_DISPOSE);
	return 0;
}

/*-----------------------------------------------------------------------------
- AskMinMax-Method
------------------------------------------------------------------------------*/
ULONG mAskMinMax(struct IClass *cl, Object *obj, struct MUIP_AskMinMax *msg)
{
	DoSuperMethodA(cl, obj, (Msg)msg);

	msg->MinMaxInfo->MinWidth += 75;
	msg->MinMaxInfo->DefWidth += 75;
	msg->MinMaxInfo->MaxWidth += 75;

	msg->MinMaxInfo->MinHeight += 75;
	msg->MinMaxInfo->DefHeight += 75;
	msg->MinMaxInfo->MaxHeight += 75;

	return (0);
}

/*-----------------------------------------------------------------------------
- Draw-Method
------------------------------------------------------------------------------*/
ULONG mDraw(struct IClass *cl, Object *obj, struct MUIP_Draw *msg)
{
	int i, x;
	struct lfoC_Data *data = INST_DATA(cl, obj);

	DoSuperMethodA(cl, obj, (Msg)msg);

	/*
	** if MADF_DRAWOBJECT isn't set, we shouldn't draw anything.
	** MUI just wanted to update the frame or something like that.
	*/

	//if (!(msg->flags & MADF_DRAWOBJECT))
	//	return (0);

	SetAPen(_rp(obj), _dri(obj)->dri_Pens[DETAILPEN]);
	RectFill(_rp(obj), _mleft(obj), _mtop(obj), _mright(obj), _mbottom(obj));

	SetAPen(_rp(obj), _dri(obj)->dri_Pens[TEXTPEN]);

	x = lfoC_transformX(obj, lfoPos[data->chn]);

	Move(_rp(obj), x, _mtop(obj));
	Draw(_rp(obj), x, _mbottom(obj));

	/*
	for (i = 0; i < 255; i += 5)
	{
		int x = lfoC_transformX(obj, i);
		int y = lfoC_transformY(obj, sineWave[i]);

		WritePixel(_rp(obj), x, y);
	}
	*/
	return (0);
}

/*-----------------------------------------------------------------------------
- lfoC_transformX()
------------------------------------------------------------------------------*/
int lfoC_transformX(Object *obj, int x)
{
	int xMin = 0;
	int xMax = 255;
	return (_mleft(obj) + (LFO_BORDER / 2)) + (((_mwidth(obj) - LFO_BORDER) * (x - xMin)) / (xMax - xMin));
}

/*-----------------------------------------------------------------------------
- lfoC_transformY()
------------------------------------------------------------------------------*/
int lfoC_transformY(Object *obj, int y)
{
	int yMin = -100;
	int yMax = 100;
	return (_mtop(obj) + (LFO_BORDER / 2)) + (((_mheight(obj) - LFO_BORDER) * (y - yMin)) / (yMax - yMin));
}

/*-----------------------------------------------------------------------------
- Class-Dispatcher
------------------------------------------------------------------------------*/
DISPATCHER(lfoC_Dispatcher)
{
	switch (msg->MethodID)
	{
		case OM_NEW:
			return lfoC_New(cl, obj, (APTR)msg);

		case MUIM_AskMinMax:
			return (mAskMinMax(cl, obj, (APTR)msg));

		case MUIM_Draw:
			return (mDraw(cl, obj, (APTR)msg));
	}

	return (DoSuperMethodA(cl, obj, msg));
}

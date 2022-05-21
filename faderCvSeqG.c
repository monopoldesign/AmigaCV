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

/*********************ß*********************************************************
* Header-Files
*******************************************************************************/
#include "main.h"
#include "faderCvSeqG.h"
#include "ledC.h"
#include "help.h"

/******************************************************************************
* Global Variables
*******************************************************************************/

/******************************************************************************
* Class-Methods
*******************************************************************************/

/*-----------------------------------------------------------------------------
- OM_NEW
------------------------------------------------------------------------------*/
ULONG faderCvSeqG_mNew(struct IClass *cl, Object *obj, struct opSet *msg)
{
	struct faderCvSeqG_Data tmp = {0};
	struct TagItem *tags;

	tags = msg->ops_AttrList;
	tmp.dest = (APTR)GetTagData(MUIA_faderCvSeqG_Dest, 0, tags);
	tmp.chn = (UBYTE)GetTagData(MUIA_faderCvSeqG_Channel, 0, tags);

	sprintf(buffer, "%d", tmp.chn + 1);

	if (obj = (Object *)DoSuperNew(cl, obj,
			Child, VGroup,
				Child, HGroup,
					Child, RectangleObject,
					End,
					Child, TextObject,
						MUIA_Background, MUII_TextBack,
						MUIA_Frame, MUIV_Frame_None,
						MUIA_Text_Contents, buffer,
					End,
					Child, RectangleObject,
					End,
				End,
				Child, tmp.SL_Level = SliderObject,
					MUIA_Slider_Horiz, FALSE,
					MUIA_FixHeight, 50,
					MUIA_Slider_Reverse, TRUE,
					MUIA_Slider_Quiet, TRUE,
					MUIA_Slider_Min, 0,
					MUIA_Slider_Max, 127,
				End,
				Child, tmp.TXT_Level = TextObject,
					MUIA_FixWidth, 14,
					MUIA_Background, MUII_TextBack,
					MUIA_Frame, MUIV_Frame_Text,
				End,
				Child, tmp.led = NewObject(CL_ledC->mcc_Class, NULL, MUIA_ledC_Channel, tmp.chn, MUIA_Frame, MUIV_Frame_Gauge, TAG_DONE),
			End));
	{
		struct faderCvSeqG_Data *data = INST_DATA(cl, obj);
		*data = tmp;

		// Setup UI
		DoMethod(data->SL_Level, MUIM_Set, MUIA_Slider_Level, *(data->dest));
		DoMethod(data->TXT_Level, MUIM_SetAsString, MUIA_Text_Contents, "%ld", *(data->dest));

		// Setup Method for Slider
		DoMethod(data->SL_Level, MUIM_Notify, MUIA_Slider_Level, MUIV_EveryTime, obj, 2, MUIM_faderCvSeqG_Slider, MUIV_TriggerValue);

		return (ULONG)obj;
	}
	return 0;
}

/*-----------------------------------------------------------------------------
- faderCvSeqG_mSet
------------------------------------------------------------------------------*/
ULONG faderCvSeqG_mSet(struct IClass *cl, Object *obj, struct opSet *msg)
{
	struct faderCvSeqG_Data *data = INST_DATA(cl, obj);
	struct TagItem *tags;

	tags = msg->ops_AttrList;
	data->pos = (UBYTE)GetTagData(MUIA_faderCvSeqG_Position, 0, tags);
	set(data->led, MUIA_ledC_Position, data->pos);

	return DoSuperMethodA(cl, obj, (Msg)msg);
}

/*-----------------------------------------------------------------------------
- faderCvSeqG_Slider
------------------------------------------------------------------------------*/
ULONG faderCvSeqG_Slider(struct IClass *cl, Object *obj, struct MUIP_faderCvSeqG_SL_Level *msg)
{
	struct faderCvSeqG_Data *data = INST_DATA(cl, obj);

	*(data->dest) = msg->level;
	DoMethod(data->TXT_Level, MUIM_SetAsString, MUIA_Text_Contents, "%ld", msg->level);

	return 0;
}

/*-----------------------------------------------------------------------------
- Dispatcher
------------------------------------------------------------------------------*/
DISPATCHER(faderCvSeqG_Dispatcher)
{
	switch(msg->MethodID)
	{
		case OM_NEW:
			return faderCvSeqG_mNew(cl, obj, (APTR)msg);
		case OM_SET:
			return faderCvSeqG_mSet(cl, obj, (APTR)msg);
		case MUIM_faderCvSeqG_Slider:
			return faderCvSeqG_Slider(cl, obj, (APTR)msg);
	}

	return DoSuperMethodA(cl, obj, msg);
}

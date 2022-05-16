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
#include "faderG.h"
#include "help.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
UBYTE dest = 0;

/******************************************************************************
* Class-Methods
*******************************************************************************/

/*-----------------------------------------------------------------------------
- OM_NEW
------------------------------------------------------------------------------*/
ULONG faderG_New(struct IClass *cl, Object *obj, struct opSet *msg)
{
	struct faderG_Data tmp = {0};
	struct TagItem *tags;

	tags = msg->ops_AttrList;
	tmp.dest = (APTR)GetTagData(MUIA_faderG_Dest, dest, tags);
	tmp.chn = (UBYTE)GetTagData(MUIA_faderG_Channel, 0, tags);

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
					MUIA_FixHeight, 100,
					MUIA_Slider_Reverse, TRUE,
					MUIA_Slider_Quiet, TRUE,
					MUIA_Slider_Min, 0,
					MUIA_Slider_Max, 99,
				End,
				Child, tmp.TXT_Level = TextObject,
					MUIA_Background, MUII_TextBack,
					MUIA_Frame, MUIV_Frame_Text,
				End,
			End));
	{
		struct faderG_Data *data = INST_DATA(cl, obj);
		*data = tmp;

		// Setup UI
		DoMethod(data->SL_Level, MUIM_Set, MUIA_Slider_Level, *(data->dest));
		DoMethod(data->TXT_Level, MUIM_SetAsString, MUIA_Text_Contents, "%ld", *(data->dest));

		// Setup Method for Slider
		DoMethod(data->SL_Level, MUIM_Notify, MUIA_Slider_Level, MUIV_EveryTime, obj, 2, MUIM_faderG_Slider, MUIV_TriggerValue);

		return (ULONG)obj;
	}
	return 0;
}

/*-----------------------------------------------------------------------------
- faderG_Slider
------------------------------------------------------------------------------*/
ULONG faderG_Slider(struct IClass *cl, Object *obj, struct MUIP_faderG_SL_Tempo *msg)
{
	struct faderG_Data *data = INST_DATA(cl, obj);

	data->level = msg->level;
	*(data->dest) = data->level;
	DoMethod(data->TXT_Level, MUIM_SetAsString, MUIA_Text_Contents, "%ld", data->level);

	return 0;
}

/*-----------------------------------------------------------------------------
- Dispatcher
------------------------------------------------------------------------------*/
DISPATCHER(faderG_Dispatcher)
{
	switch(msg->MethodID)
	{
		case OM_NEW:
			return faderG_New(cl, obj, (APTR)msg);
		case MUIM_faderG_Slider:
			return faderG_Slider(cl, obj, (APTR)msg);
	}

	return DoSuperMethodA(cl, obj, msg);
}

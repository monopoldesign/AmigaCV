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
#include "ctrlW.h"
#include "help.h"

/******************************************************************************
* Class-Methods
*******************************************************************************/

/*-----------------------------------------------------------------------------
- OM_NEW
------------------------------------------------------------------------------*/
ULONG ctrlW_New(struct IClass *cl, Object *obj, struct opSet *msg)
{
	struct ctrlW_Data tmp = {0};

	if (obj = (Object *)DoSuperNew(cl, obj,
		//MUIA_Window_Title,			"Control",
		//MUIA_Window_ID,				MAKE_ID('C', 'T', 'R', 'L'),
		MUIA_Window_TopEdge,		12,
		MUIA_Window_LeftEdge,		0,
		MUIA_Window_Width,			656,
		MUIA_Window_Height,			64,
		MUIA_Window_Screen,			myScreen,
		MUIA_Window_Borderless,		TRUE,
		MUIA_Window_SizeGadget,		FALSE,
		MUIA_Window_CloseGadget,	FALSE,
		MUIA_Window_DepthGadget,	FALSE,
		MUIA_Window_DragBar,		FALSE,
		WindowContents,				HGroup,
										Child, tmp.BT_Play = SimpleButton("Play"),
										Child, tmp.BT_Stop = SimpleButton("Stop"),
										Child, tmp.TX_Tempo = TextObject,
												MUIA_FixWidth, 20,
												MUIA_Background, MUII_TextBack,
												MUIA_Frame, MUIV_Frame_Text,
												MUIA_Text_Contents, "120",
										End,
										Child, tmp.SL_Tempo = SliderObject,
											MUIA_FixWidth, 200,
											MUIA_Slider_Quiet, TRUE,
											MUIA_Slider_Min, 20,
											MUIA_Slider_Max, 240,
											MUIA_Slider_Level, 120,
										End,
										Child, tmp.TX_Status = TextObject,
											MUIA_FixWidth, 300,
											MUIA_Background, MUII_TextBack,
											MUIA_Frame, MUIV_Frame_Text,
											MUIA_Text_Contents, "Stopped...",
										End,
									End,
		TAG_MORE, msg->ops_AttrList))
	{
		struct ctrlW_Data *data = INST_DATA(cl, obj);
		*data = tmp;

		DoMethod(data->BT_Play, MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_ctrlW_Status, 1);
		DoMethod(data->BT_Stop, MUIM_Notify, MUIA_Pressed, FALSE, obj, 2, MUIM_ctrlW_Status, 0);
		DoMethod(data->SL_Tempo, MUIM_Notify, MUIA_Slider_Level, MUIV_EveryTime, obj, 2, MUIM_ctrlW_Tempo, MUIV_TriggerValue);

		return (ULONG)obj;
	}
	return 0;
}

ULONG ctrlW_Finish(struct IClass *cl, Object *obj, Msg msg)
{
	DoMethod((Object *)xget(obj, MUIA_ApplicationObject), MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);
	return 0;
}

ULONG ctrlW_Status(struct IClass *cl, Object *obj, struct MUIP_ctrlW_Status *msg)
{
	struct ctrlW_Data *data = INST_DATA(cl, obj);

	if (msg->status)
	{
		isPlaying = TRUE;
		DoMethod(data->TX_Status, MUIM_Set, MUIA_Text_Contents, "Playing...");
	}
	else
	{
		isPlaying = FALSE;
		DoMethod(data->TX_Status, MUIM_Set, MUIA_Text_Contents, "Stopped...");
	}
	return 0;
}

ULONG ctrlW_Tempo(struct IClass *cl, Object *obj, struct MUIP_ctrlW_Tempo *msg)
{
	struct ctrlW_Data *data = INST_DATA(cl, obj);

	sprintf(buffer, "%d", msg->level);
	DoMethod(data->TX_Tempo, MUIM_Set, MUIA_Text_Contents, buffer);
	tempo = msg->level;

	return 0;
}

/*-----------------------------------------------------------------------------
- Dispatcher
------------------------------------------------------------------------------*/
DISPATCHER(ctrlW_Dispatcher)
{
	switch(msg->MethodID)
	{
		case OM_NEW:
			return ctrlW_New(cl, obj, (APTR)msg);
		case MUIM_ctrlW_Finish:
			return ctrlW_Finish(cl, obj, (APTR)msg);
		case MUIM_ctrlW_Status:
			return ctrlW_Status(cl, obj, (APTR)msg);
		case MUIM_ctrlW_Tempo:
			return ctrlW_Tempo(cl, obj, (APTR)msg);
	}

	return DoSuperMethodA(cl, obj, msg);
}

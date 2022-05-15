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
										Child, HGroup,
											MUIA_FixWidth, 75,
											Child, tmp.BT_Status = SimpleButton("Play"),
											Child, tmp.STR_Tempo = StringObject,
												MUIA_Frame, MUIV_Frame_String,
												MUIA_String_Accept, "0123456789",
												MUIA_String_MaxLen, 4,
											End,
										End,
										Child, tmp.SL_Tempo = SliderObject,
											MUIA_FixWidth, 200,
											MUIA_Slider_Quiet, TRUE,
											MUIA_Slider_Min, 20,
											MUIA_Slider_Max, 240,
										End,
										Child, tmp.TX_Status = TextObject,
											MUIA_Background, MUII_TextBack,
											MUIA_Frame, MUIV_Frame_Text,
											MUIA_Text_Contents, "Stopped...",
										End,
									End,
		TAG_MORE, msg->ops_AttrList))
	{
		struct ctrlW_Data *data = INST_DATA(cl, obj);
		*data = tmp;

		// Setup UI
		DoMethod(data->STR_Tempo, MUIM_SetAsString, MUIA_String_Contents, "%ld", tempo);
		DoMethod(data->SL_Tempo, MUIM_Set, MUIA_Slider_Level, tempo);

		// Setup Methods
		DoMethod(data->BT_Status, MUIM_Notify, MUIA_Pressed, FALSE, obj, 1, MUIM_ctrlW_Status);
		DoMethod(data->STR_Tempo, MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, obj, 2, MUIM_ctrlW_STR_Tempo, MUIV_TriggerValue);
		DoMethod(data->SL_Tempo, MUIM_Notify, MUIA_Slider_Level, MUIV_EveryTime, obj, 2, MUIM_ctrlW_SL_Tempo, MUIV_TriggerValue);

		return (ULONG)obj;
	}
	return 0;
}

/*-----------------------------------------------------------------------------
- ctrlW_Finish
------------------------------------------------------------------------------*/
ULONG ctrlW_Finish(struct IClass *cl, Object *obj, Msg msg)
{
	DoMethod((Object *)xget(obj, MUIA_ApplicationObject), MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);
	return 0;
}

/*-----------------------------------------------------------------------------
- ctrlW_Status
------------------------------------------------------------------------------*/
ULONG ctrlW_Status(struct IClass *cl, Object *obj, Msg msg)
{
	struct ctrlW_Data *data = INST_DATA(cl, obj);

	if (isPlaying)
	{
		isPlaying = FALSE;
		DoMethod(data->TX_Status, MUIM_Set, MUIA_Text_Contents, "Stopped...");
		DoMethod(data->BT_Status, MUIM_Set, MUIA_Text_Contents, "Play");
	}
	else
	{
		isPlaying = TRUE;
		DoMethod(data->TX_Status, MUIM_Set, MUIA_Text_Contents, "Playing...");
		DoMethod(data->BT_Status, MUIM_Set, MUIA_Text_Contents, "Stop");
	}

	return 0;
}

/*-----------------------------------------------------------------------------
- ctrlW_SL_Tempo
------------------------------------------------------------------------------*/
ULONG ctrlW_SL_Tempo(struct IClass *cl, Object *obj, struct MUIP_ctrlW_SL_Tempo *msg)
{
	struct ctrlW_Data *data = INST_DATA(cl, obj);

	tempo = msg->tempo;
	DoMethod(data->STR_Tempo, MUIM_SetAsString, MUIA_String_Contents, "%ld", tempo);

	return 0;
}

/*-----------------------------------------------------------------------------
- ctrlW_STR_Tempo
------------------------------------------------------------------------------*/
ULONG ctrlW_STR_Tempo(struct IClass *cl, Object *obj, struct MUIP_ctrlW_STR_Tempo *msg)
{
	struct ctrlW_Data *data = INST_DATA(cl, obj);
	ULONG tmp;

	tmp = atoi(msg->tempo);

	if (tmp >= 20 && tmp <= 240)
		tempo = tmp;

	DoMethod(data->STR_Tempo, MUIM_SetAsString, MUIA_String_Contents, "%ld", tempo);
	DoMethod(data->SL_Tempo, MUIM_Set, MUIA_Slider_Level, tempo);

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
		case MUIM_ctrlW_SL_Tempo:
			return ctrlW_SL_Tempo(cl, obj, (APTR)msg);
		case MUIM_ctrlW_STR_Tempo:
			return ctrlW_STR_Tempo(cl, obj, (APTR)msg);
	}

	return DoSuperMethodA(cl, obj, msg);
}

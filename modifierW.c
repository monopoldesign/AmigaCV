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
#include "modifierW.h"
#include "lfoC.h"
#include "faderCvSeqG.h"
#include "help.h"

char winTitle[32 * 8];
char *waveForm[5];

/******************************************************************************
* Class-Methods
*******************************************************************************/

/*-----------------------------------------------------------------------------
- OM_NEW
------------------------------------------------------------------------------*/
ULONG modifierW_New(struct IClass *cl, Object *obj, struct opSet *msg)
{
	struct modifierW_Data tmp = {0};
	struct TagItem *tags;

	tags = msg->ops_AttrList;
	tmp.chn = (UBYTE)GetTagData(MUIA_modifierW_Channel, 0, tags);

	waveForm[0] = "Sine";
	waveForm[1] = "Triangle";
	waveForm[2] = "Saw";
	waveForm[3] = "Square";
	waveForm[4] = NULL;

	switch (modType[tmp.chn])
	{
		case MOD_LFO:
			sprintf(winTitle + (32 * tmp.chn), "MOD_LFO - Channel %d", tmp.chn + 1);
			obj = (Object *)DoSuperNew(cl, obj,
								MUIA_Window_Title,			winTitle + (32 * tmp.chn),
								MUIA_Window_TopEdge,		220,
								MUIA_Window_LeftEdge,		20,
								MUIA_Window_Screen,			myScreen,
								MUIA_Window_SizeGadget,		FALSE,
								MUIA_Window_DepthGadget,	FALSE,
								WindowContents,				HGroup,
																Child, VGroup,
																	MUIA_Group_SameWidth, TRUE,
																	Child, tmp.CY_Wave = CycleObject,
																		MUIA_Frame, MUIV_Frame_Button,
																		MUIA_Cycle_Entries, waveForm,
																	End,
																	Child, HGroup,
																		Child, VGroup,
																			Child, tmp.SL_Speed = SliderObject,
																				MUIA_Slider_Horiz, FALSE,
																				MUIA_Slider_Quiet, TRUE,
																				MUIA_Slider_Min, 1,
																				MUIA_Slider_Max, 99,
																			End,
																			Child, Label("Freq"),
																		End,
																		Child, VGroup,
																			Child, tmp.SL_Offset = SliderObject,
																				MUIA_Slider_Horiz, FALSE,
																				MUIA_Slider_Quiet, TRUE,
																				MUIA_Slider_Min, -100,
																				MUIA_Slider_Max, 100,
																			End,
																			Child, Label("Offset"),
																		End,
																	End,
																End,
																Child, myLFO[tmp.chn] = NewObject(CL_lfoC->mcc_Class, NULL, MUIA_lfoC_Channel, tmp.chn, TextFrame, MUIA_Background, MUII_BACKGROUND, TAG_DONE),
															End,
								TAG_MORE, msg->ops_AttrList);
			break;

		case MOD_CVSEQ:
			sprintf(winTitle + (32 * tmp.chn), "MOD_CVSEQ - Channel %d", tmp.chn + 1);
			obj = (Object *)DoSuperNew(cl, obj,
								MUIA_Window_Title,			winTitle + (32 * tmp.chn),
								MUIA_Window_TopEdge,		220,
								MUIA_Window_LeftEdge,		20,
								MUIA_Window_Screen,			myScreen,
								MUIA_Window_SizeGadget,		FALSE,
								MUIA_Window_DepthGadget,	FALSE,
								WindowContents,				HGroup,
																/*
																Child, HGroup,
																	Child, VGroup,
																		Child, Label("Prescale:"),
																		Child, RectangleObject,
																			MUIA_FixHeight, 3,
																		End,
																		Child, Label("First:"),
																		Child, RectangleObject,
																			MUIA_FixHeight, 3,
																		End,
																		Child, Label("Last:"),
																	End,
																	Child, VGroup,
																		Child, StringObject,
																			MUIA_FixWidth, 20,
																			MUIA_Frame, MUIV_Frame_String,
																			MUIA_String_Accept, "0123456789",
																			MUIA_String_MaxLen, 3,
																			MUIA_String_Format, MUIV_String_Format_Center,
																		End,
																		Child, StringObject,
																			MUIA_FixWidth, 20,
																			MUIA_Frame, MUIV_Frame_String,
																			MUIA_String_Accept, "0123456789",
																			MUIA_String_MaxLen, 3,
																			MUIA_String_Format, MUIV_String_Format_Center,
																		End,
																		Child, StringObject,
																			MUIA_FixWidth, 20,
																			MUIA_Frame, MUIV_Frame_String,
																			MUIA_String_Accept, "0123456789",
																			MUIA_String_MaxLen, 3,
																			MUIA_String_Format, MUIV_String_Format_Center,
																		End,
																	End,
																End,
																*/
																Child, HGroup,
																	Child, tmp.FG[0] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[0], MUIA_faderCvSeqG_Channel, 0, TAG_DONE),
																	Child, tmp.FG[1] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[1], MUIA_faderCvSeqG_Channel, 1, TAG_DONE),
																	Child, tmp.FG[2] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[2], MUIA_faderCvSeqG_Channel, 2, TAG_DONE),
																	Child, tmp.FG[3] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[3], MUIA_faderCvSeqG_Channel, 3, TAG_DONE),
																	Child, tmp.FG[4] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[4], MUIA_faderCvSeqG_Channel, 4, TAG_DONE),
																	Child, tmp.FG[5] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[5], MUIA_faderCvSeqG_Channel, 5, TAG_DONE),
																	Child, tmp.FG[6] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[6], MUIA_faderCvSeqG_Channel, 6, TAG_DONE),
																	Child, tmp.FG[7] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[7], MUIA_faderCvSeqG_Channel, 7, TAG_DONE),
																	Child, tmp.FG[8] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[8], MUIA_faderCvSeqG_Channel, 8, TAG_DONE),
																	Child, tmp.FG[9] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[9], MUIA_faderCvSeqG_Channel, 9, TAG_DONE),
																	Child, tmp.FG[10] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[10], MUIA_faderCvSeqG_Channel, 10, TAG_DONE),
																	Child, tmp.FG[11] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[11], MUIA_faderCvSeqG_Channel, 11, TAG_DONE),
																	Child, tmp.FG[12] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[12], MUIA_faderCvSeqG_Channel, 12, TAG_DONE),
																	Child, tmp.FG[13] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[13], MUIA_faderCvSeqG_Channel, 13, TAG_DONE),
																	Child, tmp.FG[14] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[14], MUIA_faderCvSeqG_Channel, 14, TAG_DONE),
																	Child, tmp.FG[15] = NewObject(CL_faderCvSeqG->mcc_Class, NULL, MUIA_faderCvSeqG_Dest, &CVSeq[15], MUIA_faderCvSeqG_Channel, 15, TAG_DONE),
																End,
															End,
								TAG_MORE, msg->ops_AttrList);
			break;

		case MOD_DC:
			sprintf(winTitle + (32 * tmp.chn), "MOD_DC - Channel %d", tmp.chn + 1);
			obj = (Object *)DoSuperNew(cl, obj,
								MUIA_Window_Title,			winTitle + (32 * tmp.chn),
								MUIA_Window_TopEdge,		220,
								MUIA_Window_LeftEdge,		20,
								MUIA_Window_Screen,			myScreen,
								MUIA_Window_SizeGadget,		FALSE,
								MUIA_Window_DepthGadget,	FALSE,
								WindowContents,				VGroup,
																Child, SliderObject,
																	MUIA_FixWidth, 200,
																	MUIA_Slider_Quiet, TRUE,
																	MUIA_Slider_Min, 1,
																	MUIA_Slider_Max, 99,
																End,
															End,
								TAG_MORE, msg->ops_AttrList);
			break;
	}

	if (obj)
	{
		struct modifierW_Data *data = INST_DATA(cl, obj);
		*data = tmp;

		// Setup GUI
		DoMethod(data->CY_Wave, MUIM_Set, MUIA_Cycle_Active, LFOWave[data->chn]);
		DoMethod(data->SL_Speed, MUIM_Set, MUIA_Slider_Level, LFOSpeed[data->chn]);

		// Setup Methods
		DoMethod(data->CY_Wave, MUIM_Notify, MUIA_Cycle_Active, MUIV_EveryTime, obj, 2, MUIM_modifierW_Wave, MUIV_TriggerValue);
		DoMethod(data->SL_Speed, MUIM_Notify, MUIA_Slider_Level, MUIV_EveryTime, obj, 2, MUIM_modifierW_Speed, MUIV_TriggerValue);
		DoMethod(data->SL_Offset, MUIM_Notify, MUIA_Slider_Level, MUIV_EveryTime, obj, 2, MUIM_modifierW_Offset, MUIV_TriggerValue);

		// Close Window
		DoMethod(obj, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, obj, 2, MUIM_modifierW_Finish, 0);
		return (ULONG)obj;
	}
	return 0;
}

/*-----------------------------------------------------------------------------
- modifierW_Finish
------------------------------------------------------------------------------*/
ULONG modifierW_Finish(struct IClass *cl, Object *obj, Msg msg)
{
	struct modifierW_Data *data = INST_DATA(cl, obj);

	set(obj, MUIA_Window_Open, FALSE);
	DoMethod((Object *)xget(obj, MUIA_ApplicationObject), OM_REMMEMBER, obj);
	modifierW[data->chn] = NULL;
	myLFO[data->chn] = NULL;
	MUI_DisposeObject(obj);

	return 0;
}

/*-----------------------------------------------------------------------------
- modifierW_Wave
------------------------------------------------------------------------------*/
ULONG modifierW_Wave(struct IClass *cl, Object *obj, struct MUIP_modifierW_CY_Wave *msg)
{
	struct modifierW_Data *data = INST_DATA(cl, obj);

	LFOWave[data->chn] = msg->wave;
	DoMethod(myLFO[data->chn], MUIM_Draw);
	return 0;
}

/*-----------------------------------------------------------------------------
- modifierW_Speed
------------------------------------------------------------------------------*/
ULONG modifierW_Speed(struct IClass *cl, Object *obj, struct MUIP_modifierW_SL_Speed *msg)
{
	struct modifierW_Data *data = INST_DATA(cl, obj);

	LFOSpeed[data->chn] = msg->speed;
	return 0;
}

/*-----------------------------------------------------------------------------
- modifierW_Offset
------------------------------------------------------------------------------*/
ULONG modifierW_Offset(struct IClass *cl, Object *obj, struct MUIP_modifierW_SL_Offset *msg)
{
	struct modifierW_Data *data = INST_DATA(cl, obj);

	LFOOffset[data->chn] = msg->offset;
	DoMethod(myLFO[data->chn], MUIM_Draw);
	return 0;
}

/*-----------------------------------------------------------------------------
- modifierW_Update
------------------------------------------------------------------------------*/
ULONG modifierW_Update(struct IClass *cl, Object *obj, Msg msg)
{
	int i;
	struct modifierW_Data *data = INST_DATA(cl, obj);

	for (i = 0; i < 16; i++)
		DoMethod(data->FG[i], MUIM_faderCvSeqG_Update);

	return 0;
}

/*-----------------------------------------------------------------------------
- Dispatcher
------------------------------------------------------------------------------*/
DISPATCHER(modifierW_Dispatcher)
{
	switch(msg->MethodID)
	{
		case OM_NEW:
			return modifierW_New(cl, obj, (APTR)msg);
		case MUIM_modifierW_Finish:
			return modifierW_Finish(cl, obj, (APTR)msg);
		case MUIM_modifierW_Wave:
			return modifierW_Wave(cl, obj, (APTR)msg);
		case MUIM_modifierW_Speed:
			return modifierW_Speed(cl, obj, (APTR)msg);
		case MUIM_modifierW_Offset:
			return modifierW_Offset(cl, obj, (APTR)msg);
		case MUIM_modifierW_Update:
			return modifierW_Update(cl, obj, (APTR)msg);
	}

	return DoSuperMethodA(cl, obj, msg);
}

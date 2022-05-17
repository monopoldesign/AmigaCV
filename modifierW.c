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
	waveForm[2] = "Square";
	waveForm[3] = "Saw";
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
																			Child, SliderObject,
																				MUIA_Slider_Horiz, FALSE,
																				MUIA_Slider_Quiet, TRUE,
																				MUIA_Slider_Min, 1,
																				MUIA_Slider_Max, 99,
																			End,
																			Child, Label("Freq"),
																		End,
																		Child, VGroup,
																			Child, SliderObject,
																				MUIA_Slider_Horiz, FALSE,
																				MUIA_Slider_Quiet, TRUE,
																				MUIA_Slider_Min, 1,
																				MUIA_Slider_Max, 99,
																			End,
																			Child, Label("Offset"),
																		End,
																	End,
																End,
																Child, myLFO[tmp.chn] = NewObject(CL_lfoC->mcc_Class, NULL, MUIA_lfoC_Channel, tmp.chn, TextFrame, MUIA_Background, MUII_BACKGROUND, TAG_DONE),
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
	}

	return DoSuperMethodA(cl, obj, msg);
}

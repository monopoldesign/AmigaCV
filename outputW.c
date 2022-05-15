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
#include "outputW.h"
#include "faderG.h"
#include "help.h"

/******************************************************************************
* Class-Methods
*******************************************************************************/

/*-----------------------------------------------------------------------------
- OM_NEW
------------------------------------------------------------------------------*/
ULONG outputW_New(struct IClass *cl, Object *obj, struct opSet *msg)
{
	struct outputW_Data tmp = {0};

	if (obj = (Object *)DoSuperNew(cl, obj,
		MUIA_Window_Title,			"Outputs",
		MUIA_Window_TopEdge,		32,
		MUIA_Window_LeftEdge,		194,
		MUIA_Window_Screen,			myScreen,
		MUIA_Window_SizeGadget,		FALSE,
		MUIA_Window_DepthGadget,	FALSE,
		WindowContents,				HGroup,
										Child, NewObject(CL_faderG->mcc_Class, NULL, MUIA_faderG_Dest, &AudioOut[0], MUIA_faderG_Channel, 0, TAG_DONE),
										Child, NewObject(CL_faderG->mcc_Class, NULL, MUIA_faderG_Dest, &AudioOut[1], MUIA_faderG_Channel, 1, TAG_DONE),
										Child, NewObject(CL_faderG->mcc_Class, NULL, MUIA_faderG_Dest, &AudioOut[2], MUIA_faderG_Channel, 2, TAG_DONE),
										Child, NewObject(CL_faderG->mcc_Class, NULL, MUIA_faderG_Dest, &AudioOut[3], MUIA_faderG_Channel, 3, TAG_DONE),
										Child, NewObject(CL_faderG->mcc_Class, NULL, MUIA_faderG_Dest, &AudioOut[4], MUIA_faderG_Channel, 4, TAG_DONE),
										Child, NewObject(CL_faderG->mcc_Class, NULL, MUIA_faderG_Dest, &AudioOut[5], MUIA_faderG_Channel, 5, TAG_DONE),
										Child, NewObject(CL_faderG->mcc_Class, NULL, MUIA_faderG_Dest, &AudioOut[6], MUIA_faderG_Channel, 6, TAG_DONE),
										Child, NewObject(CL_faderG->mcc_Class, NULL, MUIA_faderG_Dest, &AudioOut[7], MUIA_faderG_Channel, 7, TAG_DONE),
									End,
		TAG_MORE, msg->ops_AttrList))
	{
		struct outputW_Data *data = INST_DATA(cl, obj);
		*data = tmp;

		DoMethod(obj, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, obj, 2, MUIM_outputW_Finish, 0);

		return (ULONG)obj;
	}
	return 0;
}

/*-----------------------------------------------------------------------------
- outputW_Finish
------------------------------------------------------------------------------*/
ULONG outputW_Finish(struct IClass *cl, Object *obj, Msg msg)
{
	set(obj, MUIA_Window_Open, FALSE);

	DoMethod((Object *)xget(obj, MUIA_ApplicationObject), OM_REMMEMBER, obj);

	MUI_DisposeObject(obj);
	outputWin = NULL;

	return 0;
}

/*-----------------------------------------------------------------------------
- Dispatcher
------------------------------------------------------------------------------*/
DISPATCHER(outputW_Dispatcher)
{
	switch(msg->MethodID)
	{
		case OM_NEW:
			return outputW_New(cl, obj, (APTR)msg);
		case MUIM_outputW_Finish:
			return outputW_Finish(cl, obj, (APTR)msg);
	}

	return DoSuperMethodA(cl, obj, msg);
}

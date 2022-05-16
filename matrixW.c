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
#include "matrixW.h"
#include "matrixG.h"
#include "help.h"

/******************************************************************************
* Class-Methods
*******************************************************************************/

/*-----------------------------------------------------------------------------
- OM_NEW
------------------------------------------------------------------------------*/
ULONG matrixW_New(struct IClass *cl, Object *obj, struct opSet *msg)
{
	struct matrixW_Data tmp = {0};

	if (obj = (Object *)DoSuperNew(cl, obj,
		MUIA_Window_Title,			"CV-Matrix",
		MUIA_Window_TopEdge,		200,
		MUIA_Window_LeftEdge,		0,
		MUIA_Window_Screen,			myScreen,
		MUIA_Window_SizeGadget,		FALSE,
		MUIA_Window_DepthGadget,	FALSE,
		WindowContents,				VGroup,
										MUIA_Group_VertSpacing, 2,
										Child, NewObject(CL_matrixG->mcc_Class, NULL, MUIA_matrixG_Dest, &CVin[0], MUIA_matrixG_Channel, 0, TAG_DONE),
										Child, NewObject(CL_matrixG->mcc_Class, NULL, MUIA_matrixG_Dest, &CVin[1], MUIA_matrixG_Channel, 1, TAG_DONE),
										Child, NewObject(CL_matrixG->mcc_Class, NULL, MUIA_matrixG_Dest, &CVin[2], MUIA_matrixG_Channel, 2, TAG_DONE),
										Child, NewObject(CL_matrixG->mcc_Class, NULL, MUIA_matrixG_Dest, &CVin[3], MUIA_matrixG_Channel, 3, TAG_DONE),
										Child, NewObject(CL_matrixG->mcc_Class, NULL, MUIA_matrixG_Dest, &CVin[4], MUIA_matrixG_Channel, 4, TAG_DONE),
										Child, NewObject(CL_matrixG->mcc_Class, NULL, MUIA_matrixG_Dest, &CVin[5], MUIA_matrixG_Channel, 5, TAG_DONE),
										Child, NewObject(CL_matrixG->mcc_Class, NULL, MUIA_matrixG_Dest, &CVin[6], MUIA_matrixG_Channel, 6, TAG_DONE),
										Child, NewObject(CL_matrixG->mcc_Class, NULL, MUIA_matrixG_Dest, &CVin[7], MUIA_matrixG_Channel, 7, TAG_DONE),
										Child, HGroup,
											Child, RectangleObject,
												MUIA_FixWidth, 36,
											End,
											Child, TextObject,
												MUIA_Background, MUII_TextBack,
												MUIA_Frame, MUIV_Frame_None,
												MUIA_Text_Contents, "1",
											End,
											Child, TextObject,
												MUIA_Background, MUII_TextBack,
												MUIA_Frame, MUIV_Frame_None,
												MUIA_Text_Contents, "2",
											End,
											Child, TextObject,
												MUIA_Background, MUII_TextBack,
												MUIA_Frame, MUIV_Frame_None,
												MUIA_Text_Contents, "3",
											End,
											Child, TextObject,
												MUIA_Background, MUII_TextBack,
												MUIA_Frame, MUIV_Frame_None,
												MUIA_Text_Contents, "4",
											End,
											Child, TextObject,
												MUIA_Background, MUII_TextBack,
												MUIA_Frame, MUIV_Frame_None,
												MUIA_Text_Contents, "5",
											End,
											Child, TextObject,
												MUIA_Background, MUII_TextBack,
												MUIA_Frame, MUIV_Frame_None,
												MUIA_Text_Contents, "6",
											End,
											Child, TextObject,
												MUIA_Background, MUII_TextBack,
												MUIA_Frame, MUIV_Frame_None,
												MUIA_Text_Contents, "7",
											End,
											Child, TextObject,
												MUIA_Background, MUII_TextBack,
												MUIA_Frame, MUIV_Frame_None,
												MUIA_Text_Contents, "8",
											End,
										End,
									End,
		TAG_MORE, msg->ops_AttrList))
	{
		struct matrixW_Data *data = INST_DATA(cl, obj);
		*data = tmp;

		DoMethod(obj, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, obj, 2, MUIM_matrixW_Finish, 0);
		DoMethod(obj, MUIM_Notify, MUIA_Window_Activate, TRUE, obj, 2, MUIM_matrixW_Activate, 0);

		return (ULONG)obj;
	}
	return 0;
}

/*-----------------------------------------------------------------------------
- matrixW_Finish
------------------------------------------------------------------------------*/
ULONG matrixW_Finish(struct IClass *cl, Object *obj, Msg msg)
{
	set(obj, MUIA_Window_Open, FALSE);

	DoMethod((Object *)xget(obj, MUIA_ApplicationObject), OM_REMMEMBER, obj);

	MUI_DisposeObject(obj);
	matrixWin = NULL;

	return 0;
}

/*-----------------------------------------------------------------------------
- matrixW_Activate
------------------------------------------------------------------------------*/
ULONG matrixW_Activate(struct IClass *cl, Object *obj, Msg msg)
{
	//printf("Activate\n");
	return 0;
}

/*-----------------------------------------------------------------------------
- Dispatcher
------------------------------------------------------------------------------*/
DISPATCHER(matrixW_Dispatcher)
{
	switch(msg->MethodID)
	{
		case OM_NEW:
			return matrixW_New(cl, obj, (APTR)msg);
		case MUIM_matrixW_Finish:
			return matrixW_Finish(cl, obj, (APTR)msg);
		case MUIM_matrixW_Activate:
			return matrixW_Activate(cl, obj, (APTR)msg);
	}

	return DoSuperMethodA(cl, obj, msg);
}

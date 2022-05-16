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
#include "matrixG.h"
#include "modifierW.h"
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
ULONG matrixG_New(struct IClass *cl, Object *obj, struct opSet *msg)
{
	struct matrixG_Data tmp = {0};
	UBYTE i;
	struct TagItem *tags;

	tags = msg->ops_AttrList;
	tmp.dest = (APTR)GetTagData(MUIA_matrixG_Dest, dest, tags);
	tmp.chn = (UBYTE)GetTagData(MUIA_matrixG_Channel, 0, tags);

	for (i = 0; i < 8; i++)
	{
		tmp.cb[i] = ImageObject,
					MUIA_Frame, MUIV_Frame_ImageButton,
					MUIA_InputMode,			MUIV_InputMode_Toggle,
					MUIA_Image_Spec,		MUII_CheckMark,
					MUIA_Selected,			FALSE,
					MUIA_Background,		MUII_ButtonBack,
					MUIA_ShowSelState,		FALSE,
				End;
	}

	sprintf(buffer, "%d", tmp.chn + 1);

	if (obj = (Object *)DoSuperNew(cl, obj,
			Child, HGroup,
				MUIA_Group_HorizSpacing,	0,
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
				Child,						tmp.bt = SimpleButton("M"),
				Child,						HSpace(4),
				Child,						tmp.cb[0],
				Child,						tmp.cb[1],
				Child,						tmp.cb[2],
				Child,						tmp.cb[3],
				Child,						tmp.cb[4],
				Child,						tmp.cb[5],
				Child,						tmp.cb[6],
				Child,						tmp.cb[7],
			End));
	{
		struct matrixG_Data *data = INST_DATA(cl, obj);
		*data = tmp;

		DoMethod(data->bt, MUIM_Notify, MUIA_Pressed, FALSE, obj, 1, MUIM_matrixG_Modify);

		// Setup CheckBoxes
		for (i = 0; i < 8; i++)
		{
			// Setup UI
			if (*(data->dest) & (1<<i))
				DoMethod(data->cb[i], MUIM_Set, MUIA_Selected, TRUE);
			else
				DoMethod(data->cb[i], MUIM_Set, MUIA_Selected, FALSE);

			// Setup Method for CheckBox
			DoMethod(data->cb[i], MUIM_Notify, MUIA_Selected, MUIV_EveryTime, obj, 3, MUIM_matrixG_CheckBox, i, MUIV_TriggerValue);
		}

		return (ULONG)obj;
	}
	return 0;
}

/*-----------------------------------------------------------------------------
- matrixG_CheckBox
------------------------------------------------------------------------------*/
ULONG matrixG_CheckBox(struct IClass *cl, Object *obj, struct MUIP_matrixG_CheckBox *msg)
{
	struct matrixG_Data *data = INST_DATA(cl, obj);

	if (msg->state)
		*(data->dest) |= 1<<msg->id;
	else
		*(data->dest) &= ~(1<<msg->id);

	return 0;
}

/*-----------------------------------------------------------------------------
- matrixG_Modify
------------------------------------------------------------------------------*/
ULONG matrixG_Modify(struct IClass *cl, Object *obj, struct MUIP_matrixG_CheckBox *msg)
{
	struct matrixG_Data *data = INST_DATA(cl, obj);

	if (!modifierW[data->chn])
	{
		modifierW[data->chn] = NewObject(CL_modifierW->mcc_Class, NULL, MUIA_modifierG_Channel, data->chn, TAG_DONE);
		DoMethod((Object *)xget(obj, MUIA_ApplicationObject), OM_ADDMEMBER, modifierW[data->chn]);
		set(modifierW[data->chn], MUIA_Window_Open, TRUE);
	}

	return 0;
}

/*-----------------------------------------------------------------------------
- Dispatcher
------------------------------------------------------------------------------*/
DISPATCHER(matrixG_Dispatcher)
{
	switch(msg->MethodID)
	{
		case OM_NEW:
			return matrixG_New(cl, obj, (APTR)msg);
		case MUIM_matrixG_CheckBox:
			return matrixG_CheckBox(cl, obj, (APTR)msg);
		case MUIM_matrixG_Modify:
			return matrixG_Modify(cl, obj, (APTR)msg);
	}

	return DoSuperMethodA(cl, obj, msg);
}

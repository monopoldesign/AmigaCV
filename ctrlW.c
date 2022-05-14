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
		MUIA_Window_Title,			"Control",
		MUIA_Window_ID,				MAKE_ID('C', 'T', 'R', 'L'),
		WindowContents,				tmp.BT_New = SimpleButton("New Window"),
		MUIA_Window_Screen,			myScreen,
		TAG_MORE, msg->ops_AttrList))
	{
		struct ctrlW_Data *data = INST_DATA(cl, obj);
		*data = tmp;

		DoMethod(obj, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, obj, 2, MUIM_ctrlW_Finish, 0);

		return (ULONG)obj;
	}
	return 0;
}

ULONG ctrlW_Finish(struct IClass *cl, Object *obj, struct MUIP_ctrlW_Finish *msg)
{
	struct ctrlW_Data *data = INST_DATA(cl, obj);

	DoMethod((Object *)xget(obj, MUIA_ApplicationObject), MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);
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
	}

	return DoSuperMethodA(cl, obj, msg);
}

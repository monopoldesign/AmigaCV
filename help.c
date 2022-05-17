/******************************************************************************
* AmigaCV
* (C)2020-2022 M.Volkel (mario.volkel@outlook.com)
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
#include "ctrlW.h"
#include "inputW.h"
#include "outputW.h"
#include "matrixW.h"
#include "modifierW.h"
#include "faderG.h"
#include "matrixG.h"
#include "lfoC.h"
#include "help.h"

/******************************************************************************
* Help-Functions
*******************************************************************************/
ULONG DoSuperNew(struct IClass *cl, Object *obj, ULONG tag1, ...)
{
	return (DoSuperMethod(cl, obj, OM_NEW, &tag1, NULL));
}

LONG xget(Object *obj, ULONG attribute)
{
	LONG x;
	get(obj, attribute, &x);
	return(x);
}

/*-----------------------------------------------------------------------------
- init()
------------------------------------------------------------------------------*/
BOOL initLibs(void)
{
	IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 37);
	MUIMasterBase = OpenLibrary(MUIMASTER_NAME, MUIMASTER_VMIN);
	UtilityBase = OpenLibrary("utility.library", 39);

	if (IntuitionBase && MUIMasterBase && UtilityBase)
		return TRUE;

	exitLibs();
	return FALSE;
}

/*-----------------------------------------------------------------------------
- end()
------------------------------------------------------------------------------*/
void exitLibs(void)
{
	if (UtilityBase)
		CloseLibrary((struct Library *)UtilityBase);

	if (MUIMasterBase)
		CloseLibrary((struct Library *)MUIMasterBase);

	if (IntuitionBase)
		CloseLibrary((struct Library *)IntuitionBase);
}

/*-----------------------------------------------------------------------------
- DisposeApp()
------------------------------------------------------------------------------*/
BOOL initClasses(void)
{
	CL_ctrlW = MUI_CreateCustomClass(NULL, MUIC_Window, NULL, sizeof(struct ctrlW_Data), ctrlW_Dispatcher);
	CL_inputW = MUI_CreateCustomClass(NULL, MUIC_Window, NULL, sizeof(struct inputW_Data), inputW_Dispatcher);
	CL_outputW = MUI_CreateCustomClass(NULL, MUIC_Window, NULL, sizeof(struct outputW_Data), outputW_Dispatcher);
	CL_matrixW = MUI_CreateCustomClass(NULL, MUIC_Window, NULL, sizeof(struct matrixW_Data), matrixW_Dispatcher);
	CL_modifierW = MUI_CreateCustomClass(NULL, MUIC_Window, NULL, sizeof(struct modifierW_Data), modifierW_Dispatcher);

	CL_faderG = MUI_CreateCustomClass(NULL, MUIC_Group, NULL, sizeof(struct faderG_Data), faderG_Dispatcher);
	CL_matrixG = MUI_CreateCustomClass(NULL, MUIC_Group, NULL, sizeof(struct matrixG_Data), matrixG_Dispatcher);

	CL_lfoC = MUI_CreateCustomClass(NULL, MUIC_Area, NULL, sizeof(struct lfoC_Data), lfoC_Dispatcher);

	if (CL_ctrlW && CL_inputW && CL_outputW && CL_matrixW && CL_modifierW && CL_faderG && CL_matrixG && CL_lfoC)
		return TRUE;

	exitClasses();
	return FALSE;
}

/*-----------------------------------------------------------------------------
- DisposeApp()
------------------------------------------------------------------------------*/
void exitClasses(void)
{
	if (CL_ctrlW)
		MUI_DeleteCustomClass(CL_ctrlW);

	if (CL_inputW)
		MUI_DeleteCustomClass(CL_inputW);

	if (CL_outputW)
		MUI_DeleteCustomClass(CL_outputW);

	if (CL_matrixW)
		MUI_DeleteCustomClass(CL_matrixW);

	if (CL_modifierW)
		MUI_DeleteCustomClass(CL_modifierW);

	if (CL_faderG)
		MUI_DeleteCustomClass(CL_faderG);

	if (CL_matrixG)
		MUI_DeleteCustomClass(CL_matrixG);

	if (CL_lfoC)
		MUI_DeleteCustomClass(CL_lfoC);
}

/*-----------------------------------------------------------------------------
- DisposeApp()
------------------------------------------------------------------------------*/
BOOL SetupScreen(void)
{
	if (!(myScreen = OpenScreenTags(NULL, 
		SA_LikeWorkbench, TRUE,
		SA_Type, PUBLICSCREEN,
		SA_PubName, screenName,
		SA_Title, "AmigaCV V0.1, (C)2020-2022 M.Volkel",
		SA_ShowTitle, TRUE,
		TAG_DONE)))
	{
		return FALSE;
	}

	while (PubScreenStatus(myScreen, NULL) == 0);

	LockPubScreen(screenName);
	return TRUE;
}

/*-----------------------------------------------------------------------------
- DisposeApp()
------------------------------------------------------------------------------*/
void CloseDownScreen(void)
{
	if (myScreen)
	{
		UnlockPubScreen(NULL, myScreen);
		CloseScreen(myScreen);
		myScreen = NULL;
	}
}

/*-----------------------------------------------------------------------------
- DisposeApp()
------------------------------------------------------------------------------*/
void errorShutdown(char *errorMsg)
{
	printf(errorMsg);
	CloseDownScreen();
	exitLibs();
	exit(20);
}

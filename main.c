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
#include "help.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
struct IntuitionBase *IntuitionBase;
struct Library *MUIMasterBase;
struct Library *UtilityBase;

char buffer[40];

struct Screen *myScreen;
UBYTE *screenName = "MDISCREEN";

Object *app, *ctrlWin, *inputWin, *outputWin;

struct MUI_CustomClass *CL_ctrlW;
struct MUI_CustomClass *CL_inputW;
struct MUI_CustomClass *CL_outputW;
struct MUI_CustomClass *CL_faderG;

ULONG tempo = 120;
BOOL isPlaying = FALSE;

UBYTE AudioIn[8] = {0, 10, 20, 30, 40, 50, 60, 70};
UBYTE AudioOut[8] = {70, 60, 50, 40, 30, 20, 10, 0};

/******************************************************************************
* Main-Program
*******************************************************************************/

/*-----------------------------------------------------------------------------
- main()
------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	if (initLibs())
	{
		if (SetupScreen())
		{
			if (initClasses())
			{
				app = ApplicationObject,
					MUIA_Application_Author,			"M.Volkel",
					MUIA_Application_Base,				"MUIMDI",
					MUIA_Application_Title,				"MUI_MDIWindows",
					MUIA_Application_Version,			"$VER: MUI_MDIWindows V0.1",
					MUIA_Application_Copyright,			"(C)2022 M.Volkel",
					MUIA_Application_Description,		"MUI-MDIWindows",
					MUIA_Application_Window,			ctrlWin = NewObject(CL_ctrlW->mcc_Class, NULL, TAG_DONE),
					MUIA_Application_UseCommodities,	FALSE,
					MUIA_Application_UseRexx,			FALSE,
				End;

				if (app)
				{
					// open Main-Window
					set(ctrlWin, MUIA_Window_Open, TRUE);

					// open Input-Window
					inputWin = NewObject(CL_inputW->mcc_Class, NULL, TAG_DONE);
					DoMethod(app, OM_ADDMEMBER, inputWin);
					set(inputWin, MUIA_Window_Open, TRUE);

					// open Output-Window
					outputWin = NewObject(CL_outputW->mcc_Class, NULL, TAG_DONE);
					DoMethod(app, OM_ADDMEMBER, outputWin);
					set(outputWin, MUIA_Window_Open, TRUE);

					// Application Main-Loop
					mainLoop();

					// close Output-Window
					set(outputWin, MUIA_Window_Open, FALSE);

					// close Input-Window
					set(inputWin, MUIA_Window_Open, FALSE);

					// close Main-Window
					set(ctrlWin, MUIA_Window_Open, FALSE);

					// Dispose App
					MUI_DisposeObject(app);
				}
				exitClasses();
			}
			else
				errorShutdown("Can't create classes!\n");

			CloseDownScreen();
		}
		else
			errorShutdown("Can't open Screen!\n");

		exitLibs();
	}
	else
		errorShutdown("Can't open all required libraries!\n");

	exit(0);
}

/*-----------------------------------------------------------------------------
- mainLoop()
------------------------------------------------------------------------------*/
void mainLoop()
{
	BOOL running = TRUE;
	ULONG signal;

	while (running)
	{
		switch (DoMethod(app, MUIM_Application_NewInput, &signal))
		{
			// Window close
			case MUIV_Application_ReturnID_Quit:
				if ((MUI_RequestA(app, ctrlWin, 0, "Quit?", "_Yes|_No", "\33cAre you sure?", 0)) == 1)
					running = FALSE;
			break;

			default:
				break;
		}

		if (running && signal)
			signal = Wait(signal | SIGBREAKF_CTRL_C);

		if (signal & SIGBREAKF_CTRL_C)
			break;
	}
}

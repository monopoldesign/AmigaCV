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
#include "modifierW.h"
#include "lfoC.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
struct IntuitionBase *IntuitionBase;
struct Library *MUIMasterBase;
struct Library *UtilityBase;

char buffer[40];

struct Screen *myScreen;
UBYTE *screenName = "MDISCREEN";

Object *app, *ctrlWin, *inputWin, *outputWin, *matrixWin, *modifierW[8];

struct MUI_CustomClass *CL_ctrlW;
struct MUI_CustomClass *CL_inputW;
struct MUI_CustomClass *CL_outputW;
struct MUI_CustomClass *CL_matrixW;
struct MUI_CustomClass *CL_modifierW;
struct MUI_CustomClass *CL_faderG;
struct MUI_CustomClass *CL_matrixG;
struct MUI_CustomClass *CL_faderCvSeqG;
struct MUI_CustomClass *CL_lfoC;
struct MUI_CustomClass *CL_ledC;

#define STACK_SIZE 1000L
struct Task *dispTaskPtr = NULL;
volatile UBYTE dispTaskRunning = FALSE;
volatile UBYTE dispTaskRemove = FALSE;

UBYTE LFOWave[8] = {0, 0, 1, 1, 2, 2, 3, 3};
UBYTE LFOSpeed[8] = {8, 8, 16, 16, 32, 32, 64, 64};
BYTE LFOOffset[8] = {0, 0, 0, 0, 0, 0, 0, 0};

UBYTE CVSeq[16] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 85, 90, 95, 100, 105, 110, 115};

ULONG tempo = 120;
BOOL isPlaying = FALSE;

UBYTE AudioIn[8] = {0, 10, 20, 30, 40, 50, 60, 70};
UBYTE AudioOut[8] = {70, 60, 50, 40, 30, 20, 10, 0};
UBYTE CVin[8] = {1, 2, 4, 8, 16, 32, 64, 128};
UBYTE modType[8] = {MOD_CVSEQ, MOD_CVSEQ, MOD_CVSEQ, MOD_CVSEQ, MOD_LFO, MOD_LFO, MOD_LFO, MOD_DC};

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

					// open Matrix-Window
					matrixWin = NewObject(CL_matrixW->mcc_Class, NULL, TAG_DONE);
					DoMethod(app, OM_ADDMEMBER, matrixWin);
					set(matrixWin, MUIA_Window_Open, TRUE);

					if (addDispTask())
					{
						if (startInterrupts())
						{
							// Application Main-Loop
							mainLoop();
							endInterrupts();
						}

						if (dispTaskRunning)
							remDispTask();
					}

					// close Matrix-Window
					set(matrixWin, MUIA_Window_Open, FALSE);

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

/*-----------------------------------------------------------------------------
- addDispTask()
------------------------------------------------------------------------------*/
BOOL addDispTask()
{
	if (!dispTaskRunning)
	{
		if (dispTaskPtr = CreateTask("DispTask", 0, dispTask, STACK_SIZE))
			return TRUE;
	}

	return FALSE;
}

/*-----------------------------------------------------------------------------
- remDispTask()
------------------------------------------------------------------------------*/
void remDispTask()
{
	if (dispTaskRunning)
	{
		dispTaskRemove = TRUE;
		while (dispTaskRunning);

		dispTaskRemove = FALSE;
		Forbid();
		DeleteTask(dispTaskPtr);
		Permit();
	}
}

/*-----------------------------------------------------------------------------
- dispTask()
------------------------------------------------------------------------------*/
void dispTask()
{
	struct MsgPort *TimerMP_Task;
	struct timerequest *TimerIO_Task;
	int i;

	if (TimerMP_Task = CreatePort(0, 0))
	{
		if (TimerIO_Task = (struct timerequest *)CreateExtIO(TimerMP_Task, sizeof(struct timerequest)))
		{
			if (!(OpenDevice(TIMERNAME, UNIT_MICROHZ, (struct IORequest *)TimerIO_Task, 0)))
			{
				dispTaskRunning = TRUE;

				while (1)
				{
					for (i = 0; i < 8; i++)
					{
						if (xget(modifierW[i], MUIA_Window_Open))
							DoMethod(modifierW[i], MUIM_modifierW_Update);
					}

					TimerIO_Task->tr_node.io_Command = TR_ADDREQUEST;
					TimerIO_Task->tr_time.tv_secs = 0;
					TimerIO_Task->tr_time.tv_micro = 40 * 1000;
					DoIO((struct IORequest *)TimerIO_Task);

					if (dispTaskRemove)
						break;
				}

				CloseDevice((struct IORequest *)TimerIO_Task);
			}
			DeleteExtIO((struct IORequest *)TimerIO_Task);
		}
		DeletePort(TimerMP_Task);
	}

	dispTaskRunning = FALSE;
	Wait(0L);
}

/*-----------------------------------------------------------------------------
- Interrupt_LFO()
------------------------------------------------------------------------------*/
void Interrupt_LFO(void)
{
	struct timerequest *tr;
	UBYTE i;

	tr = (struct timerequest *)GetMsg(intData->intPort_LFO);

	if ((tr) && (intData->intFlag_LFO == INT_ON))
	{
		for (i = 0; i < 8; i++)
		{
			//LFOVal[i] = sineWave[phaseCnt[i]];

			if (intData->sampleCnt[i] >= LFOSpeed[i])
			{
				intData->sampleCnt[i] = 0;
				intData->phaseCnt[i]++;

				if (intData->phaseCnt[i] > 255)
					intData->phaseCnt[i] = 0;
			}
			else
				intData->sampleCnt[i]++;
		}

		int_tr_LFO->tr_node.io_Command = TR_ADDREQUEST;
		int_tr_LFO->tr_time.tv_secs = 0;
		int_tr_LFO->tr_time.tv_micro = 2 * 100;
		BeginIO((struct IORequest *)tr);
	}
	else
		intData->intFlag_LFO = INT_STOPPED;
}

/*-----------------------------------------------------------------------------
- Interrupt_SEQ()
------------------------------------------------------------------------------*/
void Interrupt_SEQ(void)
{
	struct timerequest *tr;
	UBYTE i;

	tr = (struct timerequest *)GetMsg(intData->intPort_SEQ);

	if ((tr) && (intData->intFlag_SEQ == INT_ON))
	{
		for (i = 0; i < 8; i++)
		{
			intData->seqPrescale[i]++;
			if (intData->seqPrescale[i] == 6)
			{
				intData->seqPrescale[i] = 0;

				intData->seqPos[i]++;
				if (intData->seqPos[i] > 15)
					intData->seqPos[i] = 0;
			}
		}

		int_tr_SEQ->tr_node.io_Command = TR_ADDREQUEST;
		int_tr_SEQ->tr_time.tv_secs = 0;
		int_tr_SEQ->tr_time.tv_micro = tempoTab[tempo - 20];
		BeginIO((struct IORequest *)tr);
	}
	else
		intData->intFlag_SEQ = INT_STOPPED;
}

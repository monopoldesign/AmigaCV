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

const ULONG tempoTab[221] = {125000,119041,113625,108666,104166,100000,96125,92583,89250,86166,83333,80625,78125,75750,73500,71416,69416,67541,65750,64083,62500,60958,59500,58125,56791,55541,54333,53166,52083,51000,50000,49000,48041,47166,46291,45416,44625,43833,43083,42333,41666,40958,40291,39666,39041,38458,37875,37291,36750,36208,35708,35208,34708,34208,33750,33333,32875,32458,32041,31625,31250,30833,30458,30083,29750,29375,29041,28708,28375,28083,27750,27458,27166,26875,26583,26291,26041,25750,25500,25250,25000,24750,24500,24250,24000,23791,23583,23333,23125,22916,22708,22500,22291,22083,21916,21708,21541,21333,21166,21000,20833,20625,20458,20291,20125,20000,19833,19666,19500,19375,19208,19083,18916,18791,18625,18500,18375,18208,18083,17958,17833,17708,17583,17458,17333,17208,17083,17000,16875,16750,16666,16541,16416,16333,16208,16125,16000,15916,15791,15708,15625,15500,15416,15333,15208,15125,15041,14958,14875,14791,14666,14583,14500,14416,14333,14250,14166,14083,14041,13958,13875,13791,13708,13625,13583,13500,13416,13333,13291,13208,13125,13083,13000,12916,12875,12791,12750,12666,12625,12541,12500,12416,12375,12291,12250,12166,12125,12041,12000,11958,11875,11833,11791,11708,11666,11625,11541,11500,11458,11375,11333,11291,11250,11208,11125,11083,11041,11000,10958,10916,10833,10791,10750,10708,10666,10625,10583,10541,10500,10458,10416};

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
struct Task *lfoTaskPtr = NULL;
struct Task *seqTaskPtr = NULL;

volatile UBYTE lfoTaskRunning = FALSE;
volatile UBYTE lfoTaskRemove = FALSE;
volatile UBYTE seqTaskRunning = FALSE;
volatile UBYTE seqTaskRemove = FALSE;

Object *myLFO[8];
volatile ULONG phaseCnt[8] = {0, 0, 0, 0, 0, 0, 0, 0};
volatile ULONG sampleCnt[8] = {0, 0, 0, 0, 0, 0, 0, 0};
volatile BYTE LFOVal[8] = {0, 0, 0, 0, 0, 0, 0, 0};

UBYTE LFOWave[8] = {0, 0, 1, 1, 2, 2, 3, 3};
UBYTE LFOSpeed[8] = {8, 8, 16, 16, 32, 32, 64, 64};
BYTE LFOOffset[8] = {0, 0, 0, 0, 0, 0, 0, 0};

UBYTE CVSeq[16] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 85, 90, 95, 100, 105, 110, 115};
UBYTE seqPos[8] = {4, 0, 0, 0, 0, 0, 0, 0};
UBYTE seqPrescale[8] = {0, 0, 0, 0, 0, 0, 0, 0};

unsigned int dispCnt = 0;

ULONG tempo = 40;
BOOL isPlaying = FALSE;

UBYTE AudioIn[8] = {0, 10, 20, 30, 40, 50, 60, 70};
UBYTE AudioOut[8] = {70, 60, 50, 40, 30, 20, 10, 0};
UBYTE CVin[8] = {1, 2, 4, 8, 16, 32, 64, 128};
UBYTE modType[8] = {MOD_LFO, MOD_CVSEQ, MOD_LFO, MOD_DC, MOD_LFO, MOD_DC, MOD_LFO, MOD_DC};

/******************************************************************************
s* Main-Program
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

					if (addLfoTask())
					{
						if (addSeqTask())
						{
							// Application Main-Loop
							mainLoop();

							if (seqTaskRunning)
								remSeqTask();
						}

						if (lfoTaskRunning)
							remLfoTask();
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
- addLfoTask()
------------------------------------------------------------------------------*/
BOOL addLfoTask()
{
	if (!lfoTaskRunning)
	{
		if (lfoTaskPtr = CreateTask("LFOTask", 0, lfoTask, STACK_SIZE))
			return TRUE;
	}

	return FALSE;
}

/*-----------------------------------------------------------------------------
- remLfoTask()
------------------------------------------------------------------------------*/
void remLfoTask()
{
	if (lfoTaskRunning)
	{
		lfoTaskRemove = TRUE;
		while (lfoTaskRunning);

		lfoTaskRemove = FALSE;
		Forbid();
		DeleteTask(lfoTaskPtr);
		Permit();
	}
}

/*-----------------------------------------------------------------------------
- lfoTask()
------------------------------------------------------------------------------*/
void lfoTask()
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
				lfoTaskRunning = TRUE;

				while (1)
				{
					for (i = 0; i < 8; i++)
					{
						//LFOVal[i] = sineWave[phaseCnt[i]];

						if (sampleCnt[i] >= LFOSpeed[i])
						{
							sampleCnt[i] = 0;
							phaseCnt[i]++;

							if (phaseCnt[i] > 255)
								phaseCnt[i] = 0;
						}
						else
							sampleCnt[i]++;
					}

					dispCnt++;
					if (dispCnt >= 200)		// 200 * 200us = 25Hz
					{
						dispCnt = 0;

						for (i = 0; i < 8; i++)
						{
							if (myLFO[i])
								DoMethod(myLFO[i], MUIM_lfoC_Update);
						}
					}

					TimerIO_Task->tr_node.io_Command = TR_ADDREQUEST;
					TimerIO_Task->tr_time.tv_secs = 0;
					TimerIO_Task->tr_time.tv_micro = 2 * 100;
					DoIO((struct IORequest *)TimerIO_Task);

					if (lfoTaskRemove)
						break;
				}

				CloseDevice((struct IORequest *)TimerIO_Task);
			}
			DeleteExtIO((struct IORequest *)TimerIO_Task);
		}
		DeletePort(TimerMP_Task);
	}

	lfoTaskRunning = FALSE;
	Wait(0L);
}

/*-----------------------------------------------------------------------------
- addSeqTask()
------------------------------------------------------------------------------*/
BOOL addSeqTask()
{
	if (!seqTaskRunning)
	{
		if (lfoTaskPtr = CreateTask("SeqTask", 0, seqTask, STACK_SIZE))
			return TRUE;
	}

	return FALSE;
}

/*-----------------------------------------------------------------------------
- remSeqTask()
------------------------------------------------------------------------------*/
void remSeqTask()
{
	if (seqTaskRunning)
	{
		seqTaskRemove = TRUE;
		while (seqTaskRunning);

		seqTaskRemove = FALSE;
		Forbid();
		DeleteTask(lfoTaskPtr);
		Permit();
	}
}

/*-----------------------------------------------------------------------------
- seqTask()
------------------------------------------------------------------------------*/
void seqTask()
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
				seqTaskRunning = TRUE;

				while (1)
				{
					for (i = 0; i < 8; i++)
					{
						seqPrescale[i]++;
						if (seqPrescale[i] == 6)
						{
							seqPrescale[i] = 0;

							seqPos[i]++;
							if (seqPos[i] > 15)
								seqPos[i] = 0;
						}
					}

					dispCnt++;
					if (dispCnt >= 100)		// 200 * 200us = 25Hz
					{
						dispCnt = 0;

						for (i = 0; i < 8; i++)
						{
							if (modifierW[i])
								DoMethod(modifierW[i], MUIM_modifierW_Update);
						}
					}

					TimerIO_Task->tr_node.io_Command = TR_ADDREQUEST;
					TimerIO_Task->tr_time.tv_secs = 0;
					TimerIO_Task->tr_time.tv_micro = tempoTab[tempo - 20];
					DoIO((struct IORequest *)TimerIO_Task);

					if (seqTaskRemove)
						break;
				}

				CloseDevice((struct IORequest *)TimerIO_Task);
			}
			DeleteExtIO((struct IORequest *)TimerIO_Task);
		}
		DeletePort(TimerMP_Task);
	}

	seqTaskRunning = FALSE;
	Wait(0L);
}

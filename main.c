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
struct Task *dispTaskPtr = NULL;

volatile UBYTE lfoTaskRunning = FALSE;
volatile UBYTE lfoTaskRemove = FALSE;
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
UBYTE modType[8] = {MOD_LFO, MOD_CVSEQ, MOD_LFO, MOD_DC, MOD_LFO, MOD_DC, MOD_LFO, MOD_DC};

#define OFF	0
#define ON	1
#define	STOPPED 2

struct TSIData *tsidata;

struct MsgPort *intPort;
struct Interrupt *softInt;
struct timerequest *int_tr;

struct TSIData2 *tsidata2;

struct MsgPort *intPort2;
struct Interrupt *softInt2;
struct timerequest *int_tr2;

/******************************************************************************
s* Main-Program
*******************************************************************************/

/*-----------------------------------------------------------------------------
- main()
------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	/*
	if (addInterrupt())
	{
		printf("starting softint. CTRL-C to break...\n");

		while(1)
		{
			Wait(SIGBREAKF_CTRL_C);
			break;
		}

		endInterrupt();
	}
	exit(0);
	*/

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
						if (addInterrupt())
						{
							if (addInterrupt2())
							{
								// Application Main-Loop
								mainLoop();
								endInterrupt2();
							}
							endInterrupt();
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

BOOL addInterrupt(void)
{
	if (tsidata = AllocVec(sizeof(struct TSIData), MEMF_PUBLIC|MEMF_CLEAR))
	{
		if (intPort = AllocVec(sizeof(struct MsgPort), MEMF_PUBLIC|MEMF_CLEAR))
		{
			NewList(&(intPort->mp_MsgList));

			if (softInt = AllocVec(sizeof(struct Interrupt), MEMF_PUBLIC|MEMF_CLEAR))
			{
				softInt->is_Code = (APTR)tsoftcode;
				softInt->is_Data = tsidata;
				softInt->is_Node.ln_Pri = 0;

				intPort->mp_Node.ln_Type = NT_MSGPORT;
				intPort->mp_Flags = PA_SOFTINT;
				intPort->mp_SigTask = (struct Task *)softInt;

				if (int_tr = (struct timerequest *)CreateExtIO(intPort, sizeof(struct timerequest)))
				{
					if (!(OpenDevice(TIMERNAME, UNIT_MICROHZ, (struct IORequest *)int_tr, 0)))
					{
						tsidata->tsi_Flag = ON;
						tsidata->tsi_Port = intPort;
						tsidata->tsi_Counter = 0;

						int_tr->tr_node.io_Command = TR_ADDREQUEST;
						int_tr->tr_time.tv_secs = 0;
						int_tr->tr_time.tv_micro = tempoTab[tempo - 20];
						BeginIO((struct IORequest *)int_tr);

						return TRUE;
					}
					DeleteExtIO((struct IORequest *)int_tr);
				}
				FreeVec(softInt);
			}
			FreeVec(intPort);
		}
		FreeVec(tsidata);
	}
	return FALSE;
}

void endInterrupt(void)
{
	tsidata->tsi_Flag = OFF;
	while (tsidata->tsi_Flag != STOPPED);

	if (int_tr)
		CloseDevice((struct IORequest *)int_tr);

	if (int_tr)
		DeleteExtIO((struct IORequest *)int_tr);

	if (softInt)
		FreeVec(softInt);

	if (intPort)
		FreeVec(intPort);

	if (tsidata)
		FreeVec(tsidata);
}

void tsoftcode(void)
{
	struct timerequest *tr;
	UBYTE i;

	tr = (struct timerequest *)GetMsg(tsidata->tsi_Port);

	if ((tr) && (tsidata->tsi_Flag == ON))
	{
		tsidata->tsi_Counter++;

		for (i = 0; i < 8; i++)
		{
			tsidata->seqPrescale[i]++;
			if (tsidata->seqPrescale[i] == 6)
			{
				tsidata->seqPrescale[i] = 0;

				tsidata->seqPos[i]++;
				if (tsidata->seqPos[i] > 15)
					tsidata->seqPos[i] = 0;
			}
		}

		int_tr->tr_node.io_Command = TR_ADDREQUEST;
		int_tr->tr_time.tv_secs = 0;
		int_tr->tr_time.tv_micro = tempoTab[tempo - 20];
		BeginIO((struct IORequest *)tr);
	}
	else
		tsidata->tsi_Flag = STOPPED;
}

BOOL addInterrupt2(void)
{
	if (tsidata2 = AllocVec(sizeof(struct TSIData2), MEMF_PUBLIC|MEMF_CLEAR))
	{
		if (intPort2 = AllocVec(sizeof(struct MsgPort), MEMF_PUBLIC|MEMF_CLEAR))
		{
			NewList(&(intPort2->mp_MsgList));

			if (softInt2 = AllocVec(sizeof(struct Interrupt), MEMF_PUBLIC|MEMF_CLEAR))
			{
				softInt2->is_Code = (APTR)tsoftcode2;
				softInt2->is_Data = tsidata2;
				softInt2->is_Node.ln_Pri = 0;

				intPort2->mp_Node.ln_Type = NT_MSGPORT;
				intPort2->mp_Flags = PA_SOFTINT;
				intPort2->mp_SigTask = (struct Task *)softInt2;

				if (int_tr2 = (struct timerequest *)CreateExtIO(intPort2, sizeof(struct timerequest)))
				{
					if (!(OpenDevice(TIMERNAME, UNIT_MICROHZ, (struct IORequest *)int_tr2, 0)))
					{
						tsidata2->tsi_Flag = ON;
						tsidata2->tsi_Port = intPort2;
						tsidata2->tsi_Counter = 0;

						int_tr2->tr_node.io_Command = TR_ADDREQUEST;
						int_tr2->tr_time.tv_secs = 0;
						int_tr2->tr_time.tv_micro = 2 * 100;
						BeginIO((struct IORequest *)int_tr2);

						return TRUE;
					}
					DeleteExtIO((struct IORequest *)int_tr2);
				}
				FreeVec(softInt2);
			}
			FreeVec(intPort2);
		}
		FreeVec(tsidata2);
	}
	return FALSE;
}

void endInterrupt2(void)
{
	tsidata2->tsi_Flag = OFF;
	while (tsidata2->tsi_Flag != STOPPED);

	if (int_tr2)
		CloseDevice((struct IORequest *)int_tr2);

	if (int_tr2)
		DeleteExtIO((struct IORequest *)int_tr2);

	if (softInt2)
		FreeVec(softInt2);

	if (intPort2)
		FreeVec(intPort2);

	if (tsidata2)
		FreeVec(tsidata2);
}

void tsoftcode2(void)
{
	struct timerequest *tr;
	UBYTE i;

	tr = (struct timerequest *)GetMsg(tsidata2->tsi_Port);

	if ((tr) && (tsidata2->tsi_Flag == ON))
	{
		tsidata2->tsi_Counter++;

		for (i = 0; i < 8; i++)
		{
			//LFOVal[i] = sineWave[phaseCnt[i]];

			if (tsidata2->sampleCnt[i] >= LFOSpeed[i])
			{
				tsidata2->sampleCnt[i] = 0;
				tsidata2->phaseCnt[i]++;

				if (tsidata2->phaseCnt[i] > 255)
					tsidata2->phaseCnt[i] = 0;
			}
			else
				tsidata2->sampleCnt[i]++;
		}

		int_tr2->tr_node.io_Command = TR_ADDREQUEST;
		int_tr2->tr_time.tv_secs = 0;
		int_tr2->tr_time.tv_micro = 2 * 100;
		BeginIO((struct IORequest *)tr);
	}
	else
		tsidata2->tsi_Flag = STOPPED;
}

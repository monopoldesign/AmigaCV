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
#include "faderCvSeqG.h"
#include "lfoC.h"
#include "ledC.h"
#include "help.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
const ULONG tempoTab[221] = {125000,119041,113625,108666,104166,100000,96125,92583,89250,86166,83333,80625,78125,75750,73500,71416,69416,67541,65750,64083,62500,60958,59500,58125,56791,55541,54333,53166,52083,51000,50000,49000,48041,47166,46291,45416,44625,43833,43083,42333,41666,40958,40291,39666,39041,38458,37875,37291,36750,36208,35708,35208,34708,34208,33750,33333,32875,32458,32041,31625,31250,30833,30458,30083,29750,29375,29041,28708,28375,28083,27750,27458,27166,26875,26583,26291,26041,25750,25500,25250,25000,24750,24500,24250,24000,23791,23583,23333,23125,22916,22708,22500,22291,22083,21916,21708,21541,21333,21166,21000,20833,20625,20458,20291,20125,20000,19833,19666,19500,19375,19208,19083,18916,18791,18625,18500,18375,18208,18083,17958,17833,17708,17583,17458,17333,17208,17083,17000,16875,16750,16666,16541,16416,16333,16208,16125,16000,15916,15791,15708,15625,15500,15416,15333,15208,15125,15041,14958,14875,14791,14666,14583,14500,14416,14333,14250,14166,14083,14041,13958,13875,13791,13708,13625,13583,13500,13416,13333,13291,13208,13125,13083,13000,12916,12875,12791,12750,12666,12625,12541,12500,12416,12375,12291,12250,12166,12125,12041,12000,11958,11875,11833,11791,11708,11666,11625,11541,11500,11458,11375,11333,11291,11250,11208,11125,11083,11041,11000,10958,10916,10833,10791,10750,10708,10666,10625,10583,10541,10500,10458,10416};

struct InterruptData *intData;

struct MsgPort *intPort_LFO;
struct Interrupt *softInt_LFO;
struct timerequest *int_tr_LFO;

struct MsgPort *intPort_SEQ;
struct Interrupt *softInt_SEQ;
struct timerequest *int_tr_SEQ;

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
- exitLibs()
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
- initClasses()
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
	CL_faderCvSeqG = MUI_CreateCustomClass(NULL, MUIC_Group, NULL, sizeof(struct faderCvSeqG_Data), faderCvSeqG_Dispatcher);

	CL_lfoC = MUI_CreateCustomClass(NULL, MUIC_Area, NULL, sizeof(struct lfoC_Data), lfoC_Dispatcher);
	CL_ledC = MUI_CreateCustomClass(NULL, MUIC_Area, NULL, sizeof(struct ledC_Data), ledC_Dispatcher);

	if (CL_ctrlW && CL_inputW && CL_outputW && CL_matrixW && CL_modifierW && CL_faderG && CL_matrixG && CL_faderCvSeqG && CL_lfoC && CL_ledC)
		return TRUE;

	exitClasses();
	return FALSE;
}

/*-----------------------------------------------------------------------------
- exitClasses()
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

	if (CL_faderCvSeqG)
		MUI_DeleteCustomClass(CL_faderCvSeqG);

	if (CL_lfoC)
		MUI_DeleteCustomClass(CL_lfoC);

	if (CL_ledC)
		MUI_DeleteCustomClass(CL_ledC);
}

/*-----------------------------------------------------------------------------
- SetupScreen()
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
- CloseDownScreen()
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
- errorShutdown()
------------------------------------------------------------------------------*/
void errorShutdown(char *errorMsg)
{
	printf(errorMsg);
	CloseDownScreen();
	exitLibs();
	exit(20);
}

/*-----------------------------------------------------------------------------
- startInterrupts()
------------------------------------------------------------------------------*/
BOOL startInterrupts(void)
{
	if (intData = AllocVec(sizeof(struct InterruptData), MEMF_PUBLIC|MEMF_CLEAR))
	{
		if (addInterrupt_LFO())
		{
			if (addInterrupt_SEQ())
			{
				return TRUE;
			}
		}
		FreeVec(intData);
	}
	return FALSE;
}

/*-----------------------------------------------------------------------------
- endInterrupts()
------------------------------------------------------------------------------*/
void endInterrupts(void)
{
	endInterrupt_SEQ();
	endInterrupt_LFO();

	if (intData)
		FreeVec(intData);
}

/*-----------------------------------------------------------------------------
- addInterrupt_LFO()
------------------------------------------------------------------------------*/
BOOL addInterrupt_LFO(void)
{
	if (intPort_LFO = AllocVec(sizeof(struct MsgPort), MEMF_PUBLIC|MEMF_CLEAR))
	{
		NewList(&(intPort_LFO->mp_MsgList));

		if (softInt_LFO = AllocVec(sizeof(struct Interrupt), MEMF_PUBLIC|MEMF_CLEAR))
		{
			softInt_LFO->is_Code = (APTR)Interrupt_LFO;
			softInt_LFO->is_Data = intData;
			softInt_LFO->is_Node.ln_Pri = 0;

			intPort_LFO->mp_Node.ln_Type = NT_MSGPORT;
			intPort_LFO->mp_Flags = PA_SOFTINT;
			intPort_LFO->mp_SigTask = (struct Task *)softInt_LFO;

			if (int_tr_LFO = (struct timerequest *)CreateExtIO(intPort_LFO, sizeof(struct timerequest)))
			{
				if (!(OpenDevice(TIMERNAME, UNIT_MICROHZ, (struct IORequest *)int_tr_LFO, 0)))
				{
					intData->intFlag_LFO = INT_ON;
					intData->intPort_LFO = intPort_LFO;

					int_tr_LFO->tr_node.io_Command = TR_ADDREQUEST;
					int_tr_LFO->tr_time.tv_secs = 0;
					int_tr_LFO->tr_time.tv_micro = 2 * 100;
					BeginIO((struct IORequest *)int_tr_LFO);

					return TRUE;
				}
				DeleteExtIO((struct IORequest *)int_tr_LFO);
			}
			FreeVec(softInt_LFO);
		}
		FreeVec(intPort_LFO);
	}
	return FALSE;
}

/*-----------------------------------------------------------------------------
- endInterrupt_LFO()
------------------------------------------------------------------------------*/
void endInterrupt_LFO(void)
{
	intData->intFlag_LFO = INT_OFF;

	while (intData->intFlag_LFO != INT_STOPPED);

	if (int_tr_LFO)
		CloseDevice((struct IORequest *)int_tr_LFO);

	if (int_tr_LFO)
		DeleteExtIO((struct IORequest *)int_tr_LFO);

	if (softInt_LFO)
		FreeVec(softInt_LFO);

	if (intPort_LFO)
		FreeVec(intPort_LFO);
}

/*-----------------------------------------------------------------------------
- addInterrupt_SEQ()
------------------------------------------------------------------------------*/
BOOL addInterrupt_SEQ(void)
{
	if (intPort_SEQ = AllocVec(sizeof(struct MsgPort), MEMF_PUBLIC|MEMF_CLEAR))
	{
		NewList(&(intPort_SEQ->mp_MsgList));

		if (softInt_SEQ = AllocVec(sizeof(struct Interrupt), MEMF_PUBLIC|MEMF_CLEAR))
		{
			softInt_SEQ->is_Code = (APTR)Interrupt_SEQ;
			softInt_SEQ->is_Data = intData;
			softInt_SEQ->is_Node.ln_Pri = 0;

			intPort_SEQ->mp_Node.ln_Type = NT_MSGPORT;
			intPort_SEQ->mp_Flags = PA_SOFTINT;
			intPort_SEQ->mp_SigTask = (struct Task *)softInt_SEQ;

			if (int_tr_SEQ = (struct timerequest *)CreateExtIO(intPort_SEQ, sizeof(struct timerequest)))
			{
				if (!(OpenDevice(TIMERNAME, UNIT_MICROHZ, (struct IORequest *)int_tr_SEQ, 0)))
				{
					intData->intFlag_SEQ = INT_ON;
					intData->intPort_SEQ = intPort_SEQ;

					int_tr_SEQ->tr_node.io_Command = TR_ADDREQUEST;
					int_tr_SEQ->tr_time.tv_secs = 0;
					int_tr_SEQ->tr_time.tv_micro = tempoTab[tempo - 20];
					BeginIO((struct IORequest *)int_tr_SEQ);

					return TRUE;
				}
				DeleteExtIO((struct IORequest *)int_tr_SEQ);
			}
			FreeVec(softInt_SEQ);
		}
		FreeVec(intPort_SEQ);
	}
	return FALSE;
}

/*-----------------------------------------------------------------------------
- endInterrupt_SEQ()
------------------------------------------------------------------------------*/
void endInterrupt_SEQ(void)
{
	intData->intFlag_SEQ = INT_OFF;

	while (intData->intFlag_SEQ != INT_STOPPED);

	if (int_tr_SEQ)
		CloseDevice((struct IORequest *)int_tr_SEQ);

	if (int_tr_SEQ)
		DeleteExtIO((struct IORequest *)int_tr_SEQ);

	if (softInt_SEQ)
		FreeVec(softInt_SEQ);

	if (intPort_SEQ)
		FreeVec(intPort_SEQ);
}

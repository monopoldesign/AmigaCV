#ifndef _MAIN_H
#define _MAIN_H

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <exec/memory.h>
#include <libraries/mui.h>
#include <proto/exec.h>

#include <pragma/gadtools_lib.h>
#include <pragma/graphics_lib.h>
#include <pragma/muimaster_lib.h>
#include <pragma/utility_lib.h>

/******************************************************************************
* Macros
*******************************************************************************/
#define HOOKPROTONH(name, ret, obj, param) ret name(register __a2 obj, register __a1 param)
#define MakeHook(hookName, hookFunc) struct Hook hookName = {{NULL, NULL}, (HOOKFUNC)hookFunc, NULL, NULL}
#define DISPATCHER(name) LONG name(register __a0 Class *cl, register __a2 Object *obj, register __a1 Msg msg)

/******************************************************************************
* Prototypes
*******************************************************************************/
int main(int argc, char *argv[]);
void mainLoop();

BOOL addLfoTask();
void remLfoTask();
void lfoTask();

/******************************************************************************
* Definitions
*******************************************************************************/
#define MAKE_ID(a, b, c, d) ((ULONG)(a) << 24 | (ULONG)(b) << 16 | (ULONG)(c) << 8 | (ULONG)(d))

enum MODTYPE {MOD_LFO, MOD_DC};

/******************************************************************************
* Global Variables
*******************************************************************************/
extern struct IntuitionBase *IntuitionBase;
extern struct Library *MUIMasterBase;
extern struct Library *UtilityBase;

extern char buffer[40];

extern struct Screen *myScreen;
extern UBYTE *screenName;

extern Object *app, *ctrlWin, *inputWin, *outputWin, *matrixWin, *modifierW[8];

extern struct MUI_CustomClass *CL_ctrlW;
extern struct MUI_CustomClass *CL_inputW;
extern struct MUI_CustomClass *CL_outputW;
extern struct MUI_CustomClass *CL_matrixW;
extern struct MUI_CustomClass *CL_modifierW;
extern struct MUI_CustomClass *CL_faderG;
extern struct MUI_CustomClass *CL_matrixG;
extern struct MUI_CustomClass *CL_lfoC;

extern UBYTE lfoPos[8];
extern Object *myLFO[8];

extern ULONG tempo;
extern BOOL isPlaying;

extern UBYTE AudioIn[8];
extern UBYTE AudioOut[8];
extern UBYTE CVin[8];
extern UBYTE modType[8];

#endif

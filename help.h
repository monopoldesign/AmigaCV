#ifndef _HELP_H
#define _HELP_H

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

/******************************************************************************
* Prototypes
*******************************************************************************/
ULONG DoSuperNew(struct IClass *cl, Object *obj, ULONG tag1, ...);
LONG xget(Object *obj, ULONG attribute);

BOOL initLibs(void);
void exitLibs(void);
BOOL initClasses(void);
void exitClasses(void);
BOOL SetupScreen(void);
void CloseDownScreen(void);
void errorShutdown(char *errorMsg);

BOOL startInterrupts(void);
void endInterrupts(void);
BOOL addInterrupt_LFO(void);
void endInterrupt_LFO(void);
BOOL addInterrupt_SEQ(void);
void endInterrupt_SEQ(void);

/******************************************************************************
* Definitions
*******************************************************************************/
enum INT {INT_OFF, INT_ON, INT_STOPPED};

/******************************************************************************
* Global Variables
*******************************************************************************/
extern const ULONG tempoTab[221];

extern struct timerequest *int_tr_LFO;
extern struct timerequest *int_tr_SEQ;

#endif

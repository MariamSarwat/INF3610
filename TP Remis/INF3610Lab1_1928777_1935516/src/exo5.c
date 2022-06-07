/*
*********************************************************************************************************
*                                                 uC/OS-III
*                                          The Real-Time Kernel
*                                               PORT Windows
*
*												Guy Bois
*                                  Ecole Polytechnique de Montreal, Qc, CANADA
*                                                  01/2020
*
* File : exo5.c
*********************************************************************************************************
*/
#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include <stdbool.h>
#include  "os_app_hooks.h"
#include  "app_cfg.h"

/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/
#define         TASK_STK_SIZE    8192				// Size of each task's stacks (# of WORDs)

#define         T1_PRIO    			51				// Application tasks priorities
#define         T1_Timer_PRIO    	50				// Schedulers

#define			WAITFOR10TICKS	10
#define 		WAITFOR20TICKS  20
/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/

CPU_STK          T1Stk[TASK_STK_SIZE];     			// Startup task stack
CPU_STK          T1_Timer_Stk[TASK_STK_SIZE];     	// Startup task stack

OS_TCB			 T1TCB;
OS_TCB			 T1_Timer_TCB;

OS_SEM			 SemStart1;
OS_SEM			 SemStop1;

OS_MUTEX working_mutex;
bool isWorking;

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void            T1(void* data);						// Function prototypes of tasks
void            T1_Timer(void* data);
void    errMsg(OS_ERR err, char* errMSg);

/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

int main(void) {
	OS_ERR  err;
	CPU_IntInit();

	Mem_Init();                                                 // Initialize Memory Managment Module                   
	CPU_IntDis();                                               // Disable all Interrupts                               
	CPU_Init();                                                 // Initialize the uC/CPU services                       

	OSInit(&err);                                               // Initialize uC/OS-III

	// Synchro � innitialiser
	OSSemCreate(&SemStart1, "Sem_Start", 0, &err);
	errMsg(err, "Error while creating SemStart1.");

	OSSemCreate(&SemStop1, "Sem_Stop", 0, &err);
	errMsg(err, "Error while creating SemStop1.");

	OSMutexCreate(&working_mutex, "boolean working", &err);
	errMsg(err, "Error while creating working_mutex.");

	OSTaskCreate(&T1TCB, "TaskPeriodic 1", T1, (void*)0, T1_PRIO, &T1Stk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE, 
		20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task TaskPeriodic1.");

	OSTaskCreate(&T1_Timer_TCB, "Task_Timer 1", T1_Timer, (void*)0, T1_Timer_PRIO, &T1_Timer_Stk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE, 
		20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task Task_Timer 1.");

	OSStart(&err);
	return;
}

void  T1(void* data) {
	OS_ERR  err;
	CPU_TS ts;
	OS_TICK actualticks = 0;

	while (1) {
		printf("T1 attend : %d ticks\n", OSTickCtr);

		//Synchro � compl�ter
		OSSemPend(&SemStart1, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		errMsg(err, "Error while pending SemStart1.");

		//On travaille (donc isWorking mis a true)
		OSMutexPend(&working_mutex, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		isWorking = true;
		OSMutexPost(&working_mutex, OS_OPT_POST_NONE, &err);

		actualticks = OSTimeGet(&err);

		printf("T1 commence : %d ticks\n", OSTickCtr);
		while (OSTimeGet(&err) < WAITFOR10TICKS + actualticks); // On �mule une t�che ex�cutant 5 ticks
		printf("T1 fini : %d ticks\n", OSTickCtr);

		//On ne travaille plus (donc isWorking mis a false)
		OSMutexPend(&working_mutex, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		isWorking = false;
		OSMutexPost(&working_mutex, OS_OPT_POST_NONE, &err);
		
		//Synchro � compl�ter
		OSSemPost(&SemStop1, OS_OPT_POST_1, &err);
		errMsg(err, "Error while posting SemStop1.");
	}
}

void  T1_Timer(void* data) {
	OS_ERR  err;
	CPU_TS ts;

	while (1) {
		printf("T1 Timer attend : %d ticks\n", OSTickCtr);

		// Synchro � compl�ter
		OSSemPost(&SemStart1, OS_OPT_POST_1, &err);
		errMsg(err, "Error while posting SemStart1.");

		OSTimeDly(WAITFOR20TICKS, OS_OPT_TIME_DLY, &err);

		// vérification si manque le deadline 
		OSMutexPend(&working_mutex, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		if (isWorking) printf("DEADLINE MISSED!!\n");
		OSMutexPost(&working_mutex, OS_OPT_POST_NONE, &err);
		
		// Synchro � compl�ter
		OSSemPend(&SemStop1, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		errMsg(err, "Error while pending SemStop1.");
	}
}

void errMsg(OS_ERR err, char* errMsg) {
	if (err != OS_ERR_NONE) {
		printf(errMsg);
		exit(1);
	}
}
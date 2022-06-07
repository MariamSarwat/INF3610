/*
*********************************************************************************************************
*                                                 uC/OS-III
*                                          The Real-Time Kernel
*                                               PORT Windows
*
*
*												Guy Bois
*                                  Ecole Polytechnique de Montreal, Qc, CANADA
*                                                  1/28/2020
*
* File : exo1.c
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>

#include  "os_app_hooks.h"
#include  "app_cfg.h"


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define TASK_STK_SIZE       16384            // Size of each task's stacks (# of WORDs)

#define TASK1_PRIO          6                // Defining Priority of each task
#define TASK2_PRIO          5
#define TASK3_PRIO          7
#define TASK4_PRIO          8
#define TASK5_PRIO          9

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  OS_TCB   task1TCB;
static  OS_TCB   task2TCB;
static  OS_TCB   task3TCB;
static  OS_TCB   task4TCB;
static  OS_TCB   task5TCB;

static  CPU_STK  Task1Stk[TASK_STK_SIZE];   //Stack of each task
static  CPU_STK  Task2Stk[TASK_STK_SIZE];
static  CPU_STK  Task3Stk[TASK_STK_SIZE];
static  CPU_STK  Task4Stk[TASK_STK_SIZE];
static  CPU_STK  Task5Stk[TASK_STK_SIZE];
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  Task1(void* data);
static  void  Task2(void* data);
static  void  Task3(void* data);
static  void  Task4(void* data);
static  void  Task5(void* data);
void    errMsg(OS_ERR err, char* errMSg);


int  main(void) {
	OS_ERR  os_err;

	CPU_IntInit();

	Mem_Init();                                                 // Initialize Memory Managment Module                   
	CPU_IntDis();                                               // Disable all Interrupts                               
	CPU_Init();                                                 // Initialize the uC/CPU services                       

	OSInit(&os_err);                                            // Initialize uC/OS-III                                 
	if (os_err != OS_ERR_NONE) {
		while (1);
	}

	App_OS_SetAllHooks();                                       // Set all applications hooks                      


	// GENERATE TASK

	/* TODO : generate task */

	OSTaskCreate(&task1TCB, "Task1", Task1, NULL, TASK1_PRIO, &Task1Stk[0], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &os_err);
	errMsg(os_err, "Error while creating Task1.");

	OSTaskCreate(&task2TCB, "Task2", Task2, NULL, TASK2_PRIO, &Task2Stk[0], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &os_err);
	errMsg(os_err, "Error while creating Task2.");

	OSTaskCreate(&task3TCB, "Task3", Task3, NULL, TASK3_PRIO, &Task3Stk[0], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &os_err);
	errMsg(os_err, "Error while creating Task3.");

	OSTaskCreate(&task4TCB, "Task4", Task4, NULL, TASK4_PRIO, &Task4Stk[0], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &os_err);
	errMsg(os_err, "Error while creating Task4.");

	OSTaskCreate(&task5TCB, "Task5", Task5, NULL, TASK5_PRIO, &Task5Stk[0], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &os_err);
	errMsg(os_err, "Error while creating Task5.");

	/* TODO : Start */
	OSStart(&os_err);

	return -6;                                          // Should Never Get Here.                   
}

void Task1(void* data) {
	OS_ERR err;

	while (DEF_ON) {

		printf("are an \n");
		OSTimeDly(10000, OS_OPT_TIME_DLY, &err);

		if (err != OS_ERR_NONE)
			printf("error during the task 1");
	}
}

void Task2(void* data) {
	OS_ERR err;

	while (DEF_ON) {
		printf("Task priorities \n");
		OSTimeDly(10000, OS_OPT_TIME_DLY, &err);

		if (err != OS_ERR_NONE)
			printf("error during the task 1");
	}
}

void Task3(void* data) {
	OS_ERR err;

	while (DEF_ON) {
		printf("important \n");
		OSTimeDly(10000, OS_OPT_TIME_DLY, &err);

		if (err != OS_ERR_NONE)
			printf("error during the task 1");
	}
}

void Task4(void* data) {
	OS_ERR err;

	while (DEF_ON) {
		printf("feature \n");
		OSTimeDly(10000, OS_OPT_TIME_DLY, &err);

		if (err != OS_ERR_NONE)
			printf("error during the task 1");
	}
}

void Task5(void* data) {
	OS_ERR err;

	while (DEF_ON) {
		printf("of MicroC-II ! \n");
		OSTimeDly(10000, OS_OPT_TIME_DLY, &err);

		if (err != OS_ERR_NONE)
			printf("error during the task 1");
	}
}

void errMsg(OS_ERR err, char* errMsg) {
	if (err != OS_ERR_NONE) {
		printf(errMsg);
		exit(1);
	}
}
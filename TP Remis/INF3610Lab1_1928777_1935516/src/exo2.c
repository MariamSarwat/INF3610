/*
*********************************************************************************************************
*                                                 uC/OS-III
*                                          The Real-Time Kernel
*                                               PORT Windows
*
*
*												Guy Bois
*                                  Ecole Polytechnique de Montreal, Qc, CANADA
*                                                  28/01/2019
*
* File : exo2.c
*
*********************************************************************************************************
*/

// Main include of �C-III
#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>

#include  "os_app_hooks.h"
#include  "app_cfg.h"
/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define TASK_STK_SIZE       16384            // Size of each task's stacks (# of WORDs)

#define ROBOT_A_PRIO        8             // Defining Priority of each task
#define ROBOT_B_PRIO        9
#define CONTROLLER_PRIO     7

/*
*********************************************************************************************************
*                                             VARIABLES
*********************************************************************************************************
*/

CPU_STK           robotAStk[TASK_STK_SIZE];  //Stack of each task
CPU_STK           robotBStk[TASK_STK_SIZE];
CPU_STK           controllerStk[TASK_STK_SIZE];

OS_TCB robotATCB;
OS_TCB robotBTCB;
OS_TCB controllerTCB;

/*
*********************************************************************************************************
*                                           SHARED  VARIABLES
*********************************************************************************************************
*/
OS_SEM sem_controller_to_robot_A;
OS_SEM sem_robot_A_to_controller;
OS_SEM sem_robot_A_to_robot_B;
OS_SEM sem_robot_B_to_robot_A;
OS_MUTEX total_item_cout_mutex;

volatile int total_item_count = 0;

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void    robotA(void* data);
void    robotB(void* data);
void    controller(void* data);
void    errMsg(OS_ERR err, char* errMSg);
int     readCurrentTotalCount();
void    writeCurrentTotalCount(int qty);

/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

void main(void){
	OS_ERR err;

	CPU_IntInit();

	Mem_Init();                                                 // Initialize Memory Managment Module                   
	CPU_IntDis();                                               // Disable all Interrupts                               
	CPU_Init();                                                 // Initialize the uC/CPU services                       

	OSInit(&err);                                            // Initialize uC/OS-III                                 
	if (err != OS_ERR_NONE) {
		while (1);
	}

	App_OS_SetAllHooks();                                       // Set all applications hooks                      

	/* TODO : Creer Semaphore */
	OSSemCreate(&sem_controller_to_robot_A, "controller_to_A", 0, &err);
	errMsg(err, "Error while creating semaphore controller_to_A.");

	OSSemCreate(&sem_robot_A_to_robot_B, "A_to_B", 0, &err);
	errMsg(err, "Error while creating semaphore A_B.");

	OSSemCreate(&sem_robot_B_to_robot_A, "B_to_A", 0, &err);
	errMsg(err, "Error while creating semaphore B_A.");

	OSSemCreate(&sem_robot_A_to_controller, "A_to_controller", 0, &err);
	errMsg(err, "Error while creating semaphore A_to_controller.");


	/* TODO : Creer Mutex */
	OSMutexCreate(&total_item_count, "total_count", &err);
	errMsg(err, "Error while creating mutex.");

	/* TODO : Creer tache */
	OSTaskCreate(&robotATCB, "robot_A", robotA, (void*)0, ROBOT_A_PRIO, &robotAStk, TASK_STK_SIZE / 2, TASK_STK_SIZE, 
		20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating task robot_A.");

	OSTaskCreate(&robotBTCB, "robot_B", robotB, (void*)0, ROBOT_B_PRIO, &robotBStk, TASK_STK_SIZE / 2, TASK_STK_SIZE, 
		20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating task robot_B.");

	OSTaskCreate(&controllerTCB, "controller", controller, (void*)0, CONTROLLER_PRIO, &controllerStk, TASK_STK_SIZE / 2, TASK_STK_SIZE, 
		20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating task controller.");

	
	/* TODO : Start */
	if (err == OS_ERR_NONE) {									// Exemple de traitement d'erreurs
		printf("End of task creation ! \n");
		OSStart(&err);                                          //Start multitasking (i.e. give control to uC/OS-III)
	} else {
		printf("Error while creating tasks !");
	}

	while (DEF_ON);

	return;
}

/*
*********************************************************************************************************
*                                            TASK FUNCTIONS
*********************************************************************************************************
*/

void robotA(void* data) {
	OS_ERR err;
	CPU_TS ts;
	OS_TICK actualticks = 0;

	int startTime = 0;
	int orderNumber = 1;
	int itemCount;
	printf("ROBOT A @ %d : DEBUT.\n", OSTimeGet(&err) - startTime);
	while (DEF_ON) {
		itemCount = (rand() % 7 + 1);

		/* TODO : Gerer la synchronisation avec les semaphores*/
		OSSemPend(&sem_controller_to_robot_A, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		errMsg(err, "Error while pending sem_controller_to_robot_A.");

		/* TODO : Proteger ce bloc avec un mutex*/
		OSMutexPend(&total_item_count, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		int current = readCurrentTotalCount();
		writeCurrentTotalCount(current + itemCount);
		OSMutexPost(&total_item_count, OS_OPT_POST_NONE, &err);

		/* TODO : Faire l'attente active */
		while (itemCount + actualticks++ < OSTimeGet(&err));
		printf("ROBOT A COMMANDE #%d avec %d items @ %d.\n", orderNumber, itemCount, OSTimeGet(&err) - startTime);

		/* TODO : Gerer la synchronisation avec les semaphores */
		OSSemPost(&sem_robot_A_to_robot_B, OS_OPT_POST_1, &err);
		errMsg(err, "Error while posting sem_robot_A_to_robot_B.");

		OSSemPend(&sem_robot_B_to_robot_A, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		errMsg(err, "Error while pending sem_robot_B_to_robot_A.");

		OSSemPost(&sem_robot_A_to_controller, OS_OPT_POST_1, &err);
		errMsg(err, "Error while posting sem_robot_A_to_controller.");

		orderNumber++;
	}
}

void robotB(void* data) {
	OS_ERR err;
	CPU_TS ts;
	OS_TICK actualticks = 0;

	int startTime = 0;
	int orderNumber = 1;
	int itemCount;
	printf("ROBOT B @ %d : DEBUT. \n", OSTimeGet(&err) - startTime);

	while (DEF_ON){
		itemCount = (rand() % 6 + 2);

		/* TODO : Gerer la synchronisation avec les semaphores*/
		OSSemPend(&sem_robot_A_to_robot_B, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		errMsg(err, "Error while pending sem_robot_A_to_robot_B.");

		/* TODO : Proteger ce bloc avec un mutex*/
		OSMutexPend(&total_item_count, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		int current = readCurrentTotalCount();
		writeCurrentTotalCount(current + itemCount);
		OSMutexPost(&total_item_count, OS_OPT_POST_NONE, &err);

		/* TODO : Faire l'attente active */
		while (itemCount + actualticks++ < OSTimeGet(&err));
		printf("ROBOT B COMMANDE #%d avec %d items @ %d.\n", orderNumber, itemCount, OSTimeGet(&err) - startTime);

		/* TODO : Gerer la synchronisation avec les semaphores */
		OSSemPost(&sem_robot_B_to_robot_A, OS_OPT_POST_1, &err);
		errMsg(err, "Error while posting sem_robot_B_to_robot_A.");

		orderNumber++;
	}
}

void controller(void* data) {
	OS_ERR err;
	CPU_TS ts;

	int startTime = 0;
	int randomTime = 0;
	printf("CONTROLLER @ %d : DEBUT. \n", OSTimeGet(&err) - startTime);
	OSTimeDly(1, OS_OPT_TIME_DLY, &err); // Au besoin pour laisser tout le monde s'initialiser dans le cas 
										 // o� le controleur est plus prioritaire
	for (int i = 1; i < 11; i++) {
		OSSemPost(&sem_controller_to_robot_A, OS_OPT_POST_1, &err);
		errMsg(err, "Error while posting sem_controller_to_robot_A.");

		printf("CONTROLLER @ %d : COMMANDE #%d. \n", OSTimeGet(&err) - startTime, i);

		/* TODO : Gerer la synchronisation avec les semaphores */
		OSSemPend(&sem_robot_A_to_controller, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		errMsg(err, "Error while pending sem_robot_A_to_controller.");
	}
}

int readCurrentTotalCount() {
	OS_ERR err;
	/*TODO : Effectuer un delai d'une duree de 2 ticks*/
	OSTimeDly(2, OS_OPT_TIME_DLY, &err);
	return total_item_count;
}
void writeCurrentTotalCount(int newCount) {
	OS_ERR err;
	/*TODO : Effectuer un delai d'une duree de 2 ticks*/
	OSTimeDly(2, OS_OPT_TIME_DLY, &err);
	total_item_count = newCount;
}
void errMsg(OS_ERR err, char* errMsg) {
	if (err != OS_ERR_NONE) {
		printf(errMsg);
		printf("%d", err);
		while (DEF_ON);
	}
}
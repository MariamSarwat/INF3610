/*
*********************************************************************************************************
*                                                 uC/OS-III
*                                          The Real-Time Kernel
*                                               PORT Windows
*
*
*												Benjamin Heinen
*                                  Ecole Polytechnique de Montreal, Qc, CANADA
*                                                  05/2019
*
* File : exo4.c
*
*********************************************************************************************************
*/

// Main include of µC-III
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

#define ROBOT_A_PRIO        8                // Defining Priority of each task
#define ROBOT_B_PRIO        9
#define CONTROLLER_PRIO     7


/*
*********************************************************************************************************
*                                             VARIABLES
*********************************************************************************************************
*/

CPU_STK           prepRobotAStk[TASK_STK_SIZE];  //Stack of each task
CPU_STK           prepRobotBStk[TASK_STK_SIZE];
CPU_STK           controllerStk[TASK_STK_SIZE];


OS_TCB robotATCB;
OS_TCB robotBTCB;
OS_TCB controllerTCB;

/*
*********************************************************************************************************
*                                           SHARED  VARIABLES
*********************************************************************************************************
*/

// Utiliser des sémaphores au besoin
OS_SEM sem_robot_B_to_robot_A;
OS_MUTEX total_prep_time_mutex;;

OS_Q Q_controller_to_robot_A;
void* A_prep_time_msg[10];

OS_Q Q_robot_A_to_robot_B;
void* B_T1_prep_time_msg[10];


volatile int total_prep_time = 0;
int controller_done = 0;
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void    robotA(void* data);
void    robotB(void* data);
void    controller(void* data);
void    errMsg(OS_ERR err, char* errMSg);
int     readCurrentTotalPrepTime();
void    writeCurrentTotalPrepTime(int qty);

/*
*********************************************************************************************************
*                                             STRUCTURES
*********************************************************************************************************
*/

typedef struct work_data {
	int work_data_a;
	int work_data_b;
} work_data;

/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

void main(void) {
	OS_ERR err;

	/* TODO : Initialiser, creer les taches et demarer */
	CPU_IntInit();

	Mem_Init();                                                 // Initialize Memory Managment Module                   
	CPU_IntDis();                                               // Disable all Interrupts                               
	CPU_Init();                                                 // Initialize the uC/CPU services                       

	OSInit(&err);                                            // Initialize uC/OS-III                                 
	if (err != OS_ERR_NONE) {
		while (1);
	}

	OSMutexCreate(&total_prep_time_mutex, "total_prep_time_mutex", &err);
	errMsg(err, "Error while creating mutex.");

	OSSemCreate(&sem_robot_B_to_robot_A, "B_to_A", 0, &err);
	errMsg(err, "Error while creating sem_robot_B_to_robot_A.");

	OSQCreate(&Q_controller_to_robot_A, "Q_controller_to_robot_A", 100, &err);
	errMsg(err, "Error while creating Q_controller_to_robot_A.");

	OSQCreate(&Q_robot_A_to_robot_B, "Q_robot_A_to_robot_B", 100, &err);
	errMsg(err, "Error while creating Q_robot_A_to_robot_B.");

	OSTaskCreate(&robotATCB, "robot_A", robotA, (void*)0, ROBOT_A_PRIO, &prepRobotAStk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE, 
		20, 0, (void*)0, (OS_OPT_TASK_NONE), &err);
	errMsg(err, "Error while creating task robot_A.");

	OSTaskCreate(&robotBTCB, "robot_B", robotB, (void*)0, ROBOT_B_PRIO, &prepRobotBStk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE, 
		20, 0, (void*)0, (OS_OPT_TASK_NONE), &err);
	errMsg(err, "Error while creating task robot_B.");

	OSTaskCreate(&controllerTCB, "controller", controller, (void*)0, CONTROLLER_PRIO, &controllerStk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE, 
		20, 0, (void*)0, (OS_OPT_TASK_NONE), &err);
	errMsg(err, "Error while creating task controller.");

	if (err == OS_ERR_NONE) {
		printf("End of task creation ! \n");
		OSStart(&err);                                          
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

void doWorkRobotA(work_data workData, int orderNumber, int startTime){
	OS_ERR err;
	CPU_TS ts;
	OS_TICK actualticks = 0;

	/* TODO : Envoi de message au Robot B*/
	OSQPost(&Q_robot_A_to_robot_B, &workData, sizeof(work_data), OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);
	errMsg(err, "Error while posting Q_robot_A_to_robot_B.");

	/* TODO : Proteger ce bloc avec un mutex*/
	OSMutexPend(&total_prep_time_mutex, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
	int current = readCurrentTotalPrepTime();
	writeCurrentTotalPrepTime(current + workData.work_data_a);
	OSMutexPost(&total_prep_time_mutex, OS_OPT_POST_NONE, &err);
	
	/* TODO : Faire l'attente active */
	while (workData.work_data_a + actualticks++ < OSTimeGet(&err));
	printf("ROBOT A COMMANDE #%d avec %d items @ %d.\n", orderNumber, workData.work_data_a, OSTimeGet(&err) - startTime);

	// Synchronisation si requise (à justifier) 
	// Quand Robot_B a fini, on utilise la sémaphore sem_robot_B_to_robot_A pour faire la synchro avec Robot_B, 
	//de manière à ce que Robot_A puisse retourner attendre en 2)
	OSSemPend(&sem_robot_B_to_robot_A, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
	errMsg(err, "Error while pending sem_robot_B_to_robot_A.");
}

void robotA(void* data){
	OS_ERR err;
	CPU_TS ts;

	int startTime = 0;
	int orderNumber = 1;
	work_data* workData;
	OS_MSG_SIZE msg_size;
	printf("%d: TACHE PREP_ROBOT_A @ %d : DEBUT.\n", OSPrioCur, OSTimeGet(&err) - startTime);

	while (1) {
		/* TODO : Recuperer la quantite*/
		workData = (work_data*)OSQPend(&Q_controller_to_robot_A, 0, OS_OPT_PEND_BLOCKING, &msg_size, NULL, &err);
		errMsg(err, "Error while pending Q_controller_to_robot_A.");

		doWorkRobotA(*workData, orderNumber, startTime);

		/* TODO : Liberer la memoire*/
		free(workData);
		orderNumber++;
	}
}

void robotB(void* data) {
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE msg_size;
	OS_TICK actualticks = 0;

	int startTime = 0;
	int orderNumber = 1;
	int* robotBPrepTime;
	printf("%d: TACHE PREP_ROBOT_B @ %d : DEBUT. \n", OSPrioCur, OSTimeGet(&err) - startTime);

	while (1) {
		/* TODO : Recuperer la quantite */
		robotBPrepTime = ((work_data*)OSQPend(&Q_robot_A_to_robot_B, 0, OS_OPT_PEND_BLOCKING, &msg_size, NULL, &err))->work_data_b;
		errMsg(err, "Error while pending Q_robot_A_to_robot_B.");

		/* TODO : Proteger ce bloc avec un mutex*/
		OSMutexPend(&total_prep_time_mutex, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		int current = readCurrentTotalPrepTime();
		writeCurrentTotalPrepTime(current + robotBPrepTime);
		OSMutexPost(&total_prep_time_mutex, OS_OPT_POST_NONE, &err);

		/* TODO : Faire l'attente active */
		while (robotBPrepTime + actualticks++ < OSTimeGet(&err));
		printf("ROBOT B COMMANDE #%d avec %d items @ %d.\n", orderNumber, (int)robotBPrepTime, OSTimeGet(&err) - startTime);

		/* TODO : Synchronisation si requise (à justifier) */
		// Quand Robot_B a fini, on utilise la sémaphore sem_robot_B_to_robot_A pour faire la synchro avec Robot_B, 
		//de manière à ce que Robot_A puisse retourner attendre en 2)
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
	work_data* workData;
	printf("TACHE CONTROLLER @ %d : DEBUT. \n", OSTimeGet(&err) - startTime);
	while (1) {
		for (int i = 1; i < 12; i++) {
			workData = malloc(sizeof(work_data));
			workData->work_data_a = (rand() % 7 + 1);
			workData->work_data_b = (rand() % 6 + 2);

			printf("TACHE CONTROLLER @ %d : COMMANDE #%d. \n prep time A = %d, prep time B = %d\n", OSTimeGet(&err) - startTime, i, workData->work_data_a, workData->work_data_b);

			/* TODO :  Envoi de la commande */
			OSQPost(&Q_controller_to_robot_A, workData, sizeof(work_data), OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);
			errMsg(err, "Error while posting Q_controller_to_robot_A.");

			// Délai aléatoire avant nouvelle commande
			randomTime = (rand() % 9 + 5) * 4;
			OSTimeDly(randomTime, OS_OPT_TIME_DLY, &err);
		}
	}
}

int readCurrentTotalPrepTime() {
	OS_ERR err;
	/*TODO : Effectuer un delai d'une duree de 2 ticks*/
	OSTimeDly(2, OS_OPT_TIME_DLY, &err);
	return total_prep_time;
}

void writeCurrentTotalPrepTime(int newCount) {
	OS_ERR err;
	/*TODO : Effectuer un delai d'une duree de 2 ticks*/
	OSTimeDly(2, OS_OPT_TIME_DLY, &err);
	total_prep_time = newCount;
}

void errMsg(OS_ERR err, char* errMsg){
	if (err != OS_ERR_NONE){
		printf(errMsg);
		exit(1);
	}
}
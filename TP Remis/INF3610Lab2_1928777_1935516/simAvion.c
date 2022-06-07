/*
*********************************************************************************************************
*                                                 uC/OS-II
*                                          The Real-Time Kernel
*                                               PORT Windows
*
*
*            		          	Arnaud Desaulty, Frederic Fortier, Eva Terriault
*                                  Ecole Polytechnique de Montreal, Qc, CANADA
*                                                  08/2017
*
* File : simulateur.c
*
*********************************************************************************************************
*/

// Main include of ï¿½C-II
#include "simAvion.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

///////////////////////////////////////////////////////////////////////////////////////
//								Routines d'interruptions
///////////////////////////////////////////////////////////////////////////////////////

//Ecrivez les routines d'interruption des deux fit_timer (celui qui s'execute a chaque seconde et celui qui s'execute a chaque 3 secondes) et du gpio

//using generation task
void fit_timer_1s(void* not_valid) {
	OS_ERR err;
	while (1) {
		OSTimeDlyHMSM(0, 0, 1, 0, OS_OPT_TIME_HMSM_STRICT, &err);
		//printf("fit 1 isr \n");
		OSSemPost(&semGeneration, OS_OPT_POST_1, &err);
		errMsg(err, "Erreur lors du post de semGen");
	}
}

//using verirfication task
void fit_timer_3s(void* not_valid) {
	OS_ERR err;
	while (1) {
		OSTimeDlyHMSM(0, 0, 3, 0, OS_OPT_TIME_HMSM_STRICT, &err);
		//printf("fit 3 isr \n");
		OSSemPost(&semVerification, OS_OPT_POST_1, &err);
		errMsg(err, "Erreur lors du post de semVer");
	}
}

//using statistique task
void gpio_isr(void* data) {
	OS_ERR err;
	while (1) {
		OSTimeDlyHMSM(0, 0, 9, 0, OS_OPT_TIME_HMSM_STRICT, &err);
		//printf("gpio isr \n");
		OSSemPost(&semStatistiques, OS_OPT_POST_1, &err);
		errMsg(err, "Erreur lors du post de semStat");
	}
}

/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

int main(void) {
	OS_ERR os_err;

	CPU_IntInit();

	Mem_Init(); // Initialize Memory Managment Module 
	CPU_IntDis(); // Disable all Interrupts 
	CPU_Init(); // Initialize the uC/CPU services 

	OSInit(&os_err);
	create_application();
	OSStart(&os_err);
	return 0;
}

void create_application() {
	int error;
	error = create_tasks();
	if (error != 0) printf("Error %d while creating tasks\n", error);

	error = create_events();
	if (error != 0)	printf("Error %d while creating events\n", error);
}

int create_tasks() {
	OS_ERR err;

	// Stacks
	static CPU_STK generationStk[TASK_STK_SIZE]; 
	static CPU_STK atterrissageStk[TASK_STK_SIZE];
	static CPU_STK terminal0Stk[TASK_STK_SIZE];
	static CPU_STK terminal1Stk[TASK_STK_SIZE];
	static CPU_STK decollageStk[TASK_STK_SIZE];
	static CPU_STK statistiquesStk[TASK_STK_SIZE];
	static CPU_STK verificationStk[TASK_STK_SIZE];
	static CPU_STK fitTimer1sStk[TASK_STK_SIZE];
	static CPU_STK fitTimer3sStk[TASK_STK_SIZE];
	static CPU_STK gpioIsrStk[TASK_STK_SIZE];


	/*TODO : Creer les differentes taches*/
	OSTaskCreate(&fitTimer1sTCB, "fit timer 1s task", fit_timer_1s, (void*)0, FIT_TIMER_1S_PRIO, &fitTimer1sStk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		100, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task fit timer 1s.");

	OSTaskCreate(&fitTimer3sTCB, "fit timer 3s task", fit_timer_3s, (void*)0, FIT_TIMER_3S_PRIO, &fitTimer3sStk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		100, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task fit timer 3s.");

	OSTaskCreate(&gpioIsrTCB, "gpio isr task", gpio_isr, (void*)0, GPIO_ISR_PRIO, &gpioIsrStk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		100, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task gpio isr 3s.");

	OSTaskCreate(&generationTCB, "generation task", generation, (void*)0, GENERATION_PRIO, &generationStk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		100, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task generation.");

	OSTaskCreate(&atterrissageTCB, "atterrissage task", atterrissage, (void*)0, ATTERRISSAGE_PRIO, &atterrissageStk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		100, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task atterrissage.");

	OSTaskCreate(&terminal0TCB, "terminal 0 task", terminal, 0, TERMINAL0_PRIO, &terminal0Stk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		100, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task terminal 0.");

	OSTaskCreate(&terminal1TCB, "terminal 1 task", terminal, (void*)1, TERMINAL1_PRIO, &terminal1Stk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		100, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task terminal 1.");

	OSTaskCreate(&decollageTCB, "decollage task", decollage, (void*)0, DECOLLAGE_PRIO, &decollageStk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		100, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task decollage.");

	OSTaskCreate(&statistiquesTCB, "statistiques task", statistiques, (void*)0, STATISTIQUES_PRIO, &statistiquesStk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		100, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task statistiques.");

	OSTaskCreate(&verificationTCB, "verification task", verification, (void*)0, VERIFICATION_PRIO, &verificationStk[0u], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		100, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task verification.");

	return 0;
}

int create_events() {
	OS_ERR err;
	//TODO : Creer les mutex, files, semaphores que vous utilisez dans ce devoir
	OSSemCreate(&semGeneration, "semGeneration", 0, &err);
	errMsg(err, "Error while creating the semGeneration.");

	OSSemCreate(&semVerification, "semVerification", 0, &err);
	errMsg(err, "Error while creating the semVerification.");

	OSSemCreate(&semStatistiques, "semStatistiques", 0, &err);
	errMsg(err, "Error while creating the semStatistiques.");

	OSMutexCreate(&mutexPrint, "mutexPrint", &err);
	errMsg(err, "Error while creating the mutexPrint.");

	OSMutexCreate(&mutexDecollage, "mutexDecollage", &err);
	errMsg(err, "Error while creating the mutexDecollage.");

	OSMutexCreate(&safe, "mutexAtterrissage", &err);
	errMsg(err, "Error while creating the mutexAtterrissage.");

	OSFlagCreate(&terminal_status, "flag group", 0x00, &err);
	errMsg(err, "Error while creating the mutexTerminal2.");

	OSQCreate(&Q_atterrissage_high, "Q_atterrissage_high", 3, &err);
	errMsg(err, "Error while creating the Q_atterrissage_high.");

	OSQCreate(&Q_atterrissage_medium, "Q_atterrissage_medium", 4, &err);
	errMsg(err, "Error while creating the Q_atterrissage_medium.");

	OSQCreate(&Q_atterrissage_low, "Q_atterrissage_low", 6, &err);
	errMsg(err, "Error while creating the Q_atterrissage_low.");

	OSQCreate(&Q_decollage, "Q_decollage", 10, &err);
	errMsg(err, "Error while creating the Q_decollage.");

	OSQCreate(&Q_terminal_0, "Q_terminal_0", 1, &err);
	errMsg(err, "Error while creating the Q_terminal_1.");

	OSQCreate(&Q_terminal_1, "Q_terminal_1", 1, &err);
	errMsg(err, "Error while creating the Q_terminal_2.");

	return 0;
}

/*
*********************************************************************************************************
*                                            TASK FUNCTIONS
********************************************************************************************************
*/

void generation(void* data) {
	OS_ERR err;
	CPU_TS ts;
	int nbAvionsCrees = 0;
	safePrint("[GENERATION] Tache lancee\n");
	int skipGen = 0;
	int seed = 34;

	while (1) {
		/*TODO: Synchronisation unilaterale timer 1s*/
		OSSemPend(&semGeneration, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		errMsg(err, "Error while pending semGeneration\n");

		srand(seed);
		skipGen = rand() % 5; //On saute la generation 1 fois sur 6

		if (skipGen != 0) {
			//TODO: fix
			OSMutexPend(&safe, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
			errMsg(err, "Error while pending safe mutex in generation\n");
			Avion* avion = malloc(sizeof(Avion));
			OSMutexPost(&safe, OS_OPT_POST_NONE, &err);
			errMsg(err, "Error while posting safe mutex in generation\n");

			avion->id = nbAvionsCrees;
			remplirAvion(avion);
			nbAvionsCrees++;
			
			/*TODO: Envoi des avions dans les files appropriees*/
			if (avion->retard >= BORNE_INF_LOW && avion->retard <= BORNE_SUP_LOW && Q_atterrissage_low.MsgQ.NbrEntries < 6) {
				OSQPost(&Q_atterrissage_low, avion, sizeof(Avion), OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);
				errMsg(err, "Error while posting the plane to the q_atterissage_low \n");
			}
			else if (avion->retard <= BORNE_SUP_MEDIUM && Q_atterrissage_medium.MsgQ.NbrEntries < 4) {
				OSQPost(&Q_atterrissage_medium, avion, sizeof(Avion), OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);
				errMsg(err, "Error while posting the plane to the q_atterissage_medium \n");
			}
			else if (avion->retard <= BORNE_SUP_HIGH && Q_atterrissage_high.MsgQ.NbrEntries < 3) {
				OSQPost(&Q_atterrissage_high, avion, sizeof(Avion), OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);
				errMsg(err, "Error while posting the plane to the q_atterissage_high\n");
			}
			else {
				safePrint("[GENERATION] Error overflow of queue!\n");
				stopSimDebordement = true;
			}
		}
		else {
			safePrint("[GENERATION] Generation has been skipped!\n");
		}
		seed++;
	}
}

void atterrissage(void* data) {
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE msg_size;

	Avion* avion = NULL;
	safePrint("[ATTERRISSAGE] Tache lancee\n");
	while (1) {
		/*TODO: Mise en attente des 3 files en fonction de leur priorité*/

		while (1) {
			avion = (Avion*)OSQPend(&Q_atterrissage_high, 0, OS_OPT_PEND_BLOCKING, &msg_size, NULL, &err);
			if (avion != NULL) break;
			avion = (Avion*)OSQPend(&Q_atterrissage_medium, 0, OS_OPT_PEND_BLOCKING, &msg_size, NULL, &err);
			if (avion != NULL) break;
			avion = (Avion*)OSQPend(&Q_atterrissage_low, 0, OS_OPT_PEND_BLOCKING, &msg_size, NULL, &err);
			if (avion != NULL) break;
		}
		safePrint("[ATTERRISSAGE] Starting descent of plane\n");
		/*TODO: Mise en attente d'un terminal libre (mecanisme a votre choix)*/

		OSTimeDly(150, OS_OPT_TIME_DLY, &err); //Temps pour que l'avion atterrisse

		safePrint("[ATTERRISSAGE] Looking for free terminal\n");

		/*TODO: Envoi de l'avion au terminal choisi (mecanisme de votre choix)*/
		OS_FLAGS flag = OSFlagPend(&terminal_status, TERMINALS_READY, 0, OS_OPT_PEND_FLAG_CLR_ANY, &ts, &err);
		errMsg(err, "Error while getting flag group ready\n");
		if (flag == TERMINAL_0_READY || flag == TERMINALS_READY) {
			safePrint("[ATTERRISSAGE] Terminal 1 obtained\n");
			OSFlagPost(&terminal_status, TERMINAL_0_READY, OS_OPT_POST_FLAG_SET, &err);
			OSQPost(&Q_terminal_0, avion, sizeof(Avion), OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);
			errMsg(err, "Error while posting for terminal_0\n");
		}
		else if (flag == TERMINAL_1_READY) {
			safePrint("[ATTERRISSAGE] Terminal 2 obtained\n");
			OSFlagPost(&terminal_status, TERMINAL_1_READY, OS_OPT_POST_FLAG_SET, &err);
			OSQPost(&Q_terminal_1, avion, sizeof(Avion), OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);
			errMsg(err, "Error while posting for terminal_1\n");
		}
	}
}

void terminal(void* data) {
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE msg_size;
	int numTerminal = (int*)data;
	Avion* avion = NULL;
	numTerminal == 0 ? safePrint("[TERMINAL 1] Tache lancee\n") : safePrint("[TERMINAL 2] Tache lancee\n");

	while (1) {
		/*TODO: Mise en attente d'un avion venant de la piste d'atterrissage*/
		if (numTerminal == 0) {
			avion = (Avion*)OSQPend(&Q_terminal_0, 0, OS_OPT_PEND_BLOCKING, &msg_size, NULL, &err);
			safePrint("[TERMINAL 1] Obtaining Plane\n");
		}
		else {
			avion = (Avion*)OSQPend(&Q_terminal_1, 0, OS_OPT_PEND_BLOCKING, &msg_size, NULL, &err);
			safePrint("[TERMINAL 2] Obtaining Plane\n");
		}
		
		OSTimeDly(160, OS_OPT_TIME_DLY, &err);
		remplirAvion(avion);

		/*TODO: Envoi de l'avion pour le piste de decollage*/
		if (Q_decollage.MsgQ.NbrEntries < 6) {
			OSQPost(&Q_decollage, avion, sizeof(Avion), OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);
			errMsg(err, "Error while posting plane for take off\n");
		}
		else {
			stopSimDebordement = true;
		}

		/*TODO: Notifier que le terminal est libre (mecanisme de votre choix)*/
		if (numTerminal == 0) {
			safePrint("[TERMINAL 1] Liberating plane\n");
			OSFlagPost(&terminal_status, TERMINAL_0_READY, OS_OPT_POST_FLAG_CLR, &err);
			errMsg(err, "Error while posting for terminal_0\n");
		}
		else if (numTerminal == 1) {
			safePrint("[TERMINAL 2] Liberating plane\n");
			OSFlagPost(&terminal_status, TERMINAL_1_READY, OS_OPT_POST_FLAG_CLR, &err);
			errMsg(err, "Error while posting for terminal_1\n");
		}
	}
}

void decollage(void* data) {
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE msg_size;

	Avion* avion = NULL;
	safePrint("[DECOLLAGE] Tache lancee\n");

	while (1) {
		/*TODO: Mise en attente d'un avion pret pour le decollage*/
		avion = (Avion*)OSQPend(&Q_decollage, 0, OS_OPT_PEND_BLOCKING, &msg_size, NULL, &err);
		errMsg(err, "Error while pending for a plane for takeoff\n");
		
		/*TODO: Destruction de l'avion*/
		OSTimeDly(30, OS_OPT_TIME_DLY, &err); //Temps pour que l'avion decolle
		safePrint("[DECOLLAGE] Plane\n");
		
		OSMutexPend(&safe, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		errMsg(err, "Error while pending the mutexDecollage\n");
		free(avion);
		OSMutexPost(&safe, OS_OPT_POST_NONE, &err);
		errMsg(err, "Error while posting the mutexDecollage\n");
	}
}

void statistiques(void* data) {
	OS_ERR err;
	CPU_TS ts;
	safePrint("[STATISTIQUES] Tache lancee\n");

	while (1) {
		/*TODO: Synchronisation unilaterale switches*/
		OSSemPend(&semStatistiques, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		errMsg(err, "Error with pending semStatistique\n");

		safePrint("\n------------------ Affichage des statistiques ------------------\n");

		/*TODO: Obtenir statistiques pour les files d'atterrissage*/
		OSMutexPend(&mutexPrint, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		printf("Nb d'avions en attente d'atterrissage de type High : %d\n", Q_atterrissage_high.MsgQ.NbrEntries);
		printf("Nb d'avions en attente d'atterrissage de type Medium : %d\n", Q_atterrissage_medium.MsgQ.NbrEntries);
		printf("Nb d'avions en attente d'atterrissage de type Low : %d\n", Q_atterrissage_low.MsgQ.NbrEntries);

		/*TODO: Obtenir statistiques pour la file de decollage*/
		printf("Nb d'avions en attente de decollage : %d\n", Q_decollage.MsgQ.NbrEntries);
		OSMutexPost(&mutexPrint, OS_OPT_POST_NONE, &err);

		/*TODO: Obtenir statut des terminaux*/
		safePrint("Terminal 1 ");
		(Q_terminal_0.MsgQ.NbrEntries != 0) ? safePrint("OCCUPE\n") : safePrint("LIBRE\n");

		safePrint("Terminal 2 ");
		(Q_terminal_1.MsgQ.NbrEntries != 0) ? safePrint("OCCUPE\n\n") : safePrint("LIBRE\n\n");
	}
}


void verification(void* data) {
	OS_ERR err;
	CPU_TS ts;
	safePrint("[VERIFICATION] Tache lancee\n");

	while (1) {
		/*TODO: Synchronisation unilaterale avec timer 3s*/
		OSSemPend(&semVerification, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		errMsg(err, "Error with pending semVerification\n");

		if (stopSimDebordement) {
			printf("\n\nSUSPENDING ALL TASKS\n\n");
			/*TODO: Suspension de toutes les taches de la simulation*/
			OSTaskSuspend(&generationTCB, &err);
			errMsg(err, "Error with generation task suspension.\n");

			OSTaskSuspend(&atterrissageTCB, &err);
			errMsg(err, "Error with landing task suspension.\n");

			OSTaskSuspend(&terminal0TCB, &err);
			errMsg(err, "Error with terminal 1 task suspension.\n");

			OSTaskSuspend(&terminal1TCB, &err);
			errMsg(err, "Error with terminal 2 task suspension.\n");

			OSTaskSuspend(&decollageTCB, &err);
			errMsg(err, "Error with take off task suspension.\n");

			OSTaskSuspend(&statistiquesTCB, &err);
			errMsg(err, "Error with statistic task suspension.\n");

			OSTaskSuspend(&fitTimer1sTCB, &err);
			errMsg(err, "Error with verification task suspension.\n");

			OSTaskSuspend(&fitTimer3sTCB, &err);
			errMsg(err, "Error with verification task suspension.\n");

			OSTaskSuspend(&gpioIsrTCB, &err);
			errMsg(err, "Error with verification task suspension.\n");

			OSTaskSuspend(&verificationTCB, &err);
			errMsg(err, "Error with verification task suspension.\n");
		}
	}
}

void remplirAvion(Avion* avion) {
	OS_ERR err;

	srand(OSTimeGet(&err));
	avion->retard = rand() % BORNE_SUP_HIGH;
	avion->origine = rand() % NB_AEROPORTS;
	do { avion->destination = rand() % NB_AEROPORTS; } while (avion->origine == avion->destination);
	/*OSMutexPend(&mutexPrint, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
	printf("Avion retard = %d\n", avion->retard);
	printf("Avion origine = %d\n", avion->origine);
	printf("Avion destination = %d\n", avion->destination);
	OSMutexPost(&mutexPrint, OS_OPT_POST_NONE, &err);*/
	safePrint("[GENERATION]\n");
}

void errMsg(OS_ERR  err, char* errMsg) {
	CPU_TS ts;
	if (err != OS_ERR_NONE) {
		OSMutexPend(&mutexPrint, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		printf(errMsg);
		printf("%d", err);
		OSMutexPost(&mutexPrint, OS_OPT_POST_NONE, &err);
		exit(1);
	}
}

void safePrint(char* msg) {
	OS_ERR err;
	CPU_TS ts;
	OSMutexPend(&mutexPrint, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
	printf(msg);
	OSMutexPost(&mutexPrint, OS_OPT_POST_NONE, &err);
}

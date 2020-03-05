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

#include "bsp_init.h"

#include <stdlib.h>
#include <stdbool.h>
#include <xil_printf.h>
#include <xgpio.h>

///////////////////////////////////////////////////////////////////////////////////////
//								Routines d'interruptions
///////////////////////////////////////////////////////////////////////////////////////

//Ecrivez les routines d'interruption des deux fit_timer (celui qui s'execute a chaque seconde et celui qui s'execute a chaque 3 secondes) et du gpio

//using generation task
void fit_timer_1s_isr(void *not_valid) {
	/*definition handler pour timer 1s*/
	OS_ERR err;
	OSSemPost(&semGeneration, OS_OPT_POST_1, &err);
	errMsg(err, "Erreur lors du post de semGen.");
}

//using verirfication task
void fit_timer_3s_isr(void *not_valid) {
	/*definition handler pour timer 3s*/
	OS_ERR  err;
	OSSemPost(&semVerification, OS_OPT_POST_1, &err);
	errMsg(err, "Erreur lors du post de semVer");
}

//using statistique task
void gpio_isr(void * data) {
	/*definition handler gpio*/
	OS_ERR  err;
	OSSemPost(&semStatistiques,OS_OPT_POST_1, &err);
	errMsg(err, "Erreur lors du post de semStat");

	XGpio_InterruptClear(&gpSwitch, XGPIO_IR_MASK);
}

/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

int main(void){

    OS_ERR  os_err;
    UCOS_LowLevelInit();

    CPU_Init();
    Mem_Init();
    OSInit(&os_err);
	create_application();

    init_interrupt();
    connect_axi();
    OSStart(&os_err);

    return 0;
}

void create_application() {
	int error;

	error = create_tasks();
	if (error != 0)
		xil_printf("Error %d while creating tasks\n", error);

	error = create_events();
	if (error != 0)
		xil_printf("Error %d while creating events\n", error);
}

int create_tasks() {
	OS_ERR err;
	// Stacks
	static CPU_STK generationStk[TASK_STK_SIZE]; //Stack of each task
	static CPU_STK atterrissage0Stk[TASK_STK_SIZE];
	static CPU_STK terminal0Stk[TASK_STK_SIZE];
	static CPU_STK terminal1Stk[TASK_STK_SIZE];
	static CPU_STK decollageStk[TASK_STK_SIZE];
	static CPU_STK statistiquesStk[TASK_STK_SIZE];
	static CPU_STK verificationStk[TASK_STK_SIZE];

	static OS_TCB generationTCB;
	static OS_TCB atterrissageTCB;
	static OS_TCB terminal0TCB;
	static OS_TCB terminal1TCB;
	static OS_TCB decollageTCB;
	static OS_TCB statistiquesTCB;
	static OS_TCB verificationTCB;

	/*TODO : Creer les differentes taches*/

	OSTaskCreate(&generationTCB, "generation task", generation, (void*)0, GENERATION_PRIO, &generationStk[TASK_STK_SIZE - 1], TASK_STK_SIZE / 2, TASK_STK_SIZE,
		20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task generation.");

	OSTaskCreate(&atterrissageTCB, "atterrissage task", atterrissage, (void*)0, ATTERRISSAGE_PRIO, &atterrissage0Stk[TASK_STK_SIZE - 1], TASK_STK_SIZE / 2, TASK_STK_SIZE,
			20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task atterrissage.");

	OSTaskCreate(&terminal0TCB, "terminal 0 task", terminal, (void*)0, TERMINAL0_PRIO, &terminal0Stk[TASK_STK_SIZE - 1], TASK_STK_SIZE / 2, TASK_STK_SIZE,
			20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task terminal 0.");

	OSTaskCreate(&terminal1TCB, "terminal 1 task", terminal, (void*)0, TERMINAL1_PRIO, &terminal1Stk[TASK_STK_SIZE - 1], TASK_STK_SIZE / 2, TASK_STK_SIZE,
			20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task terminal 1.");

	OSTaskCreate(&decollageTCB, "decollage task", decollage, (void*)0, DECOLLAGE_PRIO, &decollageStk[TASK_STK_SIZE - 1], TASK_STK_SIZE / 2, TASK_STK_SIZE,
			20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task decollage.");

	OSTaskCreate(&statistiquesTCB, "statistiques task", statistiques, (void*)0, STATISTIQUES_PRIO, &statistiquesStk[TASK_STK_SIZE - 1], TASK_STK_SIZE / 2, TASK_STK_SIZE,
			20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
	errMsg(err, "Error while creating the task statistiques.");

	OSTaskCreate(&verificationTCB, "verification task", verification, (void*)0, VERIFICATION_PRIO, &verificationStk[TASK_STK_SIZE - 1], TASK_STK_SIZE / 2, TASK_STK_SIZE,
			20, 0, (void*)0, (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), &err);
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

	OSMutexCreate(&mutexAtterrissage, "mutexAtterrissage", &err);
	errMsg(err, "Error while creating the mutexAtterrissage.");

	OSMutexCreate(&mutexTerminal1, "mutexTerminal1", &err);
	errMsg(err, "Error while creating the mutexTerminal1.");

	OSMutexCreate(&mutexTerminal2, "mutexTerminal2", &err);
	errMsg(err, "Error while creating the mutexTerminal2.");

	OSQCreate(&Q_atterrissage_high, "Q_atterrissage_high", 3, &err);
	errMsg(err, "Error while creating the Q_atterrissage_high.");

	OSQCreate(&Q_atterrissage_medium, "Q_atterrissage_medium",4, &err);
	errMsg(err, "Error while creating the Q_atterrissage_medium.");

	OSQCreate(&Q_atterrissage_low, "Q_atterrissage_low",6, &err);
	errMsg(err, "Error while creating the Q_atterrissage_low.");

	OSQCreate(&Q_decollage, "Q_decollage",10, &err);
	errMsg(err, "Error while creating the Q_decollage.");

	OSQCreate(&Q_terminal_1, "Q_terminal_1",1, &err);
	errMsg(err, "Error while creating the Q_terminal_1.");

	OSQCreate(&Q_terminal_2, "Q_terminal_2",1, &err);
	errMsg(err, "Error while creating the Q_terminal_2.");

	return 0;
}

/*
*********************************************************************************************************
*                                            TASK FUNCTIONS
*********************************************************************************************************
*/
void generation(void* data) {
	OS_ERR err;
	CPU_TS ts;
	int nbAvionsCrees = 0;
	print("Tache generation lancee\n");
	int skipGen = 0;
	int seed = 34;
	while (DEF_ON) {

		/*TODO: Synchronisation unilaterale timer 1s*/
		OSSemPend(&semGeneration, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		errMsg(err, "Error while pending semGeneration");

		srand(seed);
		skipGen = rand() % 5; //On saute la generation 1 fois sur 6
		if (skipGen != 0){
			Avion* avion = malloc(sizeof(Avion));
			avion->id = nbAvionsCrees;
			remplirAvion(avion);
			nbAvionsCrees++;

			/*TODO: Envoi des avions dans les files appropriees*/
			if (avion->retard >= BORNE_INF_LOW && avion->retard <= BORNE_SUP_LOW) {
				OSQPost(&Q_atterrissage_low, avion, sizeof(Avion), OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);
				if(err != OS_ERR_NONE) stopSimDebordement = true;
				errMsg(err, "Error while posting the plane to the q_atterissage_low");
			}
			else if (avion->retard <= BORNE_SUP_MEDIUM) {
				OSQPost(&Q_atterrissage_medium, avion, sizeof(Avion), OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);
				if(err != OS_ERR_NONE) stopSimDebordement = true;
				errMsg(err, "Error while posting the plane to the q_atterissage_medium");
			}
			else if (avion->retard <= BORNE_SUP_HIGH) {
				OSQPost(&Q_atterrissage_high, avion, sizeof(Avion), OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);
				if(err != OS_ERR_NONE) stopSimDebordement = true;
				errMsg(err, "Error while posting the plane to the q_atterissage_high");
			}
			else errMsg(err, "Lateness not within boundaries");
		}
		else{
			print("Plane generation has been skipped!\n");
		}
		seed++;
	}
}

void atterrissage(void* data) {
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE msg_size;

	Avion* avion = NULL;
	print("Tache atterrissage lancee\n");
	while (DEF_ON) {
		/*TODO: Mise en attente des 3 files en fonction de leur priorité*/
		if ((avion = OSQAccept(Q_atterrissage_high, &err)) != NULL) {
			print("[ATTERRISSAGE] Debut atterrissage\n");
			OSMutexPend(&mutexAtterrissage, 0, OS_OPT_PEND_BLOCKING, &ts,&err);
			errMsg(err, "Error while pending mutex_atterissage for HIGH priority");
			OSTimeDly(150, OS_OPT_TIME_DLY, &err); //Temps pour que l'avion atterrisse
			print("[ATTERRISSAGE] Attente terminal libre\n");
			OSMutexPost(&mutexAtterrissage, OS_OPT_POST_NONE, &err);
			errMsg(err, "Error while posting mutex_atterissage for HIGH priority");
		}

		else if ((avion = OSQAccept(Q_atterrissage_medium, &err)) != NULL) {
			print("[ATTERRISSAGE] Debut atterrissage\n");
			OSMutexPend(&mutexAtterrissage, 0, OS_OPT_PEND_BLOCKING, &ts,&err);
			errMsg(err, "Error while pending mutex_atterissage for MEDIUM priority");
			OSTimeDly(150, OS_OPT_TIME_DLY, &err); //Temps pour que l'avion atterrisse
			print("[ATTERRISSAGE] Attente terminal libre\n");
			OSMutexPost(&mutexAtterrissage, OS_OPT_POST_NONE, &err);
			errMsg(err, "Error while posting mutex_atterissage for MEDIUM priority");
		}

		else if ((avion = OSQAccept(Q_atterrissage_low, &err)) != NULL) {
			print("[ATTERRISSAGE] Debut atterrissage\n");
			OSMutexPend(&mutexAtterrissage, 0, OS_OPT_PEND_BLOCKING, &ts,&err);
			errMsg(err, "Error while pending mutex_atterissage for LOW priority");
			OSTimeDly(150, OS_OPT_TIME_DLY, &err); //Temps pour que l'avion atterrisse
			print("[ATTERRISSAGE] Attente terminal libre\n");
			OSMutexPost(&mutexAtterrissage, OS_OPT_POST_NONE, &err);
			errMsg(err, "Error while posting mutex_atterissage for LOW priority");
		}

		/*TODO: Mise en attente d'un terminal libre (mecanisme a votre choix)*/
        /*TODO: Envoi de l'avion au terminal choisi (mecanisme de votre choix)*/
		if ((avion != NULL && OSQAccept(Q_terminal_1, &err)) == NULL) {
			OSQPost(&Q_terminal_1, avion, sizeof(Avion), OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);
			errMsg(err, "Error while posting for terminal_1");
			avion = NULL;
			print("[ATTERRISSAGE] Terminal libre num 1 obtenu\n");
		}
		if ((avion != NULL && OSQAccept(Q_terminal_2, &err)) == NULL) {
			OSQPost(&Q_terminal_2, avion, sizeof(Avion), OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);
			errMsg(err, "Error while posting for terminal_2");
			avion = NULL;
			print("[ATTERRISSAGE] Terminal libre num 2 obtenu\n");
		}
	}
}

void terminal(void* data)
{
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE msg_size;
	int numTerminal = (int*)data;
	Avion* avion = NULL;
	numTerminal == 1 ? print("[TERMINAL 1] Tache lancee\n") : print("[TERMINAL 2] Tache lancee\n");

	while (DEF_ON) {

		/*TODO: Mise en attente d'un avion venant de la piste d'atterrissage*/
		if (numTerminal == 1) {
			avion = OSQPend(&Q_terminal_1, 0, OS_OPT_PEND_BLOCKING, &msg_size, NULL, &err);
			OSMutexPend(&mutexTerminal1, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
			errMsg(err, "Error while pending the mutexTerminal1");
			print("TERMINAL 1: Obtention avion\n");
			//Attente pour le vidage, le nettoyage et le remplissage de l'avion
			OSTimeDly(160, OS_OPT_TIME_DLY, &err);
			remplirAvion(avion);
			OSMutexPost(&mutexTerminal1, OS_OPT_POST_NONE, &err);
			errMsg(err, "Error while posting the mutexTerminal1");
		}
		else if (numTerminal == 2) {
			avion = OSQPend(&Q_terminal_2, 0, OS_OPT_PEND_BLOCKING, &msg_size, NULL, &err);
			OSMutexPend(&mutexTerminal2, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
			errMsg(err, "Error while pending the mutexTerminal2");
			print("TERMINAL 2: Obtention avion\n");
			//Attente pour le vidage, le nettoyage et le remplissage de l'avion
			OSTimeDly(160, OS_OPT_TIME_DLY, &err);
			remplirAvion(avion);
			OSMutexPost(&mutexTerminal2, OS_OPT_POST_NONE, &err);
			errMsg(err, "Error while posting the mutexTerminal2");
		}
		else print("Invalid terminal number");

		/*TODO: Envoi de l'avion pour le piste de decollage*/
		OSQPost(&Q_decollage, avion, sizeof(Avion), OS_OPT_POST_FIFO + OS_OPT_POST_NO_SCHED, &err);
		errMsg(err, "Error while posting plane for take off");

 
        /*TODO: Notifier que le terminal est libre (mecanisme de votre choix)*/
		if (numTerminal == 1) print("[TERMINAL 1] Liberation avion\n");
		else if (numTerminal == 2) print("[TERMINAL 2] Liberation avion\n");
	}

}

void decollage(void* data)
{
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE msg_size;

	Avion* avion = NULL;
	print("Tache decollage lancee\n");

	while (DEF_ON) {
		/*TODO: Mise en attente d'un avion pret pour le decollage*/
		avion = OSQPend(&Q_decollage, 0, OS_OPT_PEND_BLOCKING, &msg_size, NULL, &err);
		errMsg(err, "Error while pending for a plane for takeoff");
		OSMutexPend(&mutexDecollage, 0, OS_OPT_PEND_BLOCKING, &ts,&err);
		errMsg(err, "Error while pending the mutexDecollage");
		OSTimeDly(30, OS_OPT_TIME_DLY, &err); //Temps pour que l'avion decolle
		OSMutexPost(&mutexDecollage, OS_OPT_POST_NONE, &err);
		errMsg(err, "Error while posting the mutexDecollage");
		print("DECOLLAGE: decollage avion\n");

        /*TODO: Destruction de l'avion*/
		free(avion);
	}
}


void statistiques(void* data){
	OS_ERR err;
	CPU_TS ts;
	print("[STATISTIQUES] Tache lancee\n");

	while(DEF_ON){
        /*TODO: Synchronisation unilaterale switches*/
		OSSemPend(&semStatistiques, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		print("\n------------------ Affichage des statistiques ------------------\n");

		/*TODO: Obtenir statistiques pour les files d'atterrissage*/
		OSMutexPend(&mutexPrint, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		xil_printf("Nb d'avions en attente d'atterrissage de type High : %d\n", longueurFile(Q_atterrissage_high));
		xil_printf("Nb d'avions en attente d'atterrissage de type Medium : %d\n", longueurFile(Q_atterrissage_medium));
		xil_printf("Nb d'avions en attente d'atterrissage de type Low : %d\n", longueurFile(Q_atterrissage_low));

		/*TODO: Obtenir statistiques pour la file de decollage*/
		xil_printf("Nb d'avions en attente de decollage : %d\n", longueurFile(Q_decollage));
		OSMutexPost(&mutexPrint, OS_OPT_POST_NONE,&err);

        /*TODO: Obtenir statut des terminaux*/
		print("Terminal 1 ");
        (longueurFile(Q_terminal_1) != 0) ? print("OCCUPE\n") : print("LIBRE\n");
 
        print("Terminal 2 ");
        (longueurFile(Q_terminal_2) != 0) ? print("OCCUPE\n") : print("LIBRE\n");
	}
}

int longueurFile(OS_Q q) {
	OS_ERR err;
	OS_Q_DATA donnee;
	OSQQuery(q, &donnee, &err);
	errMsg(err, "Error with query for Q_length");
	return donnee.OSNMsgs;
}

void verification(void* data){
	OS_ERR err;
	CPU_TS ts;
	print("[VERIFICATION] Tache lancee\n");
    while(1){
        /*TODO: Synchronisation unilaterale avec timer 3s*/
    	OSSemPend(&semVerification, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
    	errMsg(err, "Error with pending semVerification");

        if (stopSimDebordement){
            /*TODO: Suspension de toutes les taches de la simulation*/
        	OSTaskSuspend(&generationTCB, &err);
        	errMsg(err, "Error with generation task suspension.");

        	OSTaskSuspend(&atterrissageTCB, &err);
        	errMsg(err, "Error with landing task suspension.");

        	OSTaskSuspend(&terminal0TCB, &err);
			errMsg(err, "Error with terminal 1 task suspension.");

			OSTaskSuspend(&terminal1TCB, &err);
			errMsg(err, "Error with terminal 2 task suspension.");

			OSTaskSuspend(&decollageTCB, &err);
			errMsg(err, "Error with take off task suspension.");

			OSTaskSuspend(&statistiquesTCB, &err);
			errMsg(err, "Error with statistic task suspension.");

			OSTaskSuspend(&verificationTCB, &err);
			errMsg(err, "Error with verification task suspension.");
        }
    }
}
void remplirAvion(Avion* avion) {
	OS_ERR err;
	CPU_TS ts;
	srand(OSTimeGet(&err));
	avion->retard = rand() % BORNE_SUP_HIGH;
    avion->origine = rand() % NB_AEROPORTS;
    do { avion->destination = rand() % NB_AEROPORTS; } while (avion->origine == avion->destination);
    OSMutexPend(&mutexPrint, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
    xil_printf("Avion retard = %d\n", avion->retard);
    xil_printf("Avion origine = %d\n", avion->origine);
    xil_printf("Avion destination = %d\n", avion->destination);
    OSMutexPost(&mutexPrint, OS_OPT_POST_NONE, &err);
}

void errMsg(OS_ERR  err, char* errMsg){
	CPU_TS ts;
	if (err != OS_ERR_NONE){
		OSMutexPend(&mutexPrint, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
		xil_printf(errMsg);
		xil_printf("%d", err);
		OSMutexPost(&mutexPrint, OS_OPT_POST_NONE, &err);
		exit(1);
	}
}

void print(char* msg) {
	OS_ERR err;
	CPU_TS ts;
	OSMutexPend(&mutexPrint, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
	xil_printf(msg);
	OSMutexPost(&mutexPrint, OS_OPT_POST_NONE, &err);
}

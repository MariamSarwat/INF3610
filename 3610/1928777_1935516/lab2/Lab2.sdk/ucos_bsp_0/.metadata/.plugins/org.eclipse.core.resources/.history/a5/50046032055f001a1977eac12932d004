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



/**
* Ecrivez les routines d'interruption des deux fit_timer (celui qui s'execute a chaque seconde et celui qui s'execute a chaque 3 secondes) et du gpio
*/

void initialize_fit_1s() {

}

void initialize_fit_3s() {

}

void initialize_gpio() {
	bsp_init.initialize_gpio();
}

/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

int main(void)
{

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


	/**
	* TODO : Creer les differentes taches
	*/
	
	return 0;
}

int create_events() {
	OS_ERR err;


	/**
	* TODO : Creer les mutex, files, semaphores que vous utilisez dans ce devoir
	*/

	return 0;
}

/*
*********************************************************************************************************
*                                            TASK FUNCTIONS
*********************************************************************************************************
*/
void generation(void* data) {
	OS_ERR err;
	int nbAvionsCrees = 0;
	xil_printf("Tache generation lancee\n");
	int skipGen = 0;
	int seed = 34;
	while (DEF_ON) {

		/*TODO: Synchronisation unilaterale timer 1s*/
		srand(seed);
		skipGen = rand() % 5; //On saute la generation 1 fois sur 6
		if (skipGen != 0){
			Avion* avion = malloc(sizeof(Avion));
			avion->id = nbAvionsCrees;
			remplirAvion(avion);
			nbAvionsCrees++;

			/*TODO: Envoi des avions dans les files appropriees*/
		}
		else{
			//xil_printf("Generation avion a ete skippe!\n");
		}
		seed++;
	}
}

void atterrissage(void* data)
{
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE msg_size;

	Avion* avion = NULL;
]	xil_printf("Tache atterrissage lancee\n");
	while (DEF_ON) {
		/*TODO: Mise en attente des 3 files en fonction de leur priorité*/
		xil_printf("[ATTERRISSAGE] Debut atterrissage\n");
		OSTimeDly(150, OS_OPT_TIME_DLY, &err); //Temps pour que l'avion atterrisse
        xil_printf("[ATTERRISSAGE] Attente terminal libre\n");
		/*TODO: Mise en attente d'un terminal libre (mecanisme a votre choix)*/
        //xil_printf("[ATTERRISSAGE] Terminal libre num %d obtenu\n", ...);
 
        /*TODO: Envoi de l'avion au terminal choisi (mecanisme de votre choix)*/
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
	xil_printf("TERMINAL %d: Tache lancee\n", numTerminal);

	while (DEF_ON) {

		/*TODO: Mise en attente d'un avion venant de la piste d'atterrissage*/
		xil_printf("TERMINAL %d: Obtention avion\n", numTerminal);

		//Attente pour le vidage, le nettoyage et le remplissage de l'avion
		OSTimeDly(160, OS_OPT_TIME_DLY, &err);

		remplirAvion(avion);

		/*TODO: Envoi de l'avion pour le piste de decollage*/
		xil_printf("[TERMINAL %d] Liberation avion\n", numTerminal);
 
        /*TODO: Notifier que le terminal est libre (mecanisme de votre choix)*/
	}

}

void decollage(void* data)
{
	OS_ERR err;
	CPU_TS ts;
	OS_MSG_SIZE msg_size;

	Avion* avion = NULL;
	xil_printf("Tache decollage lancee\n");

	while (DEF_ON) {
		/*TODO: Mise en attente d'un avion pret pour le decollage*/
		OSTimeDly(30, OS_OPT_TIME_DLY, &err); //Temps pour que l'avion decolle
		xil_printf("DECOLLAGE: decollage avion\n");

        /*TODO: Destruction de l'avion*/
	}
}


void statistiques(void* data){
	OS_ERR err;
	CPU_TS ts;
    xil_printf("[STATISTIQUES] Tache lancee\n");

	while(DEF_ON){
        /*TODO: Synchronisation unilaterale switches*/
		xil_printf("\n------------------ Affichage des statistiques ------------------\n");

		/*TODO: Obtenir statistiques pour les files d'atterrissage*/
        /*xil_printf("Nb d'avions en attente d'atterrissage de type High : %d\n", ...);
        xil_printf("Nb d'avions en attente d'atterrissage de type Medium : %d\n", ...);
        xil_printf("Nb d'avions en attente d'atterrissage de type Low : %d\n", ...);*/
 
        /*TODO: Obtenir statistiques pour la file de decollage*/
        //xil_printf("Nb d'avions en attente de decollage : %d\n", ...);
 
        /*TODO: Obtenir statut des terminaux*/
        xil_printf("Terminal 0 ");
        int statutTerm0 = 0; /*A modifier (simplement un exemple d'affichage pour vous aider)*/
        (statutTerm0 == 0) ? xil_printf("OCCUPE\n") : xil_printf("LIBRE\n");
 
        xil_printf("Terminal 1 ");
        int statutTerm1 = 0; /*A modifier (simplement un exemple d'affichage pour vous aider)*/
        (statutTerm1 == 0) ? xil_printf("OCCUPE\n") : xil_printf("LIBRE\n");
	}
}

void verification(void* data){
	OS_ERR err;
	CPU_TS ts;
	xil_printf("[VERIFICATION] Tache lancee\n");
    while(1){
        /*TODO: Synchronisation unilaterale avec timer 3s*/
        if (stopSimDebordement){
            /*TODO: Suspension de toutes les taches de la simulation*/
        }
    }
}
void remplirAvion(Avion* avion) {
	OS_ERR err;

	srand(OSTimeGet(&err));
	 avion->retard = rand() % BORNE_SUP_HIGH;
    avion->origine = rand() % NB_AEROPORTS;
    do { avion->destination = rand() % NB_AEROPORTS; } while (avion->origine == avion->destination);
    /*xil_printf("Avion retard = %d\n", avion->retard);
    xil_printf("Avion origine = %d\n", avion->origine);
    xil_printf("Avion destination = %d\n", avion->destination);*/
}


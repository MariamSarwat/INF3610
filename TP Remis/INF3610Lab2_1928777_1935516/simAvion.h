/*
 * tourControleAvions.h
 *
 *  Created on: 14 sept. 2018
 *     Author: evter
 */

#ifndef SRC_SIMAVION_H_
#define SRC_SIMAVION_H_

#include <os.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define TASK_STK_SIZE       16384            // Size of each task's stacks (# of WORDs)

#define	FIT_TIMER_1S_PRIO	20
#define	FIT_TIMER_3S_PRIO	21
#define	GPIO_ISR_PRIO		22
#define	VERIFICATION_PRIO	23				// Defining Priority of each task
#define	GENERATION_PRIO		24
#define	STATISTIQUES_PRIO	25
#define	ATTERRISSAGE_PRIO	26
#define	TERMINAL0_PRIO		27
#define	TERMINAL1_PRIO		28
#define	DECOLLAGE_PRIO		29

//Intervalles criteres de retard
#define BORNE_INF_LOW      	0
#define BORNE_SUP_LOW      	19
#define BORNE_INF_MEDIUM   	20
#define BORNE_SUP_MEDIUM  	39
#define BORNE_INF_HIGH     	40
#define BORNE_SUP_HIGH      60


#define TERMINAL_0_READY 0x10
#define TERMINAL_1_READY 0x01
#define TERMINALS_READY 0x11
/*
*********************************************************************************************************
*                                             VARIABLES
*********************************************************************************************************
*/

static OS_TCB generationTCB;
static OS_TCB atterrissageTCB;
static OS_TCB terminal0TCB;
static OS_TCB terminal1TCB;
static OS_TCB decollageTCB;
static OS_TCB statistiquesTCB;
static OS_TCB verificationTCB;
static OS_TCB fitTimer1sTCB;
static OS_TCB fitTimer3sTCB;
static OS_TCB gpioIsrTCB;
/*
*********************************************************************************************************
*                                               QUEUES
*********************************************************************************************************
*/

OS_Q Q_atterrissage_high;
OS_Q Q_atterrissage_medium;
OS_Q Q_atterrissage_low;
OS_Q Q_decollage;
OS_Q Q_terminal_0;
OS_Q Q_terminal_1;

/*
*********************************************************************************************************
*                                              FLAGS
*********************************************************************************************************
*/
OS_FLAG_GRP terminal_status;
/*

*********************************************************************************************************
*                                              SEMAPHORES
*********************************************************************************************************
*/

OS_SEM semGeneration;
OS_SEM semVerification;
OS_SEM semStatistiques;

OS_MUTEX mutexPrint;
OS_MUTEX mutexDecollage;
OS_MUTEX safe;

/*
*********************************************************************************************************
*                                             ENUMERATIONS
*********************************************************************************************************
*/

enum Aeroport { YUL, YYZ, YVR, PEK, DBX, LGA, HND, LAX, CDG, AMS, NB_AEROPORTS };

/*
*********************************************************************************************************
*                                             STRUCTURES
*********************************************************************************************************
*/

typedef struct Avion {
	int id;
	int retard;
	enum Aeroport origine;
	enum Aeroport destination;
} Avion;

/*
********************************************************************************************************
*                                             SHARED VAIRABLES
*********************************************************************************************************
*/
bool stopSimDebordement;
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void create_application();
int create_tasks();
int create_events();

void generation(void *data);
void atterrissage(void *data);
void terminal(void *data);
void decollage(void *data);
void remplirAvion(Avion* avion);
void statistiques(void *data);
void verification(void* data);
void errMsg(OS_ERR err, char* errMSg);
void safePrint(char* msg);
void fit_timer_1s(void* not_valid);
void fit_timer_3s(void* not_valid);
void gpio_isr(void* data);
#endif /* SRC_SIMAVION_H_ */

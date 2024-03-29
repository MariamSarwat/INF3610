/*
 * tourControleAvions.h
 *
 *  Created on: 14 sept. 2018
 *      Author: evter
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

#define	VERIFICATION_PRIO	3				// Defining Priority of each task
#define	GENERATION_PRIO		4
#define	STATISTIQUES_PRIO	5
#define	ATTERRISSAGE_PRIO	6
#define	TERMINAL0_PRIO		7
#define	TERMINAL1_PRIO		8
#define	DECOLLAGE_PRIO		9

//Intervalles criteres de retard
#define BORNE_INF_LOW      	0
#define BORNE_SUP_LOW      	2
#define BORNE_INF_MEDIUM   	3
#define BORNE_SUP_MEDIUM  	5
#define BORNE_INF_HIGH     	6
#define BORNE_SUP_HIGH     	8
/*
*********************************************************************************************************
*                                             VARIABLES
*********************************************************************************************************
*/
CPU_STK           generationStk[TASK_STK_SIZE]; //Stack of each task
CPU_STK           decollageStk[TASK_STK_SIZE]; //Stack of each task
CPU_STK           atterrissageStk[TASK_STK_SIZE];
CPU_STK           terminal0Stk[TASK_STK_SIZE];
CPU_STK           terminal1Stk[TASK_STK_SIZE];
CPU_STK           terminal2Stk[TASK_STK_SIZE];
CPU_STK           terminal3Stk[TASK_STK_SIZE];

OS_TCB generationTCB;
OS_TCB atterrissageTCB;
OS_TCB terminal0TCB;
OS_TCB terminal1TCB;
OS_TCB decollageTCB;
OS_TCB statistiquesTCB;
OS_TCB verificationTCB;

/*
*********************************************************************************************************
*                                               QUEUES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              FLAGS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              SEMAPHORES
*********************************************************************************************************
*/



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
*********************************************************************************************************
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

void	generation(void *data);
void	atterrissage(void *data);
void    terminal(void *data);
void    decollage(void *data);
void	remplirAvion(Avion* avion);
void	statistiques(void *data);
void 	verification(void* data);



#endif /* SRC_SIMAVION_H_ */

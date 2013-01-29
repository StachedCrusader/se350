/**
 * @file:   process.h
 * @brief:  process management hearder file
 * @author: Irene Huang
 * @author: Thomas Reidemeister
 * @date:   2013/01/06
 * NOTE: Assuming there are only two user processes in the system
 */

#ifndef _PROCESS_H_
#define _PROCESS_H_

#ifdef DEBUG_0
#define USR_SZ_STACK 0x200         /* user proc stack size 2048B = 0x200*4 */
#else
#define USR_SZ_STACK 0x080         /* user proc stack size 512B  = 0x80*4  */
#endif /* DEBUG_0 */

#define NULL 0
#define INITIAL_xPSR 0x01000000    /* user process initial xPSR value */
#define CODE_NULL_PROCESS_FIXED 1000
#define CODE_NULL_PROCESS_PRESET 1001
#define CODE_BAD_PRIORITY 1002
#define CODE_BAD_ID 1003

#include <stdint.h>

/* process states, note we only assume three states in this example */
typedef enum {NEW = 0, RDY, RUN, BLCK} proc_state_t;  

/*
  PCB data structure definition.
  You may want to add your own member variables
  in order to finish P1 and the entire project 
*/
typedef struct pcb { 
  //struct pcb *mp_next;     /* next pcb, not used in this example, RTX project most likely will need it, keep here for reference */  
  uint32_t *mp_sp;         /* stack pointer of the process */
  uint32_t m_pid;          /* process id */
	uint32_t m_priority;			//Process Priority
  proc_state_t m_state;    /* state of the process */	
} pcb_t;

//QUEUE CODE DECLS

typedef struct PQ
{
        int size;
        pcb_t *procs[15];
} process_PQ;

void pq_insert(process_PQ *, pcb_t *);

pcb_t *pq_removeTop(process_PQ *);

pcb_t *pq_peekTop(process_PQ *);

void pq_heap(process_PQ *, int);

int pq_size(process_PQ *);

void bubble_up(process_PQ *);

int linearSearch(process_PQ *, int);

//QUEUE CODE DECLS ENDS

/*
  NOTE the example code uses compile time memory for stack allocation
  This makes the image size quite large. 
  The project requires you to use dynamically allocated memory for
  stack operation. The focus of the example code is for context switching,
  so we use statically allocated stack to simplify the code.
*/

uint32_t stackNull[USR_SZ_STACK];     /* stack for proc2 */

uint32_t stack1[USR_SZ_STACK];     /* stack for proc1 */
uint32_t stack2[USR_SZ_STACK];     /* stack for proc2 */
uint32_t stack3[USR_SZ_STACK];     /* stack for proc3 */
uint32_t stack4[USR_SZ_STACK];     /* stack for proc4 */

process_PQ * pq;

pcb_t *pcbNull;
pcb_t *pcb1;
pcb_t *pcb2;

pcb_t *pcb3;
pcb_t *pcb4;

pcb_t *pcbTest1;
pcb_t *pcbTest2;
pcb_t *pcbTest3;
pcb_t *pcbTest4;
pcb_t *pcbTest5;
pcb_t *pcbTest6;

pcb_t  *gp_current_process = NULL; /* always point to the current process */

extern void process_init(void);    /* initialize all procs in the system */
int scheduler(void);               /* pick the pid of the next to run process */
int k_release_process(void);       /* kernel release_process function */

extern void proc1(void);           /* user process 1 */
extern void proc2(void);           /* user process 2 */
extern void proc3(void);           /* user process 3 */
extern void proc4(void);           /* user process 4 */

extern void procTestRequestMemoryBlock(void);           
extern void procTestReleaseMemoryBlock(void);           
extern void procTestSomething(void);

extern void procNULL(void);           /* user process NULL */
extern void procA(void);           /* user process  */

extern void __rte(void);           /* pop exception stack frame */

int set_process_priority(int process_ID, int priority);
int get_process_priority(int process_ID);

#endif /* ! _PROCESS_H_ */

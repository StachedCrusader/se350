/**
 * @file:  process.c
 * @brief: process management source file
 * @author Irene Huang
 * @author Thomas Reidemeister
 * @date   2013/01/12
 * NOTE: The example code shows one way of context switching implmentation.
 *       The code only has minimal sanity check. There is no stack overflow check.
 *       The implementation assumes only two simple user processes, NO external interrupts. 
 *  	   The purpose is to show how context switch could be done under stated assumptions. 
 *       These assumptions are not true in the required RTX Project!!!
 *       If you decide to use this piece of code, you need to understand the assumptions and
 *       the limitations. Some part of the code is not written in the most efficient way.
 */

#include <LPC17xx.h>
#include "uart_polling.h"
#include "process.h"

#ifdef DEBUG_0
#include <stdio.h>
#endif  /* DEBUG_0 */

/**
 * @biref: initialize all processes in the system
 * NOTE: We assume there are only two user processes in the system in this example.
 *       We should have used an array or linked list pcbs so the repetive coding
 *       can be eliminated.
 *       We should have also used an initialization table which contains the entry
 *       points of each process so that this code does not have to hard code
 *       proc1 and proc2 symbols. We leave all these imperfections as excercies to the reader 
 */
 
void process_init() 
{
  volatile int i;
	uint32_t * sp;
	
	pq = (process_PQ *)s_request_memory_block();
	
	//TEST PROCESSSES
	pcbTest1 = (pcb_t *)s_request_memory_block();
	
	pcbTest1->m_pid = 1;
	pcbTest1->m_priority = 1;
	
	pcbTest1->m_state = NEW;
	
	sp  = stack1 + USR_SZ_STACK;
    
	/* 8 bytes alignement adjustment to exception stack frame */
	if (!(((uint32_t)sp) & 0x04)) {
	    --sp; 
	}
	
	*(--sp)  = INITIAL_xPSR;      /* user process initial xPSR */ 
	*(--sp)  = (uint32_t) procTestRequestMemoryBlock;  /* PC contains the entry point of the process */

	for (i = 0; i < 6; i++) { /* R0-R3, R12 are cleared with 0 */
		*(--sp) = 0x0;
	}
    pcbTest1->mp_sp = sp;
	
	pcbTest2 = (pcb_t *)s_request_memory_block();
	
	pcbTest2->m_pid = 1;
	pcbTest2->m_priority = 1;
	
	pcbTest2->m_state = NEW;
	
	sp  = stack2 + USR_SZ_STACK;
    
	/* 8 bytes alignement adjustment to exception stack frame */
	if (!(((uint32_t)sp) & 0x04)) {
	    --sp; 
	}
	
	*(--sp)  = INITIAL_xPSR;      /* user process initial xPSR */ 
	*(--sp)  = (uint32_t) procTestReleaseMemoryBlock;  /* PC contains the entry point of the process */

	for (i = 0; i < 6; i++) { /* R0-R3, R12 are cleared with 0 */
		*(--sp) = 0x0;
	}
    pcbTest2->mp_sp = sp;
	
	pcbTest3 = (pcb_t *)s_request_memory_block();
	
	pcbTest3->m_pid = 1;
	pcbTest3->m_priority = 1;
	
	pcbTest3->m_state = NEW;
	
	sp  = stack3 + USR_SZ_STACK;
    
	/* 8 bytes alignement adjustment to exception stack frame */
	if (!(((uint32_t)sp) & 0x04)) {
	    --sp; 
	}
	
	*(--sp)  = INITIAL_xPSR;      /* user process initial xPSR */ 
	*(--sp)  = (uint32_t) procTestSomething;  /* PC contains the entry point of the process */

	for (i = 0; i < 6; i++) { /* R0-R3, R12 are cleared with 0 */
		*(--sp) = 0x0;
	}
    pcbTest3->mp_sp = sp;
	
	
	pq_insert(pq, pcbTest1);
	pq_insert(pq, pcbTest2);
	pq_insert(pq, pcbTest3);
}

/*@brief: scheduler, pick the pid of the next to run process
 *@return: pid of the next to run process
 *         -1 if error happens
 *POST: if gp_current_process was NULL, then it gets set to &pcb1.
 *      No other effect on other global variables.
 */
int scheduler(void)
{
  volatile int pid;
	pcb_t *top;
	
	if (gp_current_process == NULL) {
	  gp_current_process = pq_peekTop(pq);
	}

	return pq_peekTop(pq)->m_pid;
}
/**
 * @brief release_processor(). 
 * @return -1 on error and zero on success
 * POST: gp_current_process gets updated
 */
int k_release_processor(void)
{
	 volatile int pid;
	 volatile proc_state_t state;
	 pcb_t *p_pcb_old = NULL;

	 pid = scheduler();
	 if (gp_current_process == NULL) {
	   return -1;  
	 }
	 
	 p_pcb_old = gp_current_process;

	 gp_current_process = pq->procs[linearSearch(pq, pid)];
	 
	 pq_removeTop(pq);
	 
	 state = gp_current_process->m_state;

    if (state == NEW) 
		{
	     if (p_pcb_old->m_state != NEW) 
			 {
					p_pcb_old->m_state = RDY;
					p_pcb_old->mp_sp = (uint32_t *) __get_MSP();
			 }
		 gp_current_process->m_state = RUN;
		 //RE-INSERT PCB OLD into PQ
		 pq_insert(pq, p_pcb_old);
		 __set_MSP((uint32_t) gp_current_process->mp_sp);
		 __rte();  /* pop exception stack frame from the stack for a new process */
	 } 
	 else if (state == RDY)
	 {     
			p_pcb_old->m_state = RDY; 
			p_pcb_old->mp_sp = (uint32_t *) __get_MSP(); /* save the old process's sp */
			gp_current_process->m_state = RUN;
			//RE-INSERT PCB OLD into PQ
			pq_insert(pq, p_pcb_old);
			__set_MSP((uint32_t) gp_current_process->mp_sp); /* switch to the new proc's stack */		
	 }
	 else 
	 {
	     gp_current_process = p_pcb_old; /* revert back to the old proc on error */
	     return -1;
	 }	 	 
	 return 0;
}

int get_process_priority(int process_ID) {

	//Grab from the PQ the process' priority of pid == process_ID
	int i;
	for (i = 0; i < pq->size; i++)
	{
		if (pq->procs[i]->m_pid == process_ID)
			return pq->procs[i]->m_priority;
	}
	
	return -1; // ERROR
}



int set_process_priority(int process_ID, int priority) {
	
	int index = linearSearch(pq, process_ID);
	
	if(index != -1){ //IF EXISTS
		if(process_ID == 0){ //null process should always be priority of 4
			return CODE_NULL_PROCESS_FIXED;
		}
		else{
			if(priority < 4 && priority >= 0){
				//set mapping of process_ID to new priority
				pq->procs[index]->m_priority = priority;
			}
			else
			{
				return CODE_BAD_PRIORITY;
			}
		}
	}
	return CODE_BAD_ID;
}

//QUEUE CODE STARTS HERE

void pq_insert(process_PQ *queue, pcb_t *p)
{
  queue->size += 1;
  queue->procs[queue->size - 1] = p;
  bubble_up(queue);
}

void bubble_up(process_PQ *queue)
{
  int i = queue->size - 1;
  int parent = ((i-1) / 2);

  while (i > 0 && queue->procs[i]->m_priority
                  < queue->procs[parent]->m_priority)
   {
                pcb_t * temp = queue->procs[parent];
                queue->procs[parent] = queue->procs[i];
                queue->procs[i] = temp;

                i = parent;
                parent = ((i - 1) / 2);
  }
}

pcb_t *pq_peekTop(process_PQ *queue)
{
  return queue->procs[0];
}

int pq_size(process_PQ *queue)
{
  return queue->size;
}

void pq_heap(process_PQ *queue, int index)
{
  int left = (2*index) + 1;
  int right = (2*index) + 2;

  if (left >= queue->size)
    return;
  if (right >= queue->size)
  {
    pcb_t * temp = queue->procs[left];
    queue->procs[left] = queue->procs[index];
    queue->procs[index] = temp;

    pq_heap(queue, left);
  }
  else if (queue->procs[left]->m_priority < queue->procs[index]->m_priority
    && queue->procs[left]->m_priority < queue->procs[right]->m_priority)
  {
    pcb_t * temp = queue->procs[left];
    queue->procs[left] = queue->procs[index];
    queue->procs[index] = temp;

    pq_heap(queue, left);
  }
  else if (queue->procs[right]->m_priority < queue->procs[index]->m_priority
    && queue->procs[right]->m_priority < queue->procs[left]->m_priority)
  {
    pcb_t * temp = queue->procs[right];
    queue->procs[right] = queue->procs[index];
    queue->procs[index] = temp;

    pq_heap(queue, right);
  }
  return;
}

pcb_t *pq_removeTop(process_PQ *queue)
{
	int last = queue->size - 1;
	pcb_t * temp = queue->procs[0];
  if (queue->size == 0)
    return 0;
  
  
  queue->procs[0] = queue->procs[last];
  queue->procs[last] = temp;

  queue->size -= 1;

  pq_heap(queue, 0);

  return queue->procs[last];
}

int linearSearch(process_PQ *queue, int process_ID)
{
	int i;
	for (i = 0; i < pq->size; i++)
	{
		if (pq->procs[i]->m_pid == process_ID)
			return i;
	}
	
	return -1;
}

/*
#include "queue.h"

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
  pcb_t *node = queue->procs[index];
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
}*/

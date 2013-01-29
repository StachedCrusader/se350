/**
 *@file:  usr_proc.c
 *@brief: Two user processes proc1 and proc2
 *@author: Irene Huang
 *@date: 2013/01/12
 */

#include "rtx.h"
#include "uart_polling.h"
#include <stdio.h>

#ifdef DEBUG_0
//#include <stdio.h>
#endif  /* DEBUG_0 */

void procNull(void)
{
	volatile int ret_val = 0;
	while(1)
	{
		ret_val = release_processor();
	}
}

void procTestRequestMemoryBlock(void)
{
	volatile int i =0;
	volatile int ret_val = 0;
	
	
	//WRITE TEST CODE
	//ONCE TEST CODE IS COMPLETE
	//SWITCH CONTEXT
	
	int *firstBlock = (int *)s_request_memory_block();
	int *secondBlock = (int *)s_request_memory_block();
	
	*firstBlock = 4;
	*secondBlock = 4;
	
	if (firstBlock == secondBlock)
	{
		uart0_put_string("PROBLEM -- firstblock is equal to secondblock\n");
	}
	else
	{
		uart0_put_string("NOT A PROBLEM -- firstblock is not equal to secondblock\n");
	}
	
	if (*firstBlock == *secondBlock)
	{
		uart0_put_string("NOT A PROBLEM -- the values of first block and second block are the same\n");
	}
	
	if ((firstBlock + 32) == (secondBlock))
	{
		uart0_put_string("NOT A PROBLEM -- firstblock + 32 (128 bits) is second block\n");
	}
	
	ret_val = release_processor();
}


void procTestReleaseMemoryBlock(void)
{
	volatile int ret_val = 0;
	volatile int response = 1;
	
	int *firstBlock = (int *)s_request_memory_block();
	int *secondBlock = (int *)s_request_memory_block();
	int *thirdBlock;
	
	//sanity check
	if (firstBlock == secondBlock)
	{
		uart0_put_string("PROBLEM -- firstblock is equal to secondblock\n");
	}
	response = s_release_memory_block(secondBlock);
	
	//test deletion works
	if (response == 0)
	{
		uart0_put_string("NOT A PROBLEM -- successfull deletion\n");
	}
	else
	{
		uart0_put_string("PROBLEM -- not successfull deleted\n");
	}
	
	thirdBlock = (int *)s_request_memory_block();
	if ((firstBlock + 32) == (thirdBlock))
	{
		uart0_put_string("NOT A PROBLEM -- firstblock + 32 (128 bits) is third block\n");
	}
	
	
	
	ret_val = release_processor();
}

void procTestSomething(void)
{
	volatile int ret_val = 0;
	
	//while (1)
	{
		uart0_put_string("Test3\n");
		ret_val = release_processor();
	}
}

void proc1(void)
{
  volatile int i =0;
  volatile int ret_val = 10;
  while ( 1) {
    if (i!=0 &&i%1 == 0 ) {
      ret_val = release_processor();
#ifdef DEBUG_0
      printf("\n\rproc1: ret_val=%d. ", ret_val);
#else
      uart0_put_string("\n\r");
#endif /* DEBUG_0 */
    }
		uart0_put_string("ABCDE");
    //uart0_put_char('A' + i%26);
    i++;
  }

}

void proc2(void)
{
  volatile int i =0;
  volatile int ret_val = 20;
  while ( 1) {
    if (i!=0 &&i%1 == 0 ) {
      ret_val = release_processor();
#ifdef DEBUG_0
      printf("\n\rproc2: ret_val=%d. ", ret_val);
#else
      uart0_put_string("\n\r");
#endif  /* DEBUG_0 */
    }
    //uart0_put_char('a' + i%26);
		uart0_put_string("abcde");
    i++;
  }
}

void proc3(void)
{
  volatile int i =0;
  volatile int ret_val = 20;
  while ( 1) {
    if (i!=0 &&i%1 == 0 ) {
      ret_val = release_processor();
#ifdef DEBUG_0
      printf("\n\rproc2: ret_val=%d. ", ret_val);
#else
      uart0_put_string("\n\r");
#endif  /* DEBUG_0 */
    }
    //uart0_put_char('a' + i%26);
		uart0_put_string("12345");
    i++;
  }
}

void proc4(void)
{
  volatile int i =0;
  volatile int ret_val = 20;
  while ( 1) {
    if (i!=0 &&i%1 == 0 ) {
      ret_val = release_processor();
#ifdef DEBUG_0
      printf("\n\rproc2: ret_val=%d. ", ret_val);
#else
      uart0_put_string("\n\r");
#endif  /* DEBUG_0 */
    }
    //uart0_put_char('a' + i%26);
		uart0_put_string("!!!!!");
    i++;
  }
}


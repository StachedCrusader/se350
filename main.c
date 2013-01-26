#include <LPC17xx.h>
#include "uart_polling.h"
#include "rtx.h"

#define START_ADDRESS 0x10004000
#define END_ADDRESS   0x10008000
#define SIZE 128

#define CODE_FAIL 1
#define CODE_SUCCESS 0

struct Node
{
	int address;
	int released;
};

struct Node mem_map[SIZE];

void init_memory()
{
	//explicitely make all the values 0;
	//int size = (START_ADDRESS - END_ADDRESS) / 128;

	//struct Node temp[SIZE];
	int i = 0;
	int address = START_ADDRESS;
	for (i = 0; i < SIZE; i++)
	{
		//temp[i] = 0;
		mem_map[i].address = address;
		mem_map[i].released = 0;
		address += 128;
	}
}

void * s_request_memory_block()
{
	int i = 0;
	void *temp;
	for (i = 0; i < SIZE; i++)
	{
		if (mem_map[i].released == 0)
		{
			temp = (void *)mem_map[i].address;
			mem_map[i].released = 1;
			return temp;
		}
	}
	return 0;
}

int s_release_memory_block(void* memory_block)
{	

/*
	if (memory block pointer is not valid)
	return ErrorCode;
	Add memory block to the pool;
	Update datastructure;
	returen SuccessCode;
*/
	int valid = 1;
	int i = 0;	
	//valid = CHECK IF memory_block IS VALID
	if (valid != 1){
		return CODE_FAIL;
	}	
	for (i = 0; i < SIZE; i++)
	{
	if ( (int*)mem_map[i].address == memory_block){	
		mem_map[i].released = 0;
		//Set data to zero?
		return CODE_SUCCESS;
		}
	}	
	return CODE_FAIL; 
}

 int main() {
	int * hello;
	int * hello2;
	int * hello3;
	volatile unsigned int ret_val = 1234; 
	 
	SystemInit();
	__disable_irq();
	uart0_init();
	init_memory();
	process_init();
	__enable_irq();
	
  //SET Control to user level
	__set_CONTROL(__get_CONTROL() | BIT(0));
	 
	hello = s_request_memory_block();
	hello2 = s_request_memory_block();
	s_release_memory_block(hello);
	s_release_memory_block(hello2);

	hello3 = s_request_memory_block();
	 
	*hello3 = 88; 
	 
	uart0_put_string("Hello World!\n\r");	
	
	ret_val = release_processor();
	 
	return 0;
}

/*
int set_process_priority(int process_ID, int priority) {
if(processExists(process_ID)){ //process already exists
if(process_ID == 0){ //null process should always be priority of 4
return CODE_NULL_PROCESS_FIXED;
}
else{
if(priority == 0 || priority == 1 || priority == 2 || priority == 3){
//set mapping of process_ID to new priority
}
}
else{ //process does not exist
if(process_ID == 0){ //handle null process
//create mapping of process_ID to 4
if(priortity == 4){
return 0;
}
else{
return CODE_NULL_PROCESS_PRESET;
}
}
else{ //normal processes
if(priority == 0 || priority == 1 || priority == 2 || priority == 3){
//create mapping of process_ID to priority
}
else{
return CODE_BAD_PRIORITY;
}
}
}

int get_process_priority(int process_ID) {
//code here
}*/

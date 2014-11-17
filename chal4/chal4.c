#ifndef CTF_THREADS
#define CTF_THREADS
#endif
#include <sys/ptrace.h>
#include <signal.h>
#include <string.h>
#include "ctfserver.h"
#define BREAKS 5

extern char __executable_start;
extern char __etext;

void handler(void *pSock);
void anti_debug();

int main()
{
    anti_debug();
    if (!ctfserver(handler)) return 1;
    sleep(3);
    printf("done!\n");
}

void handler(void *pSock){
    char yolo[8];
    yolo[0] = 'y';
    sock rsock = *((sock *)pSock);
    yolo[1] = 'o';
    char rBuf[BUFSIZE];
    anti_debug();
    yolo[2] = 'l';
    if (!rprintf(rsock, "Don't debug me, bro.\n")) pthread_exit(NULL);
    yolo[5] = yolo[1];
    sleep(2);
    yolo[4] = yolo[5];
    if (!rprintf(rsock, "Well, since you're here... Whats my motto? ")) pthread_exit(NULL);
    yolo[3] = yolo[4];
    if (!rgets(rsock, rBuf)) pthread_exit(NULL);
    yolo[6] = '!';
    char *out = strtok(rBuf, "\n");
    yolo[7] = '\0';
    if (out == NULL) pthread_exit(NULL);

    if (strstr(rBuf, yolo) != NULL){
        if (!send_flag(rsock, "Nice... ")){
            close(rsock);
            pthread_exit(NULL);
        }
    }else{
        if (!rprintf(rsock, "Pffftt... NO. I don't live that.\n")){
            close(rsock);
            pthread_exit(NULL);
        }
    }
    close(rsock);
}

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
    int count = 0;
    for (size_t i = 0; i != size; i++){
        if (ch[i] == bp1 || ch[i] == bp2){
            count++;
            if (count > BREAKS){
                printf("Breakpoint detected. @0x%lx: 0x%x\nAborting.\n", (unsigned long)&ch[i], ch[i]);
                raise(SIGSEGV);
            }
        }
    }
}

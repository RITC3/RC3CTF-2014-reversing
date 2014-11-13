#include "ctfserver.h"

void handler(void * pSock);

int main()
{
    if (!ctfserver(handler)) return 1;
    return 0;
}

void handler(void * pSock){
    sock rsock = *((sock*)pSock);
    char sBuf[BUFSIZE];
    char rBuf[BUFSIZE];
    memset(rBuf, '\0', BUFSIZE);
    memset(sBuf, '\0', BUFSIZE);
    if (!rprintf(rsock, "What is a stack but a collection of things ready to be smashed ;)\nGive it to me babby: ")) return;
    if (!rgets(rsock, rBuf)) return;
    if (!rprintf(rsock, "\n")) return;
    volatile unsigned int q = 0;
    volatile unsigned int i = 0;
    char buf[512];
    strcpy(buf, rBuf);

    if (i == 0xDDAAAADD){
        if (!rprintf(rsock, "Almost")) return;
        for (int i = 0; i < 5; ++i){
            sleep(1);
            if (!rprintf(rsock, ".")) return;
        }
        if (!rprintf(rsock, "\n")) return;

        if (q == 0xDDEEEEDD){
            send_flag(rsock, "You win! Here is your flag: ");
            return;
        } else
            if (!rprintf(rsock, "No.\n")) return;
    }else
        if (!rprintf(rsock, "No.\n")) return;
    close(rsock);
}

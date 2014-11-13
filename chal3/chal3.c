#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include <stdarg.h>

typedef int sock;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

#define BUFSIZE 1028
#define PORT 12345
#define MAX_CONNECTIONS 30

sock gsock;

void handler(sock rsock);
void seg_handler(int signal);
bool rputs(sock rsock, char *fmt, ...);
bool rgets(sock rsock, char *rBuf);

int main()
{
    sock rsock, lsock;
    sockaddr_in lsin, rsin;
    memset(&lsin, 0, sizeof(sockaddr_in));
    memset(&rsin, 0, sizeof(sockaddr_in));

    lsin.sin_family = AF_INET;
    lsin.sin_port = htons(PORT);
    lsin.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((lsock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        return 1;
    }
    if (bind(lsock, (sockaddr *)&lsin, sizeof(lsin)) == -1){
        perror("bind");
        return 1;
    }
    if (listen(lsock, MAX_CONNECTIONS) == -1){
        perror("listen");
        return 1;
    }

    socklen_t rsin_len = sizeof(rsin);
    pid_t pid;
    while (1){
        rsock = accept(lsock, (sockaddr *)&rsin, &rsin_len);
        if (rsock != -1){
            pid = fork();
            if (!pid){
                handler(rsock);
                close(rsock);
                return 0;
            }else
                close(rsock);
        }
    }
    close(lsock);
    return 0;
}

void seg_handler(int signal){
    rputs(gsock, "Da done fucked up A. A. Ron: %d @ %x", signal);
    close(gsock);
    exit(1);
}

void handler(sock rsock){
    signal(SIGSEGV, seg_handler);
    char sBuf[BUFSIZE];
    char rBuf[BUFSIZE];
    memset(rBuf, '\0', BUFSIZE);
    memset(sBuf, '\0', BUFSIZE);
    gsock = rsock;
    if (!rputs(rsock, "What is a stack but a collection of things ready to be smashed ;)\nGive it to me babby: ")) return;
    if (!rgets(rsock, rBuf)) return;
    if (!rputs(rsock, "\n")) return;
    volatile unsigned int q = 0;
    volatile unsigned int i = 0;
    char buf[512];
    strcpy(buf, rBuf);

    if (i == 0xDDAAAADD){
        if (!rputs(rsock, "Almost")) return;
        for (int i = 0; i < 5; ++i){
            sleep(1);
            if (!rputs(rsock, ".")) return;
        }
        if (!rputs(rsock, "\n")) return;

        if (q == 0xDDEEEEDD){
            char fBuf[15];
            FILE *fp = fopen("flag.txt", "r");
            fgets(fBuf, 14, fp);
            fclose(fp);
            if (!rputs(rsock, "You win! Here is your flag: %s\n", fBuf)) return;
            return;
        } else
            if (!rputs(rsock, "No.\n")) return;
    }else
        if (!rputs(rsock, "No.\n")) return;
    close(rsock);
}

bool rputs(sock rsock, char *fmt, ...){
    char msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, BUFSIZE, fmt, args);
    va_end(args);
    if (send(rsock, msg, strlen(msg), 0) == -1){
        perror("send");
        return false;
    }
    return true;
}

bool rgets(sock rsock, char *rBuf){
    memset(rBuf, 0, BUFSIZE);
    if (recv(rsock, rBuf, BUFSIZE, 0) == -1){
        perror("recv");
        return false;
    }
    return true;
}

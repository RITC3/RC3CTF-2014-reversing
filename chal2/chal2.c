#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>

typedef int sock;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

#define BUFSIZE 1028
#define PORT 12345
#define MAX_CONNECTIONS 30

void *handler(void * pSock);

pthread_mutex_t tmutex;
sock gsock;

int main()
{
    sock rsock, lsock;
    sockaddr_in lsin, rsin;
    pthread_t pid;
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
    if ( pthread_mutex_init(&tmutex, NULL) != 0 ){
        perror("mutex init failed");
        close(lsock);
        return 1;
    }

    while (1){
        rsock = accept(lsock, (sockaddr *)&rsin, &rsin_len);
        pthread_mutex_lock(&tmutex);
        pthread_create(&pid, NULL, handler, (void *)&rsock);
        pthread_mutex_unlock(&tmutex);
    }

    return 0;
}

void sig_handler(){
    FILE *fp = fopen("flag.txt", "r");
    char sBuf[BUFSIZE];
    char fBuf[15];
    fgets(fBuf, 15, fp);
    sprintf(sBuf, "You've won Charlie!\n%s", fBuf);
    if (send(gsock, sBuf, strlen(sBuf), 0) == -1){
        perror("send");
        close(gsock);
        pthread_mutex_unlock(&tmutex);
        pthread_exit(NULL);
    }
    fclose(fp);
    close(gsock);
    pthread_mutex_unlock(&tmutex);
    pthread_exit(NULL);
}

void *handler(void *pSock){
    sock *rsock = (sock *)pSock;
    int int1, int2;
    char sBuf[BUFSIZE];
    char rBuf[BUFSIZE];
    memset(rBuf, '\0', BUFSIZE);
    memset(sBuf, '\0', BUFSIZE);
    signal(SIGFPE, sig_handler);

    pthread_mutex_lock(&tmutex);
    strncpy(sBuf, "Enter a number: ", BUFSIZE);
    if (send(*rsock, sBuf, strlen(sBuf), 0) == -1){
        perror("send");
        close(*rsock);
        pthread_mutex_unlock(&tmutex);
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&tmutex);

    if (recv(*rsock, rBuf, BUFSIZE, 0) == -1){
        perror("recv");
        close(*rsock);
        pthread_mutex_unlock(&tmutex);
        pthread_exit(NULL);
    }
    int1 = atoi(rBuf);

    pthread_mutex_lock(&tmutex);
    strncpy(sBuf, "Enter another number: ", BUFSIZE);
    if (send(*rsock, sBuf, strlen(sBuf), 0) == -1){
        perror("send");
        close(*rsock);
        pthread_mutex_unlock(&tmutex);
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&tmutex);

    if (recv(*rsock, rBuf, BUFSIZE, 0) == -1){
        perror("recv");
        close(*rsock);
        pthread_mutex_unlock(&tmutex);
        pthread_exit(NULL);
    }
    int2 = atoi(rBuf);

    pthread_mutex_lock(&tmutex);
    if (!int2){
        strncpy(sBuf, "No dividing by zero!!!\n", BUFSIZE);
        if (send(*rsock, sBuf, strlen(sBuf), 0) == -1){
            perror("send");
            close(*rsock);
            pthread_mutex_unlock(&tmutex);
            pthread_exit(NULL);
        }
    } else {
        gsock = *rsock;
        int ans = int1 / int2;
        sprintf(sBuf, "%d divided by %d equals: %d\n", int1, int2, ans);
        if (send(*rsock, sBuf, strlen(sBuf), 0) == -1){
            perror("send");
            close(*rsock);
            pthread_mutex_unlock(&tmutex);
            pthread_exit(NULL);
        }
    }

    pthread_mutex_unlock(&tmutex);
    close(*rsock);
    pthread_exit(NULL);
}


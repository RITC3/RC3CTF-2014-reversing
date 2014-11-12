#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 0;
    unsigned int i = 0;
    unsigned int q = 0;
    char buf[512];
    strcpy(buf, argv[1]);

    if (i == 0xDAADDAAD){
        puts("Almost...");
        sleep(5);
        if (q == 0xD00DD00D){
            puts("You win.\n");
            char fBuf[15];
            FILE *fp = fopen("flag.txt", "r");
            fgets(fBuf, 14, fp);
            fclose(fp);
            puts(fBuf);
        } else
            puts("No.\n");
        return 0;
    }
    puts("No.\n");
    return 0;
}

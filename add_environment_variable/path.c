#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>

extern char **environ;

char temp[10];
char cmd[200];
int main()
{
    char **env = environ;
    int compare = 0;
    int track = -1;
    do
    {
        track++;
        memset(temp, 0, 10);
        memcpy(temp, env[track], 3);
    } while (strcmp(temp, "PWD") != 0);
    sprintf(cmd, "export LD_LIBRARY_PATH=%s", env[track] + 4);
    system(cmd);
    printf("%s\n", cmd);
    return 0;
}

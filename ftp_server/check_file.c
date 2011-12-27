#include "server.h"

int is_dir(const char* filename)
{
    struct stat stat_buf;
    if (stat(filename, &stat_buf) < 0)
        return 0;

    if (S_ISDIR(stat_buf.st_mode))
        return 1;
    else
        return 0;
}

int check_file_for_send(const char* filename, char buf[])
{
    FILE* fp;

    if (is_dir(filename) == 1)
    {
        strcat(buf, "Is a directory\n");
        return -1;
    }

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        strcat(buf, sys_errlist[errno]);
        strcat(buf, "\n");
        return -1;
    }
    else
    {
        fclose(fp);
        strcat(buf, "You can transfer file now\n");
        return 0;
    }
}

int check_file_for_recv(const char* filename, char buf[])
{
    FILE* fp;

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        strcat(buf, sys_errlist[errno]);
        strcat(buf, "\n");
        return -1;
    }
    else
    {
        fclose(fp);
        strcat(buf, "You can transfer file now\n");
        return 0;
    }
}

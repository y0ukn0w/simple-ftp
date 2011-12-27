#include "client.h"

void cmd_analysize(char str_buf[], char cmd_buf[])
{
    int i, j;
    char cmd[TMP_BUF_SIZE];
    char* argv;

    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)cmd_buf;
    cmd_hdr->cmd_argc = 0;

    /** remove the tail newline character **/
    str_buf[strlen(str_buf)-1] = '\0';

    memset(cmd, 0, sizeof(cmd));
    for (i = 0; str_buf[i] != '\0' && (str_buf[i] == ' ' || str_buf[i] == '\t'); i++);

    for (j = 0; str_buf[i] != '\0' && str_buf[i] != ' ' && str_buf[i] != '\t';)
        cmd[j++] = str_buf[i++];

    argv = cmd_hdr->cmd_argv;
    while (1)
    {
        for (; str_buf[i] != '\0' && (str_buf[i] == ' ' || str_buf[i] == '\t'); i++);
        if (str_buf[i] == '\0')
            break;

        for (j = 0; str_buf[i] != '\0' && str_buf[i] != ' ' && str_buf[i] != '\t';)
            argv[j++] = str_buf[i++];
        cmd_hdr->cmd_argc++;
        argv += strlen(argv) + 1;
    }

    if (strcmp(cmd, "ls") == 0)
    {
        cmd_hdr->cmd_type = LS;
    }
    else if (strcmp(cmd, "mkdir") == 0)
    {
        cmd_hdr->cmd_type = MKDIR;
    }
    else if (strcmp(cmd, "rmdir") == 0)
    {
        cmd_hdr->cmd_type = RMDIR;
    }
    else if (strcmp(cmd, "pwd") == 0)
    {
        cmd_hdr->cmd_type = PWD;
    }
    else if (strcmp(cmd, "cd") == 0)
    {
        cmd_hdr->cmd_type = CD;
    }
    else if (strcmp(cmd, "rm") == 0)
    {
        cmd_hdr->cmd_type = RM;
    }
    else if (strcmp(cmd, "put") == 0)
    {
        cmd_hdr->cmd_type = PUT;
    }
    else if (strcmp(cmd, "mput") == 0)
    {
        cmd_hdr->cmd_type = MPUT;

    }
    else if (strcmp(cmd, "get") == 0)
    {
         cmd_hdr->cmd_type = GET;
    }
    else if (strcmp(cmd, "mget") == 0)
    {
         cmd_hdr->cmd_type = MGET;
    }
    else if (strcmp(cmd, "q") == 0)
    {
        cmd_hdr->cmd_type = QUIT;
    }
    else if (strcmp(cmd, "") == 0)
    {
        cmd_hdr->cmd_type = DUMMY;
    }
    else
    {
        cmd_hdr->cmd_type = NOT_SUPPORTED;
    }

}

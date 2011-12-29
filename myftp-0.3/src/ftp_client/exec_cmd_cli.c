#include "client.h"

int exec_cmd_cli(char input_buf[], char recv_buf[], char send_buf[], char file_buf[],
                 int server_sockfd, int file_sockfd)
{
    int i, j;
    char cmd[BUF_SIZE];
    char* argv;

    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)send_buf;
    cmd_hdr->cmd_argc = 0;

    /** remove the tail newline character **/
    input_buf[strlen(input_buf)-1] = '\0';

    memset(cmd, 0, sizeof(cmd));
    for (i = 0; input_buf[i] != '\0' && (input_buf[i] == ' ' || input_buf[i] == '\t'); i++);

    /** get command name **/
    for (j = 0; input_buf[i] != '\0' && input_buf[i] != ' ' && input_buf[i] != '\t';)
        cmd[j++] = input_buf[i++];

    /** get command argument, may be more than one **/
    argv = cmd_hdr->cmd_argv;
    while (1)
    {
        for (; input_buf[i] != '\0' && (input_buf[i] == ' ' || input_buf[i] == '\t'); i++);
        if (input_buf[i] == '\0')
            break;

        for (j = 0; input_buf[i] != '\0' && input_buf[i] != ' ' && input_buf[i] != '\t';)
            argv[j++] = input_buf[i++];
        cmd_hdr->cmd_argc++;
        argv += strlen(argv) + 1;
    }

    /** server command **/
    if (strcmp(cmd, "ls") == 0)
    {
        cmd_hdr->cmd_type = LS;
        return 1;
    }
    else if (strcmp(cmd, "pwd") == 0)
    {
        cmd_hdr->cmd_type = PWD;
        return 1;
    }
    else if (strcmp(cmd, "cd") == 0)
    {
        cmd_hdr->cmd_type = CD;
        return 1;
    }
    else if (strcmp(cmd, "rm") == 0)
    {
        cmd_hdr->cmd_type = RM;
        return 1;
    }
    else if (strcmp(cmd, "mkdir") == 0)
    {
        cmd_hdr->cmd_type = MKDIR;
        return 1;
    }
    else if (strcmp(cmd, "rmdir") == 0)
    {
        cmd_hdr->cmd_type = RMDIR;
        return 1;
    }
    else if (strcmp(cmd, "get") == 0)
    {
        cmd_hdr->cmd_type = GET;
        do_get_cli(recv_buf, send_buf, file_buf, server_sockfd, file_sockfd);
        return 0;
    }
    else if (strcmp(cmd, "mget") == 0)
    {
        do_mget_cli(recv_buf, send_buf, file_buf, server_sockfd, file_sockfd);
        return 0;
   }
    else if (strcmp(cmd, "put") == 0)
    {
        cmd_hdr->cmd_type = PUT;
        do_put_cli(recv_buf, send_buf, file_buf, server_sockfd, file_sockfd);
        return 0;
    }
    else if (strcmp(cmd, "mput") == 0)
    {
        do_mput_cli(recv_buf, send_buf, file_buf, server_sockfd, file_sockfd);
        return 0;
    }
    /** client command **/
    else if (strcmp(cmd, "dir") == 0)
    {
        struct stat stat_buf;
        char tmp_buf[BUF_SIZE];
        memset(tmp_buf, 0, sizeof(tmp_buf));

        if (cmd_hdr->cmd_argv[0] == '\0')
            strcpy(cmd_hdr->cmd_argv, ".");

        if (stat(cmd_hdr->cmd_argv, &stat_buf) < 0)
        {
            printf("ls: %s\n", sys_errlist[errno]);
        }
        else
        {
            if (S_ISDIR(stat_buf.st_mode))
                display_dir(cmd_hdr->cmd_argv, tmp_buf);
            else
                display_file(cmd_hdr->cmd_argv, cmd_hdr->cmd_argv, tmp_buf);
            printf("%s", tmp_buf);
        }
        return 0;
    }
    else if (strcmp(cmd, "lcd") == 0)
    {
        if (cmd_hdr->cmd_argv[0] == '\0')
            ;
        else
        {
            if (chdir(cmd_hdr->cmd_argv) < 0)
            {
                printf("cd: %s\n", sys_errlist[errno]);
            }
        }
        return 0;
    }
    else if (strcmp(cmd, "lpwd") == 0)
    {
        getcwd(send_buf, BUF_SIZE);
        printf("%s\n", send_buf);
        return 0;
    }
    else if (strcmp(cmd, "lmkdir") == 0)
    {
        if (cmd_hdr->cmd_argv[0] == '\0')
        {
            printf("mkdir: missing operand\n");
        }
        else
        {
            if (mkdir(cmd_hdr->cmd_argv, 0777) < 0)
            {
                printf("mkdir: %s\n", sys_errlist[errno]);
            }
        }
        return 0;
    }
    else if (strcmp(cmd, "lrmdir") == 0)
    {
        if (cmd_hdr->cmd_argv[0] == '\0')
        {
            printf("rmdir: missing operand\n");
        }
        else
        {
            if (rmdir(cmd_hdr->cmd_argv) < 0)
            {
                printf("rmdir: %s\n", sys_errlist[errno]);
            }
        }
        return 0;
    }
    else if (strcmp(cmd, "binary") == 0)
    {
        printf("switch to binary mode...\n");
        return 0;
    }
    else if (strcmp(cmd, "ascii") == 0)
    {
        printf("switch to ascii mode...\n");
        return 0;
    }
    else if ((strcmp(cmd, "help") == 0) || (strcmp(cmd, "?") == 0))
    {
        printf("command on server:\nls\tcd\tpwd\trm\tmkdir\trmdir\nget\tmget\tput\tmput\n");
        printf("\ncomand on client:\ndir\tlcd\tlpwd\tlmkdir\tlrmdir\thelp\nascii\tbinary\tquit\texit\tbye\t?\t!\n");
        return 0;
    }
    else if ((strcmp(cmd, "quit") == 0) || (strcmp(cmd, "bye") == 0)
             || (strcmp(cmd, "exit") == 0) || strcmp(cmd, "!") == 0)
    {
        cmd_hdr->cmd_type = QUIT;
        return 1;
   }
    else if (strcmp(cmd, "") == 0)
    {
        return 0;
    }
    else
    {
        printf("command not found\n");
        return 0;
    }
}

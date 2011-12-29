#include "server.h"

/**/
void do_server_cmd()
{
    int i, j;
    char input_buf[BUF_SIZE];
    char cmd[BUF_SIZE];
    char argv[BUF_SIZE];

    while (1)
    {
        printf("ftpserver> ");

        memset(input_buf, 0, BUF_SIZE);
        fgets(input_buf, BUF_SIZE , stdin);

        /** remove the tail newline character **/
        input_buf[strlen(input_buf)-1] = '\0';

        memset(cmd, 0, BUF_SIZE);
        memset(argv, 0, BUF_SIZE);

        for (i = 0; input_buf[i] != '\0' && (input_buf[i] == ' ' || input_buf[i] == '\t'); i++);
        /** get command name **/
        for (j = 0; input_buf[i] != '\0' && input_buf[i] != ' ' && input_buf[i] != '\t';)
            cmd[j++] = input_buf[i++];

        for (; input_buf[i] != '\0' && (input_buf[i] == ' ' || input_buf[i] == '\t'); i++);
        /** get command argument **/
        for (j = 0; input_buf[i] != '\0' && input_buf[i] != ' ' && input_buf[i] != '\t';)
            argv[j++] = input_buf[i++];

        /** list active user **/
        if (strcmp(cmd, "list") == 0)
        {
            for (i = 0; i < MAX_USER_COUNT; i++)
                if (user_list[i].active == 1)
                    printf("%s ", user_list[i].name);
            printf("\n");
        }
        else if (strcmp(cmd, "count") == 0)
        {
            if (strcmp(argv, "all") == 0)
                printf("total visit count: %d\n", visit_count);
            else if (strcmp(argv, "current") == 0)
                printf("current active count: %d\n", active_count);
            else
                printf("command not found\n");
        }
        else if (strcmp(cmd, "kill") == 0)
        {
            if (argv[0] == '\0')
            {
                printf("command not found\n");
                continue;
            }
            for (i = 0; i < MAX_USER_COUNT; i++)
            {
                if (strcmp(argv, user_list[i].name) == 0)
                {
                    if (user_list[i].active == 1)
                        user_list[i].active = 0;
                    else
                        printf("user [%s] is not active\n", argv);

                    break;
                }
            }

            if (i == MAX_USER_COUNT)
                printf("no such user\n");
        }
        else if ((strcmp(cmd, "quit") == 0) || (strcmp(cmd, "exit") == 0))
        {
            printf("## ftp server is quitting...\n");
            break;
        }
        else if ((strcmp(cmd, "help") == 0) || (strcmp(cmd, "?") == 0))
        {
            printf("available commands are:\nlist\tkill\thelp\texit\tquit\ncount all\tcount current\t?\n");
        }
        else if (strcmp(cmd, "") == 0)
        {
            ;
        }
        else
        {
            printf("command not found\n");
        }
    }
}

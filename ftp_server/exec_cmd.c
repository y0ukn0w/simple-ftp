#include "server.h"

void exec_cmd(char cmd_buf[], char send_buf[],
              char file_buf[], int client_file_sockfd,
              char current_dir[], char processed_path[])
{
    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)cmd_buf;
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)send_buf;

    int i;
    char* argv = cmd_hdr->cmd_argv;
    for (i = 0; i < cmd_hdr->cmd_argc; i++)
    {
        printf("argv[%d]: ==> %s\n", i, argv);
        argv += strlen(argv) + 1;
    }

    if (cmd_hdr->cmd_type == LS)
    {
        do_ls(cmd_hdr->cmd_argv, result_hdr->ret_result, current_dir, processed_path);
    }
    else if (cmd_hdr->cmd_type == MKDIR)
    {
        do_mkdir(cmd_hdr->cmd_argv, result_hdr->ret_result, current_dir, processed_path);
    }
    else if (cmd_hdr->cmd_type == RMDIR)
    {
        do_rmdir(cmd_hdr->cmd_argv, result_hdr->ret_result, current_dir, processed_path);
    }
    else if (cmd_hdr->cmd_type == PWD)
    {
        do_pwd(current_dir, result_hdr->ret_result);
    }
    else if (cmd_hdr->cmd_type == CD)
    {
        do_cd(cmd_hdr->cmd_argv, result_hdr->ret_result, current_dir, processed_path);
    }
    else if (cmd_hdr->cmd_type == RM)
    {
        do_rm(cmd_hdr->cmd_argv, result_hdr->ret_result, current_dir, processed_path);
    }
    /**
    else if (cmd_hdr->cmd_type == PUT)
    {
        do_put(cmd_hdr->cmd_argv, result_hdr->ret_result);
    }
    else if (cmd_hdr->cmd_type == MPUT)
    {
         do_mput(cmd_hdr->cmd_argv, result_hdr->ret_result);
    }
    **
    else if (cmd_hdr->cmd_type == GET)
    {
        do_get(cmd_hdr->cmd_argv, send_buf, file_buf, client_file_sockfd, current_dir, processed_path);
    }
    **
    else if (cmd_hdr->cmd_type == MGET)
    {
         do_mget(cmd_hdr->cmd_argv, result_hdr->ret_result);
    }
    **/
    else
    {
         ;
    }

}

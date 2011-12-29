#include "server.h"

/** execute command from client **/
int exec_cmd(char recv_buf[], char send_buf[], char file_buf[],
              char current_dir[], char processed_path[],
              int client_sockfd, int file_sockfd)
{
    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)recv_buf;
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)send_buf;

    if (cmd_hdr->cmd_type == LS)
    {
        do_ls(cmd_hdr->cmd_argv, result_hdr->ret_result, current_dir, processed_path);
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
    else if (cmd_hdr->cmd_type == MKDIR)
    {
        do_mkdir(cmd_hdr->cmd_argv, result_hdr->ret_result, current_dir, processed_path);
    }
    else if (cmd_hdr->cmd_type == RMDIR)
    {
        do_rmdir(cmd_hdr->cmd_argv, result_hdr->ret_result, current_dir, processed_path);
    }
    else if (cmd_hdr->cmd_type == GET)
    {
        do_get(recv_buf, send_buf, file_buf, current_dir, processed_path, client_sockfd, file_sockfd);
        return 0;
    }
    else if (cmd_hdr->cmd_type == PUT)
    {
        do_put(recv_buf, send_buf, file_buf, current_dir, processed_path, client_sockfd, file_sockfd);
        return 0;
    }
    else
    {
        return 0;
    }
    return 1;
}

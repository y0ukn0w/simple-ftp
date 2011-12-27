#include "server.h"

int check_get(const char* filename, char send_buf[], char current_dir[], char processed_path[])
{
    printf("## check get\n");
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)send_buf;

    if (filename[0] == '\0')
    {
        result_hdr->ret_status = FAIL;
        strcpy(result_hdr->ret_result, "get: missing operand\n");
        return -1;
    }

    preprocess_path(filename, current_dir, processed_path);

    strcpy(result_hdr->ret_result, "get: ");
    if (check_file_for_send(processed_path, result_hdr->ret_result) < 0)
    {
        result_hdr->ret_status = FAIL;
        return -1;
    }
    else
    {
        result_hdr->ret_status = SUCCESS;
        return 0;
    }
}

void do_get(char recv_buf[], char send_buf[], char file_buf[],
            char current_dir[], char processed_path[],
            int client_sockfd, int file_sockfd)
{
    printf("## GET cmd ##\n");
    int ret;
    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)recv_buf;

    ret = check_get(cmd_hdr->cmd_argv, send_buf, current_dir, processed_path);
    send(client_sockfd, send_buf, BUF_SIZE, 0);
    if (ret == 0)
    {
        memset(recv_buf, 0, BUF_SIZE);
        recv(client_sockfd, recv_buf, BUF_SIZE, 0);

        if (cmd_hdr->cmd_type == READY)
            send_file(processed_path, file_sockfd, file_buf);
    }
}

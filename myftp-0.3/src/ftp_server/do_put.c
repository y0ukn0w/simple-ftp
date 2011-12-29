#include "server.h"

int check_put(const char* filename, char send_buf[], char current_dir[], char processed_path[])
{
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)send_buf;

    if (filename[0] == '\0')
    {
        result_hdr->ret_status = FAIL;
        strcpy(result_hdr->ret_result, "put: missing operand\n");
        return -1;
    }

    preprocess_path(filename, current_dir, processed_path);

    strcpy(result_hdr->ret_result, "put: ");
    if (check_file_for_recv(processed_path, result_hdr->ret_result) < 0)
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

void do_put(char recv_buf[], char send_buf[], char file_buf[],
            char current_dir[], char processed_path[],
            int client_sockfd, int file_sockfd)
{
    printf("## PUT cmd ##\n");
    int ret;
    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)recv_buf;

    ret = check_put(cmd_hdr->cmd_argv, send_buf, current_dir, processed_path);
    send(client_sockfd, send_buf, BUF_SIZE, 0);
    if (ret == 0)
    {
        recv_file(processed_path, file_sockfd, file_buf);
    }
}

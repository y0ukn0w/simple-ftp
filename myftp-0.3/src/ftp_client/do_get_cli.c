#include "client.h"

/** get file from ftp server **/
void do_get_cli(char recv_buf[], char send_buf[], char file_buf[],
                int server_sockfd, int file_sockfd)
{
    int ret;
    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)send_buf;
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)recv_buf;

    printf("** send GET cmd\n");

    memset(recv_buf, 0, BUF_SIZE);
    ret = check_file_for_recv(cmd_hdr->cmd_argv, recv_buf);

    if (ret < 0)
        printf("get: %s", recv_buf);
    else
    {
        send(server_sockfd, send_buf, BUF_SIZE, 0);

        memset(recv_buf, 0, BUF_SIZE);
        recv(server_sockfd, recv_buf, BUF_SIZE, 0);

        printf("%s", result_hdr->ret_result);

        if (result_hdr->ret_status == SUCCESS)
        {
            recv_file(cmd_hdr->cmd_argv, file_sockfd, file_buf);
        }
    }
}

#include "client.h"

/** login on ftp server **/
int do_login_cli(char recv_buf[], char send_buf[], int server_sockfd, char* username, char* password)
{
    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)send_buf;
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)recv_buf;

    cmd_hdr->cmd_type = LOGIN;
    cmd_hdr->cmd_argc = 2;

    strcpy(cmd_hdr->cmd_argv, username);
    strcpy(cmd_hdr->cmd_argv + strlen(cmd_hdr->cmd_argv) + 1, password);

    /** send username and password **/
    send(server_sockfd, send_buf, BUF_SIZE, 0);

    recv(server_sockfd, recv_buf, BUF_SIZE, 0);
    printf("%s", result_hdr->ret_result);

    if (result_hdr->ret_status == FAIL)
    {
        return -1;
    }
    return 0;
}

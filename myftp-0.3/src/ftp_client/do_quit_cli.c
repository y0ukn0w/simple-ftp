#include "client.h"

void do_quit_cli(char recv_buf[], char send_buf[], int server_sockfd, int file_sockfd)
{
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)recv_buf;

    send(server_sockfd, send_buf, BUF_SIZE, 0);

    memset(recv_buf, 0, BUF_SIZE);
    recv(server_sockfd, recv_buf, BUF_SIZE, 0);
    printf("%s", result_hdr->ret_result);

    close(server_sockfd);
    close(file_sockfd);
}

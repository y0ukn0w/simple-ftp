#include "server.h"

void do_quit(char send_buf[], int client_sockfd, int file_sockfd)
{
    printf("doing quit...\n");
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)send_buf;

    if (file_sockfd >= 0)
        close(file_sockfd);

    strcpy(result_hdr->ret_result, "Quit OK, goodbye!\n");

    send(client_sockfd, send_buf, BUF_SIZE, 0);
    return;
}

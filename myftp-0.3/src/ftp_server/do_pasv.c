#include "server.h"

int do_pasv(char send_buf[], int client_sockfd, int data_port, int data_sockfd)
{
    printf("## PASV cmd\n");
    int file_sockfd;
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)send_buf;

    result_hdr->ret_status = SUCCESS;
    /** send data port to client **/
    memcpy(result_hdr->ret_result, (char*)&data_port, sizeof(int));

    send(client_sockfd, send_buf, BUF_SIZE, 0);

    file_sockfd = accept(data_sockfd, NULL, NULL);
    return file_sockfd;
}

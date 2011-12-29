#include "client.h"

/** send PASV command to server **/
int do_pasv_cli(char recv_buf[], char send_buf[], int server_sockfd, char* ftp_server_ip)
{
    printf("** enter PASV mode\n");
    int file_sockfd, data_port;

    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)send_buf;
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)recv_buf;

    cmd_hdr->cmd_type = PASV;
    send(server_sockfd, send_buf, BUF_SIZE, 0);

    recv(server_sockfd, recv_buf, BUF_SIZE, 0);
    if (result_hdr->ret_status == FAIL)
    {
        printf("%s", result_hdr->ret_result);
        return -1;
    }
    else
    {
        /** create socket for file transfer **/
        memcpy((char*)&data_port, result_hdr->ret_result, sizeof(int));
        printf("** data port is %d\n", data_port);
        file_sockfd = ftp_connect_socket(ftp_server_ip, data_port);
        return file_sockfd;
    }
}

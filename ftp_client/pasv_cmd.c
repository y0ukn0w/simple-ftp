#include "client.h"

int pasv_cmd(char recv_buf[], char send_buf[], int server_sockfd, char* ftp_server_ip)
{
    printf("pasv_cmd\n");
//    int i;
    int file_sockfd, data_port;
    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)send_buf;
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)recv_buf;

    cmd_hdr->cmd_type = PASV;
    printf("send pasv cmd\n");
    send(server_sockfd, send_buf, BUF_SIZE, 0);

    printf("waiting for response\n");
    recv(server_sockfd, recv_buf, BUF_SIZE, 0);
    printf("got answer\n");
    if (result_hdr->ret_status == FAIL)
    {
        printf("%s", result_hdr->ret_result);
        return -1;
    }
    else
    {
        memcpy((char*)&data_port, result_hdr->ret_result, sizeof(int));
//        for (i = 0; i < 6; i++)
//            printf("%2X ", ((unsigned char*)result_hdr->ret_result)[i]);
        printf("data_port is %u\n", data_port);
        file_sockfd = ftp_connect_socket(ftp_server_ip, data_port);
        if (file_sockfd < 0)
        {
            return -1;
        }
        else
        {
            return file_sockfd;
        }
    }
}

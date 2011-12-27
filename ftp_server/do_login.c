#include "server.h"

int do_login(char recv_buf[], char send_buf[], int client_sockfd)
{
    printf("## someone login\n");
    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)recv_buf;
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)send_buf;

    char* username = cmd_hdr->cmd_argv;
    char* password = cmd_hdr->cmd_argv + strlen(cmd_hdr->cmd_argv) + 1;

    if (check_login(username, password) == 1)
    {
        result_hdr->ret_status = SUCCESS;
        strcpy(result_hdr->ret_result, "## Login OK, welcome!\n");
        send(client_sockfd, send_buf, BUF_SIZE, 0);
        return 0;
    }
    else
    {
        result_hdr->ret_status = FAIL;
        strcpy(result_hdr->ret_result, "## Login failed\n");
        send(client_sockfd, send_buf, BUF_SIZE, 0);
        return -1;
    }
}

int do_pasv(char send_buf[], int client_sockfd, int data_port, int data_sockfd, int* file_sockfd)
{
    printf("## PASV cmd\n");

    struct cmd_result_header* result_hdr = (struct cmd_result_header*)send_buf;

    result_hdr->ret_status = SUCCESS;
    memcpy(result_hdr->ret_result, (char*)&data_port, sizeof(int));

    send(client_sockfd, send_buf, BUF_SIZE, 0);

    *file_sockfd = accept(data_sockfd, NULL, NULL);
    if (*file_sockfd < 0)
    {
        perror("accept");
        return -1;
    }
    return 0;
}

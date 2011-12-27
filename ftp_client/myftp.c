#include "client.h"

int ftp_connect_socket(char *ip, int port);

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("%s username:password@ftp-server-ip:ftp-server-port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *username, *password, *ftp_server_ip, *ftp_server_port;

    username = strtok(argv[1], ":");
    password = strtok(NULL, "@");
    ftp_server_ip = strtok(NULL, ":");
    ftp_server_port = strtok(NULL, " ");

    if (username == NULL || password == NULL || ftp_server_ip == NULL || ftp_server_port == NULL)
    {
        printf("%s username:password@ftp-server-ip:ftp-server-port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(ftp_server_port);
    int server_sockfd = ftp_connect_socket(ftp_server_ip, port);
    if (server_sockfd < 0)
    {
        perror("Connecting ftp server");
        exit(EXIT_FAILURE);
    }

    char send_buf[BUF_SIZE];
    char recv_buf[BUF_SIZE];

    struct cmd_type_header* cmd_hdr;
    struct cmd_result_header* result_hdr;
    char* cmd_argv;

    cmd_hdr = (struct cmd_type_header*)send_buf;
    result_hdr = (struct cmd_result_header*)recv_buf;


    memset(send_buf, 0, BUF_SIZE);
    /** send login info **/
    cmd_hdr->cmd_type = LOGIN;
    cmd_hdr->cmd_argc = 2;

    cmd_argv = cmd_hdr->cmd_argv;
    strcpy(cmd_argv, username);

    cmd_argv += strlen(cmd_argv) + 1;
    strcpy(cmd_argv, password);

    send(server_sockfd, send_buf, BUF_SIZE, 0);

    memset(recv_buf, 0, BUF_SIZE);
    recv(server_sockfd, recv_buf, BUF_SIZE, 0);

    printf("%s", result_hdr->ret_result);
    if (result_hdr->ret_status == FAIL)
    {
        close(server_sockfd);
        exit(EXIT_FAILURE);
    }

    char str_buf[BUF_SIZE];

    int file_sockfd;
    char file_buf[BUF_SIZE];

    /** send PASV cmd **/
    memset(send_buf, 0, BUF_SIZE);
    memset(recv_buf, 0, BUF_SIZE);
    file_sockfd = pasv_cmd(recv_buf, send_buf, server_sockfd, ftp_server_ip);
    if (file_sockfd < 0)
    {
        printf("creating socket for file transfer error\n");
        exit(EXIT_FAILURE);
    }

    int ret;
//    char* filename;

    while (1)
    {
        printf("myftp> ");

        memset(str_buf, 0, sizeof(str_buf));
        fgets(str_buf, BUF_SIZE , stdin);

        memset(send_buf, 0, BUF_SIZE);
        cmd_analysize(str_buf, send_buf);

        if (cmd_hdr->cmd_type == NOT_SUPPORTED)
            printf("command not found\n");
        else if (cmd_hdr->cmd_type == DUMMY)
            continue;
        else if (cmd_hdr->cmd_type == QUIT)
        {
            send(server_sockfd, send_buf, BUF_SIZE, 0);

            memset(recv_buf, 0, BUF_SIZE);
            recv(server_sockfd, recv_buf, BUF_SIZE, 0);
            printf("%s", result_hdr->ret_result);

            close(server_sockfd);
            close(file_sockfd);
            break;
        }
        else if (cmd_hdr->cmd_type == GET)
        {
            printf("** send get cmd\n");
            send(server_sockfd, send_buf, BUF_SIZE, 0);

            memset(recv_buf, 0, BUF_SIZE);
            recv(server_sockfd, recv_buf, BUF_SIZE, 0);

            printf("%s", result_hdr->ret_result);

            if (result_hdr->ret_status == SUCCESS)
            {
                memset(recv_buf, 0, BUF_SIZE);
                ret = check_file_for_recv(cmd_hdr->cmd_argv, recv_buf);

                if (ret == 0)
                    cmd_hdr->cmd_type = READY;
                else
                    cmd_hdr->cmd_type = NOT_READY;
                send(server_sockfd, send_buf, BUF_SIZE, 0);

                if (ret == 0)
                    recv_file(cmd_hdr->cmd_argv, file_sockfd, file_buf);
            }
        }
        else if (cmd_hdr->cmd_type == PUT)
        {
            printf("** send put cmd\n");

            memset(recv_buf, 0, BUF_SIZE);
            ret = check_file_for_send(cmd_hdr->cmd_argv, recv_buf);

            if (ret == 0)
            {
                send(server_sockfd, send_buf, BUF_SIZE, 0);

                memset(recv_buf, 0, BUF_SIZE);
                recv(server_sockfd, recv_buf, BUF_SIZE, 0);

                printf("%s", result_hdr->ret_result);

                if (result_hdr->ret_status == SUCCESS)
                {
                    send_file(cmd_hdr->cmd_argv, file_sockfd, file_buf);
                }
            }
        }
        else
        {
            send(server_sockfd, send_buf, BUF_SIZE, 0);

            memset(recv_buf, 0, BUF_SIZE);
            recv(server_sockfd, recv_buf, BUF_SIZE, 0);
            printf("%s", result_hdr->ret_result);
        }

    }
    return 0;
}


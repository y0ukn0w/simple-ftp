#include "client.h"

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s username:password@ftp-server-ip:ftp-server-port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *username, *password, *ftp_server_ip, *ftp_server_port;

    username = strtok(argv[1], ":");
    password = strtok(NULL, "@");
    ftp_server_ip = strtok(NULL, ":");
    ftp_server_port = strtok(NULL, " ");

    if (username == NULL || password == NULL || ftp_server_ip == NULL || ftp_server_port == NULL)
    {
        printf("Usage: %s username:password@ftp-server-ip:ftp-server-port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(ftp_server_port);
    int server_sockfd = ftp_connect_socket(ftp_server_ip, port);
    if (server_sockfd < 0)
    {
        perror("** connecting ftp server");
        exit(EXIT_FAILURE);
    }

    char send_buf[BUF_SIZE];
    char recv_buf[BUF_SIZE];

    char input_buf[BUF_SIZE];
    char file_buf[FILE_BUF_SIZE];

    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)send_buf;
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)recv_buf;

    memset(send_buf, 0, BUF_SIZE);
    memset(recv_buf, 0, BUF_SIZE);

    int ret;
    /** login on the server **/
    ret = do_login_cli(recv_buf, send_buf, server_sockfd, username, password);
    /** login failed **/
    if (ret < 0)
    {
        close(server_sockfd);
        exit(EXIT_FAILURE);
    }

    memset(send_buf, 0, BUF_SIZE);
    memset(recv_buf, 0, BUF_SIZE);

    int file_sockfd;
    /** send PASV command **/
    file_sockfd = do_pasv_cli(recv_buf, send_buf, server_sockfd, ftp_server_ip);
    if (file_sockfd < 0)
    {
        printf("** conncet socket failed\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("myftp> ");

        memset(input_buf, 0, BUF_SIZE);
        /** get command from the console **/
        fgets(input_buf, BUF_SIZE , stdin);

        memset(send_buf, 0, BUF_SIZE);
        memset(recv_buf, 0, BUF_SIZE);
        /** analysize command buffer **/
        ret = exec_cmd_cli(input_buf, recv_buf, send_buf, file_buf, server_sockfd, file_sockfd);

        /** need to do something else **/
        if (ret == 1)
        {
            /** terminate the session **/
            if (cmd_hdr->cmd_type == QUIT)
            {
                do_quit_cli(recv_buf, send_buf, server_sockfd, file_sockfd);
                break;
            }
            /** send command to server **/
            else
            {
                send(server_sockfd, send_buf, BUF_SIZE, 0);
                memset(recv_buf, 0, BUF_SIZE);
                recv(server_sockfd, recv_buf, BUF_SIZE, 0);
                printf("%s", result_hdr->ret_result);
            }
        }
    }
    return 0;
}

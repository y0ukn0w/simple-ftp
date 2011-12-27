#include "server.h"

char* FTP_ROOT_DIR = "/tmp";

int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int client_sockfd;
    socklen_t len = sizeof(struct sockaddr_in);
    struct sockaddr_in remoteaddr;

    int port = atoi(argv[1]);
    if(port <= BUF_SIZE || port > 65535)
    {
        printf("the port should be in the range from 1025 to 65535\n");
        exit(EXIT_FAILURE);
    }

    int ftp_fd = ftp_create_socket(port);
    if (ftp_fd < 0)
    {
        printf("create socket of port error\n");
        exit(EXIT_FAILURE);
    }

    int data_port = port + 1;
    int file_sockfd = ftp_create_socket(data_port);
    if (file_sockfd < 0)
    {
        printf("create socket of port error\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        client_sockfd = accept(ftp_fd, (struct sockaddr*)&remoteaddr, &len);
        if (client_sockfd < 0)
        {
            perror("accept");
            break;
        }
        printf("client socketfd is %d\n", client_sockfd);
        struct client_thread_argv c_argv = {client_sockfd, data_port, file_sockfd};
        pthread_t tid;
        pthread_create(&tid, NULL, (void*)client_thread, (void*)&c_argv);
    }

    close(ftp_fd);
    return 0;

}

int ftp_create_socket(int port)
{
    int fd;
    struct sockaddr_in localaddr;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("creating socket");
        return -1;
    }

    localaddr.sin_family = AF_INET;
    localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localaddr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *)&localaddr, sizeof(localaddr)) < 0)
    {
        perror("bind socket");
        return -1;
    }

    if (listen(fd, 5) < 0)
    {
        perror("listen socket");
        return -1;
    }
    printf("create socket successfully\n");
    return fd;
}

void* client_thread(void* argv)
{
    struct client_thread_argv* cli_argv = (struct client_thread_argv*)argv;
	int client_sockfd = cli_argv->client_sockfd;
    int file_sockfd = cli_argv->file_sockfd;
    int data_port = cli_argv->data_port;

    printf("file_sockfd is %d\ndata_port is %d\n", file_sockfd, data_port);

	char recv_buf[BUF_SIZE], send_buf[BUF_SIZE];
	char file_buf[FILE_BUF_SIZE];

    char current_dir[TMP_BUF_SIZE] = "/";
    char processed_path[TMP_BUF_SIZE];

    struct cmd_type_header* cmd_hdr;
    struct cmd_result_header* result_hdr;

    cmd_hdr = (struct cmd_type_header*)recv_buf;
    result_hdr = (struct cmd_result_header*)send_buf;

    int client_file_sockfd = -1;
	while (1)
	{
        memset(recv_buf, 0, sizeof(recv_buf));
		if(recv(client_sockfd, recv_buf, sizeof(recv_buf), 0) < 0)
		{
			perror("recv");
            break;
		}

		memset(send_buf, 0, sizeof(send_buf));

        if (cmd_hdr->cmd_type == LOGIN)
        {
            printf("someone login\n");

            char *username, *password;
            username = cmd_hdr->cmd_argv;
            password = cmd_hdr->cmd_argv + strlen(cmd_hdr->cmd_argv) + 1;

            if (check_login(username, password) == 1)
            {
                result_hdr->ret_status = SUCCESS;
                strcpy(result_hdr->ret_result, "Login OK\n");
                send(client_sockfd, send_buf, sizeof(send_buf), 0);

                memset(recv_buf, 0, sizeof(recv_buf));
                if(recv(client_sockfd, recv_buf, sizeof(recv_buf), 0) < 0)
                {
                    perror("recv");
                    break;
                }
                if (cmd_hdr->cmd_type == PASV)
                {
                    printf("in server main(), PASV cmd\n");
                    memset(send_buf, 0, sizeof(send_buf));

                    result_hdr->ret_status = SUCCESS;
                    memcpy(result_hdr->ret_result, (char*)&data_port, sizeof(int));
                    printf("\nsend file sockfd\n");
                    send(client_sockfd, send_buf, BUF_SIZE, 0);

                    client_file_sockfd = accept(file_sockfd, NULL, NULL);
                    if (client_file_sockfd < 0)
                    {
                        perror("accept");
                        break;
                    }

                }
            }
            else
            {
                result_hdr->ret_status = FAIL;
                strcpy(result_hdr->ret_result, "Login failed\n");
                send(client_sockfd, send_buf, sizeof(send_buf), 0);
                break;
            }

        }
        else if (cmd_hdr->cmd_type == QUIT)
        {
            do_quit(client_file_sockfd);
            strcpy(result_hdr->ret_result, "Quit OK, goodbye!\n");
            send(client_sockfd, send_buf, sizeof(send_buf), 0);
            break;
        }
        else if (cmd_hdr->cmd_type == GET)
        {
            do_get(cmd_hdr->cmd_argv, send_buf, current_dir, processed_path);
            send(client_sockfd, send_buf, sizeof(send_buf), 0);

            if (result_hdr->ret_status == SUCCESS)
                send_file(processed_path, client_file_sockfd, file_buf);
        }
        else if (cmd_hdr->cmd_type == PUT)
        {
            do_put(cmd_hdr->cmd_argv, send_buf, current_dir, processed_path);
            send(client_sockfd, send_buf, sizeof(send_buf), 0);

            if (result_hdr->ret_status == SUCCESS)
                recv_file(processed_path, client_file_sockfd, file_buf);
        }
        else
		{
		    memset(processed_path, 0, sizeof(processed_path));
		    exec_cmd(recv_buf, send_buf, file_buf, client_file_sockfd, current_dir, processed_path);
            send(client_sockfd, send_buf, sizeof(send_buf), 0);
		}
	}

	printf("closing socket\n");
    close(client_sockfd);
    pthread_exit(NULL);
}

#include "server.h"

/** serve for connected client **/
void* client_thread(void* argv)
{
    struct client_thread_argv* cli_argv = (struct client_thread_argv*)argv;
	int client_sockfd = cli_argv->client_sockfd;
    int data_sockfd = cli_argv->data_sockfd;
    int data_port = cli_argv->data_port;

	char recv_buf[BUF_SIZE], send_buf[BUF_SIZE];
	char file_buf[FILE_BUF_SIZE];

    char current_dir[BUF_SIZE] = "/";
    char processed_path[BUF_SIZE];

    struct cmd_type_header* cmd_hdr;
    cmd_hdr = (struct cmd_type_header*)recv_buf;

    int file_sockfd, user_id;
    int ret;
	while (1)
	{
        memset(recv_buf, 0, BUF_SIZE);
		if(recv(client_sockfd, recv_buf, BUF_SIZE, 0) < 0)
		{
			perror("## recv");
            break;
		}

		memset(send_buf, 0, BUF_SIZE);
        memset(processed_path, 0, sizeof(processed_path));

        if (cmd_hdr->cmd_type == LOGIN)
        {
            user_id = do_login(recv_buf, send_buf, client_sockfd);
            /** login failed **/
            if (user_id < 0)
                break;
        }
        else if (cmd_hdr->cmd_type == PASV)
        {
            file_sockfd = do_pasv(send_buf, client_sockfd, data_port, data_sockfd);
            if (file_sockfd < 0)
                break;
        }
        else if (cmd_hdr->cmd_type == QUIT)
        {
            /** quit this session **/
            do_quit(send_buf, client_sockfd, file_sockfd, user_id);
            break;
        }
        else
		{
		    ret = exec_cmd(recv_buf, send_buf, file_buf, current_dir, processed_path, client_sockfd, file_sockfd);
            /** send result back to client **/
		    if (ret == 1)
                send(client_sockfd, send_buf, BUF_SIZE, 0);
		}
	}

	printf("## closing client socket...\n");
    close(client_sockfd);
    pthread_exit(NULL);
}

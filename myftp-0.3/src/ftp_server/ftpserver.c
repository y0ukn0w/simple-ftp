#include "server.h"

char FTP_ROOT_DIR[BUF_SIZE];

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    if(port <= BUF_SIZE || port > 65535)
    {
        printf("the port should be in the range from 1025 to 65535\n");
        exit(EXIT_FAILURE);
    }

    int ftp_fd = ftp_create_socket(port);
    if (ftp_fd < 0)
    {
        printf("## create socket failed\n");
        exit(EXIT_FAILURE);
    }

    int data_port = port + 1;
    /** create socket for data transfer **/
    int data_sockfd = ftp_create_socket(data_port);
    if (data_sockfd < 0)
    {
        printf("## create socket failed\n");
        exit(EXIT_FAILURE);
    }

    /** set the ftp root to the current directory **/
    getcwd(FTP_ROOT_DIR, BUF_SIZE);

    /** load valid user list **/
    init_user_list();

    /** create server thread, waiting for client **/
    pthread_t tid;
    struct server_thread_argv svr_argv = { ftp_fd, data_port, data_sockfd };
    pthread_create(&tid, NULL, server_thread, &svr_argv);

    do_server_cmd();

    printf("## closing data socket...\n");
    close(data_sockfd);

    printf("## closing server socket...\n");
    close(ftp_fd);

    printf("## ftp server quits OK\n");
    return 0;

}

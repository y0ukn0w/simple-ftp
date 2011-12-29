#include "server.h"

/** wait for client to connect **/
void* server_thread(void* argv)
{
    struct server_thread_argv* svr_argv = (struct server_thread_argv*)argv;
    int ftp_fd = svr_argv->ftp_fd;
    int data_port = svr_argv->data_port;
    int data_sockfd = svr_argv->data_sockfd;

    int client_sockfd;
    while (1)
    {
        client_sockfd = accept(ftp_fd, NULL, NULL);
        if (client_sockfd < 0)
        {
            perror("## accept");
            break;
        }

        /** create a new thread to serve the connected client **/
        struct client_thread_argv cli_argv = { client_sockfd, data_port, data_sockfd };
        pthread_t tid;
        pthread_create(&tid, NULL, client_thread, &cli_argv);
    }
    printf("## server thread is quitting...\n");
    pthread_exit(NULL);
    return NULL;
}

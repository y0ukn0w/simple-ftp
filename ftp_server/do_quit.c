#include "server.h"

void do_quit(int file_sockfd)
{
    printf("doing quit...\n");
    if (file_sockfd >= 0)
        close(file_sockfd);
    return;
}

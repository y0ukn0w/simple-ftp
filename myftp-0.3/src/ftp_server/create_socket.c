#include "server.h"

int ftp_create_socket(int port)
{
    int fd;
    struct sockaddr_in localaddr;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("## creating socket");
        return -1;
    }

    localaddr.sin_family = AF_INET;
    localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localaddr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr*)&localaddr, sizeof(localaddr)) < 0)
    {
        perror("## bind socket");
        return -1;
    }

    if (listen(fd, 5) < 0)
    {
        perror("## listen socket");
        return -1;
    }
    return fd;
}

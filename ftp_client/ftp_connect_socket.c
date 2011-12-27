#include "client.h"

int ftp_connect_socket(char* ip, int port)
{
    int fd;
    struct sockaddr_in remoteaddr;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    remoteaddr.sin_family = AF_INET;
    remoteaddr.sin_addr.s_addr = inet_addr(ip);
    remoteaddr.sin_port = htons(port);

    if(connect(fd, (struct sockaddr*)&remoteaddr, sizeof(remoteaddr)) < 0)
    {
        perror("connect");
        return -1;
    }
    else
        return fd;
}

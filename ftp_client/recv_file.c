#include "server.h"

void recv_file(const char* file, int file_sockfd, char file_buf[])
{
    printf("open file for write\n");
    printf("filename: %s\n", file);

    struct file_header* file_hdr;
    file_hdr = (struct file_header*)file_buf;

    FILE* fp = fopen(file, "w");
    if (fp == NULL)
    {
        printf("open file for write failed\n");
        return;
    }
    printf("open OK\n");

    int i = 0;
    while (1)
    {
        memset(file_buf, 0, FILE_BUF_SIZE);
        recv(file_sockfd, file_buf, FILE_BUF_SIZE, 0);

        printf(".");
        i++;
        if (i % 60 == 0)
            printf("\n");
        if (file_hdr->trans_size != 0)
            fwrite(file_hdr->content, 1, file_hdr->trans_size, fp);
        else
            break;
    }

    printf("\nwrite file OK\n");
    fclose(fp);
}

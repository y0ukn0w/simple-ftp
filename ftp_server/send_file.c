#include "server.h"

void send_file(const char* file, int client_file_sockfd, char file_buf[])
{
    FILE* fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("open file for write failed\n");
        return;
    }

    struct file_header* file_hdr;
    file_hdr = (struct file_header*)file_buf;

    int i, j = 1;
    while (!feof(fp))
    {
        int n_read = FILE_BUF_SIZE - sizeof(struct file_header);
        memset(file_buf, 0, FILE_BUF_SIZE);
        file_hdr->trans_size = fread(file_hdr->content, 1, n_read, fp);
        for (i = 0; i < 15; i++)
            printf("%02X ", (unsigned char)(file_hdr->content[i]));
        send(client_file_sockfd, file_buf, FILE_BUF_SIZE, 0);
        printf("\n%d: send ok\n", j++);
    }

    memset(file_buf, 0, FILE_BUF_SIZE);
    send(client_file_sockfd, file_buf, FILE_BUF_SIZE, 0);

    fclose(fp);
}

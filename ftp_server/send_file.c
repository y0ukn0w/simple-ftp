#include "server.h"

void send_file(const char* filename, int file_sockfd, char file_buf[])
{
    printf("open file for read, filename: %s\n", filename);

    int i = 0;
    FILE* fp;
    struct file_header* file_hdr = (struct file_header*)file_buf;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("open file %s for read failed\n", filename);
    }
    else
    {
        printf("begin to send file...\n");
        while (!feof(fp))
        {
            file_hdr->trans_size = fread(file_hdr->content,
                                         1,
                                         FILE_BUF_SIZE - sizeof(struct file_header),
                                         fp);
            printf(".");
            i++;
            if (i % 70 == 0)
                printf("\n");
            send(file_sockfd, file_buf, FILE_BUF_SIZE, 0);
        }
        fclose(fp);
    }

    printf("\ncompleted\n");

    /** file transfer completed **/
    file_hdr->trans_size = -1;
    send(file_sockfd, file_buf, FILE_BUF_SIZE, 0);
}

void recv_file(const char* filename, int file_sockfd, char file_buf[])
{
    printf("open file for write, filename: %s\n", filename);

    int i = 0;
    FILE* fp;
    struct file_header* file_hdr = (struct file_header*)file_buf;

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("open file %s for write failed\n", filename);
        printf("ERROR: %s\n", sys_errlist[errno]);
    }
    else
    {
        printf("begin to receive file...\n");
        while (1)
        {
            memset(file_buf, 0, FILE_BUF_SIZE);
            recv(file_sockfd, file_buf, FILE_BUF_SIZE, 0);

            printf(".");
            i++;
            if (i % 60 == 0)
                printf("\n");

            if (file_hdr->trans_size != -1)
                fwrite(file_hdr->content, 1, file_hdr->trans_size, fp);
            else
                break;
        }
        fclose(fp);
        printf("\nwrite file OK\n");
    }
}

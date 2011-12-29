#include "file.h"
#include <sys/socket.h>

struct file_header
{
    int trans_size;
    char content[];
};

void send_file(const char* filename, int file_sockfd, char file_buf[])
{
    printf("#* open file for read, filename: %s\n", filename);

    int i = 0;
    FILE* fp;
    struct file_header* file_hdr = (struct file_header*)file_buf;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("#* open failed: %s\n", sys_errlist[errno]);
    }
    else
    {
        printf("#* begin to send file...\n");
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

    /** file transfer completed **/
    file_hdr->trans_size = -1;
    send(file_sockfd, file_buf, FILE_BUF_SIZE, 0);
    printf("\n#* transfer completed\n");
}

void recv_file(const char* filename, int file_sockfd, char file_buf[])
{
    printf("#* open file for write, filename: %s\n", filename);

    int i = 0;
    FILE* fp;
    struct file_header* file_hdr = (struct file_header*)file_buf;

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("#* open failed: %s\n", sys_errlist[errno]);
    }
    else
    {
        printf("#* begin to receive file...\n");
        while (1)
        {
            recv(file_sockfd, file_buf, FILE_BUF_SIZE, 0);

            printf(".");
            i++;
            if (i % 70 == 0)
                printf("\n");

            if (file_hdr->trans_size != -1)
                fwrite(file_hdr->content, 1, file_hdr->trans_size, fp);
            else
                break;
        }
        fclose(fp);
        printf("\n#* transfer completed\n");
    }
}

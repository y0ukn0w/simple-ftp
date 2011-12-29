#include "client.h"

/** put multiple files to ftp server **/
void do_mput_cli(char recv_buf[], char send_buf[], char file_buf[],
                int server_sockfd, int file_sockfd)
{
    int i;
    char* filename;
    char tmp_buf[BUF_SIZE];

    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)send_buf;
    struct cmd_type_header* cmd_hdr_tmp = (struct cmd_type_header*)tmp_buf;

    printf("** MPUT cmd\n");

    /** just do PUT operation continuously **/
    cmd_hdr_tmp->cmd_type = PUT;
    cmd_hdr_tmp->cmd_argc = 1;

    filename = cmd_hdr->cmd_argv;
    for (i = 0; i < cmd_hdr->cmd_argc; i++)
    {
        strcpy(cmd_hdr_tmp->cmd_argv, filename);
        do_put_cli(recv_buf, tmp_buf, file_buf, server_sockfd, file_sockfd);
        filename += strlen(filename) + 1;
    }
}

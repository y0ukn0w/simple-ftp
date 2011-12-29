#include "client.h"

/** get multiple files from ftp server **/
void do_mget_cli(char recv_buf[], char send_buf[], char file_buf[],
                int server_sockfd, int file_sockfd)
{
    int i;
    char* filename;
    char tmp_buf[BUF_SIZE];

    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)send_buf;
    struct cmd_type_header* cmd_hdr_tmp = (struct cmd_type_header*)tmp_buf;

    printf("** MGET cmd\n");

    /** just do GET operation continuously **/
    cmd_hdr_tmp->cmd_type = GET;
    cmd_hdr_tmp->cmd_argc = 1;

    filename = cmd_hdr->cmd_argv;
    for (i = 0; i < cmd_hdr->cmd_argc; i++)
    {
        strcpy(cmd_hdr_tmp->cmd_argv, filename);
        do_get_cli(recv_buf, tmp_buf, file_buf, server_sockfd, file_sockfd);
        filename += strlen(filename) + 1;
    }
}

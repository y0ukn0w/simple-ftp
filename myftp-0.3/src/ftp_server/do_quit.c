#include "server.h"

/** client quits **/
void do_quit(char send_buf[], int client_sockfd, int file_sockfd, int user_id)
{
    printf("## client is quitting...\n");
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)send_buf;

    close(file_sockfd);

    strcpy(result_hdr->ret_result, "## Quit OK, goodbye!\n");

    send(client_sockfd, send_buf, BUF_SIZE, 0);

    user_list[user_id].active = 0;
    active_count--;

    return;
}

#include "server.h"

int active_count;
int visit_count;

int do_login(char recv_buf[], char send_buf[], int client_sockfd)
{
    printf("## someone login\n");
    struct cmd_type_header* cmd_hdr = (struct cmd_type_header*)recv_buf;
    struct cmd_result_header* result_hdr = (struct cmd_result_header*)send_buf;

    char* username = cmd_hdr->cmd_argv;
    char* password = cmd_hdr->cmd_argv + strlen(cmd_hdr->cmd_argv) + 1;

    printf("## checking user...\n");
    int user_id = check_login(username, password);
    if (user_id < 0)
    {
        printf("## invalid user\n");
        result_hdr->ret_status = FAIL;
        strcpy(result_hdr->ret_result, "## login failed\n");
        send(client_sockfd, send_buf, BUF_SIZE, 0);
        return -1;
    }
    else
    {
        printf("## user [%s] login\n", username);
        user_list[user_id].active = 1;
        active_count++;
        visit_count++;

        result_hdr->ret_status = SUCCESS;
        sprintf(result_hdr->ret_result, "## login OK, welcome! You are user #%02d\n", active_count);
        send(client_sockfd, send_buf, BUF_SIZE, 0);
        return user_id;
    }
}

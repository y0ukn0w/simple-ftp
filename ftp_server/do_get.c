#include "server.h"

void do_get(const char* filename, char send_buf[], char current_dir[], char processed_path[])
{
    printf("get cmd\n");
    FILE* fp;

    struct cmd_result_header* result_hdr;
    result_hdr = (struct cmd_result_header*)send_buf;

    if (filename[0] == '\0')
    {
        result_hdr->ret_status = FAIL;
        strcpy(result_hdr->ret_result, "get: missing operand\n");
        return;
    }

    preprocess_path(filename, current_dir, processed_path);

    printf("open file\n");
    fp = fopen(processed_path, "r");

    if (fp == NULL)
    {
        result_hdr->ret_status = FAIL;
        sprintf(result_hdr->ret_result, "get: %s\n", sys_errlist[errno]);
        return;
    }

    fclose(fp);
    result_hdr->ret_status = SUCCESS;
    strcpy(result_hdr->ret_result, "begin to send file\n");

    return;
}

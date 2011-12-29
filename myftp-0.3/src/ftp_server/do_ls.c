#include "server.h"

void do_ls(const char* filename, char buf[], char current_dir[], char processed_path[])
{
    struct stat stat_buf;

    preprocess_path(filename, current_dir, processed_path);

    if (stat(processed_path, &stat_buf) < 0)
    {
        sprintf(buf, "ls: %s\n", sys_errlist[errno]);
        return;
    }

    if (S_ISDIR(stat_buf.st_mode))
        display_dir(processed_path, buf);
    else
        display_file(processed_path, filename, buf);
}

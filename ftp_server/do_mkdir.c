#include "server.h"

void do_mkdir(const char* path, char buf[], char current_dir[], char processed_path[])
{
    if (path[0] == '\0')
    {
        strcpy(buf, "mkdir: missing operand\n");
        return;
    }

    preprocess_path(path, current_dir, processed_path);

    if (mkdir(processed_path, 0777) < 0)
    {
        sprintf(buf, "mkdir: %s\n", sys_errlist[errno]);
    }
}

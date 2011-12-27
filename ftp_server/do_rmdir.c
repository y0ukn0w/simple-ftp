#include "server.h"

void do_rmdir(const char* path, char buf[], char current_dir[], char processed_path[])
{
    if (path[0] == '\0')
    {
        strcpy(buf, "rmdir: missing operand\n");
        return;
    }

    preprocess_path(path, current_dir, processed_path);

    if (rmdir(processed_path) < 0)
    {
        sprintf(buf, "rmdir: %s\n", sys_errlist[errno]);
    }
}

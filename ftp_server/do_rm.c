#include "server.h"

void do_rm(const char* path, char buf[], char current_dir[], char processed_path[])
{
    if (path[0] == '\0')
    {
        strcpy(buf, "rm: missing operand\n");
        return;
    }

    preprocess_path(path, current_dir, processed_path);

    if (unlink(processed_path) < 0)
    {
        sprintf(buf, "rm: %s\n", sys_errlist[errno]);
    }
}

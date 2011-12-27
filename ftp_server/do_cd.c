#include "server.h"

void do_cd(const char* path, char buf[], char current_dir[], char processed_path[])
{
    if (path[0] == '\0')
    {
        strcpy(current_dir, "/");
        return;
    }

    preprocess_path(path, current_dir, processed_path);

    if (chdir(processed_path) < 0)
    {
        sprintf(buf, "cd: %s\n", sys_errlist[errno]);
        return;
    }

    strcpy(current_dir, &processed_path[strlen(FTP_ROOT_DIR)]);
}

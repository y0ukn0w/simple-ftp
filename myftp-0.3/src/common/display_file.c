#include "file.h"
#include <dirent.h>

/** display file size and file name **/
void display_file(const char* path, const char* filename, char buf[])
{
    struct stat stat_buf;
    char tmp_buf[BUF_SIZE];

    if (stat(path, &stat_buf) < 0)
    {
        strcat(buf, "ls: ");
        strcat(buf, sys_errlist[errno]);
        strcat(buf, "\n");
        return;
    }

    sprintf(tmp_buf, "%10ld\t%s", stat_buf.st_size, filename);
    strcat(buf, tmp_buf);

    if (S_ISDIR(stat_buf.st_mode))
        strcat(buf, "/");
    strcat(buf, "\n");
}

/** display directory content **/
void display_dir(const char* dirname, char buf[])
{
    DIR* dir;
    struct dirent* mydirent;
    char tmp_buf[BUF_SIZE];

    if ((dir = opendir(dirname)) == NULL)
    {
        strcat(buf, "ls: ");
        strcat(buf, sys_errlist[errno]);
        strcat(buf, "\n");
        return;
    }

    while ((mydirent = readdir((dir))) != NULL)
    {
        /** hide . and .. directory **/
        if (strcmp(mydirent->d_name, ".") == 0 || strcmp(mydirent->d_name, "..") == 0)
            continue;

        sprintf(tmp_buf, "%s/%s", dirname, mydirent->d_name);
        display_file(tmp_buf, mydirent->d_name, buf);
    }

    closedir(dir);
}

#include "server.h"

void display_file(const char* path, const char* filename, char buf[])
{
//    int i;
    struct stat stat_buf;
//    char filemode[11];
//    char tmp_buf[TMP_BUF_SIZE];

    if (stat(path, &stat_buf) < 0)
    {
        strcat(buf, "ls: ");
        strcat(buf, sys_errlist[errno]);
        strcat(buf, "\n");
        return;
    }

/**
    switch (stat_buf.st_mode & S_IFMT)
    {
    case S_IFREG:
        filemode[0] = '-'; break;
    case S_IFBLK:
        filemode[0] = 'b'; break;
    case S_IFLNK:
        filemode[0] = 'l'; break;
    case S_IFSOCK:
        filemode[0] = 's'; break;
    case S_IFDIR:
        filemode[0] = 'd'; break;
    case S_IFCHR:
        filemode[0] = 'c'; break;
    case S_IFIFO:
        filemode[0] = 'p'; break;
    }

    for (i = 8; i >= 0; i--)
    {
        if ((stat_buf.st_mode) >> i & 1)
        {
            switch (i % 3)
            {
            case 2:
                filemode[9-i] = 'r'; break;
            case 1:
                filemode[9-i] = 'w'; break;
            case 0:
                filemode[9-i] = 'x'; break;
            }
        }
        else
            filemode[9-i] = '-';
    }
    filemode[10] = '\0';

    sprintf(tmp_buf, "%s\t%s\t%s\t%8ld\t%s",
                    filemode,
                    getpwuid(stat_buf.st_uid)->pw_name,
                    getgrgid(stat_buf.st_gid)->gr_name,
                    stat_buf.st_size, filename);
    strcat(buf, tmp_buf);

    if (filemode[0] == 'd')
        strcat(buf, "/");
    strcat(buf, "\n");
    **/

    strcat(buf, filename);
    if (S_ISDIR(stat_buf.st_mode))
        strcat(buf, "/");
    strcat(buf, "\n");

}

void display_dir(const char* dirname, char buf[])
{
    DIR* dir;
    struct dirent* mydirent;
    char tmp_buf[TMP_BUF_SIZE];

    if ((dir = opendir(dirname)) == NULL)
    {
        strcat(buf, "ls: ");
        strcat(buf, sys_errlist[errno]);
        strcat(buf, "\n");
        return;
    }

    while ((mydirent = readdir((dir))) != NULL)
    {
        if (strcmp(mydirent->d_name, ".") == 0 || strcmp(mydirent->d_name, "..") == 0)
            continue;

        sprintf(tmp_buf, "%s/%s", dirname, mydirent->d_name);
        display_file(tmp_buf, mydirent->d_name, buf);
    }

    closedir(dir);
}

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

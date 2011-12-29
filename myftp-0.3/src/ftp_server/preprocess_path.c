#include "server.h"

/** convert the virtual path to actual path on server **/
void preprocess_path(const char* path, char current_dir[], char processed_path[])
{
    char virtual_path[BUF_SIZE];
    int i, t;

    if (path != NULL && path[0] == '/')
    {
        sprintf(virtual_path, "%s", path);
    }
    else
    {
        sprintf(virtual_path, "%s/%s", (strcmp(current_dir, "/") == 0) ? "" : current_dir, path);
    }

    /** handle '.' and '..' in the path string **/
    for (i = 1; virtual_path[i] != '\0'; i++)
    {
        if (virtual_path[i] == '.')
        {
            if (virtual_path[i-1] == '/' && virtual_path[i+1] == '.'
                    && (virtual_path[i+2] == '/' || virtual_path[i+2] == '\0'))
            {
                t = i + 2;
                if (i > 1)
                {
                    for (i -= 2; virtual_path[i] != '/'; i--);
                    virtual_path[i] = '\0';
                }
                else
                    virtual_path[0] = '\0';

                if (virtual_path[t] == '/' && virtual_path[t+1] != '\0')
                    strcat(virtual_path, &virtual_path[t]);
                else
                    i--;
            }
            else if (virtual_path[i-1] == '/'
                     && (virtual_path[i+1] == '/' || virtual_path[i+1] == '\0'))
            {
                t = i + 1;
                virtual_path[--i] = '\0';
                if (virtual_path[t] == '/' && virtual_path[t+1] != '\0')
                    strcat(virtual_path, &virtual_path[t]);
                else
                    i--;
            }
        }
    }

    if (virtual_path[0] == '\0')
		strcpy(virtual_path, "/");
	else /** remove the tail '/' character **/
	{
		t = strlen(virtual_path);
		if (t > 1 && virtual_path[t-1] == '/')
			virtual_path[t-1] = '\0';
	}

    sprintf(processed_path, "%s%s", FTP_ROOT_DIR, virtual_path);
}

#include "server.h"

/** check login username **/
int check_login(char* username, char* password)
{
    int i;
    char tmp_passwd[MAX_LENGTH];
    memset(tmp_passwd, 0, MAX_LENGTH);
    if (strcmp(username, "anonymous") == 0)
        return 0;

    for (i = 1; i < MAX_USER_COUNT; i++)
    {
        if (strcmp(username, user_list[i].name) == 0)
        {
            crypt(password, tmp_passwd);
            if (strcmp(tmp_passwd, user_list[i].passwd) == 0)
                return i;
        }
    }
    return -1;
}

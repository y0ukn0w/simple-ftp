#include "server.h"

/** here is the valid user list, currently containing anonymous, admin and test **/
struct user_info user_list[MAX_USER_COUNT];

void init_user_list()
{
    int i;
    for (i = 0; i < MAX_USER_COUNT; i++)
        user_list[i].active = 0;

    strcpy(user_list[0].name, "anonymous");

    strcpy(user_list[1].name, "admin");
    crypt("admin", user_list[0].passwd);

    strcpy(user_list[2].name, "test");
    crypt("test", user_list[1].passwd);

}

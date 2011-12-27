#include "server.h"

void do_pwd(char current_dir[], char buf[])
{
    sprintf(buf, "%s\n", current_dir);
}

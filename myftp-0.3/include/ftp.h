#ifndef FTP_H_INCLUDED
#define FTP_H_INCLUDED

#include "file.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

enum CMD_TYPE { LOGIN, PASV,
                LS, CD, PWD, RM, MKDIR, RMDIR, /** remote command **/
                PUT, GET, /** file operation **/
                QUIT }; /** quit from serve **/

struct cmd_type_header
{
    enum CMD_TYPE cmd_type;
    int cmd_argc;
    char cmd_argv[];
};

enum CMD_RETURN_STATUS { SUCCESS, FAIL };

struct cmd_result_header
{
    enum CMD_RETURN_STATUS ret_status;
    char ret_result[];
};

#endif // FTP_H_INCLUDED

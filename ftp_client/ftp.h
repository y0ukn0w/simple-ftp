#ifndef FTP_H_INCLUDED
#define FTP_H_INCLUDED

enum CMD_TYPE { LOGIN, LS, CD, PWD, RM, MKDIR, RMDIR, PASV, PUT, MPUT, GET, MGET, QUIT, NOT_SUPPORTED, DUMMY};

struct cmd_type_header
{
    enum CMD_TYPE cmd_type;
    int cmd_argc;
    char cmd_argv[];
};

enum CMD_RETURN_STATUS { SUCCESS, FAIL};

struct cmd_result_header
{
    enum CMD_RETURN_STATUS ret_status;
    char ret_result[];
};

struct file_header
{
    int trans_size;
//    int is_end;
    char content[];
};

void send_file(const char* file, int client_file_sockfd, char file_buf[]);
void recv_file(const char* file, int file_sockfd, char file_buf[]);

#endif // FTP_H_INCLUDED

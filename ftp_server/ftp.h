#ifndef FTP_H_INCLUDED
#define FTP_H_INCLUDED

enum CMD_TYPE { LOGIN, PASV,
                LS, CD, PWD, RM, MKDIR, RMDIR,
                PUT, MPUT, GET, MGET, /** file operation **/
                HELP, QUIT, NOT_SUPPORTED, DUMMY,
                READY, NOT_READY }; /** for file transfer **/

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

struct file_header
{
    int trans_size;
    char content[];
};

void send_file(const char* file, int client_file_sockfd, char file_buf[]);
void recv_file(const char* file, int file_sockfd, char file_buf[]);

int check_file_for_recv(const char* filename, char buf[]);
int check_file_for_send(const char* filename, char buf[]);


#endif // FTP_H_INCLUDED

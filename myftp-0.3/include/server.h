#ifndef SERVER_CMD_H_INCLUDED
#define SERVER_CMD_H_INCLUDED

#include "ftp.h"
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_LENGTH 20
struct user_info
{
    char name[MAX_LENGTH];
    char passwd[MAX_LENGTH];
    int active;
};

#define MAX_USER_COUNT 5
extern struct user_info user_list[];

extern int active_count;
extern int visit_count;

extern char FTP_ROOT_DIR[];

struct server_thread_argv
{
    int ftp_fd;
    int data_port;
    int data_sockfd;
};

struct client_thread_argv
{
    int client_sockfd;
    int data_port;
    int data_sockfd;
};

int ftp_create_socket(int port);

void* server_thread(void* argv);
void* client_thread(void* argv);
void init_user_list();
void do_server_cmd();

int check_login(char* username, char* password);
int do_login(char recv_buf[], char send_buf[], int client_sockfd);
int do_pasv(char send_buf[], int client_sockfd, int data_port, int data_sockfd);
void do_quit(char send_buf[], int client_sockfd, int file_sockfd, int user_id);

int exec_cmd(char recv_buf[], char send_buf[], char file_buf[],
                char current_dir[], char processed_path[],
                int client_sockfd, int file_sockfd);

void preprocess_path(const char* path, char current_dir[], char processed_path[]);

void do_ls(const char* filename, char buf[], char current_dir[], char processed_path[]);
void do_cd(const char* path, char buf[], char current_dir[], char processed_path[]);
void do_pwd(char current_dir[], char buf[]);
void do_rm(const char* path, char buf[], char current_dir[], char processed_path[]);
void do_mkdir(const char* path, char buf[], char current_dir[], char processed_path[]);
void do_rmdir(const char* path, char buf[], char current_dir[], char processed_path[]);

void do_get(char recv_buf[], char send_buf[], char file_buf[],
            char current_dir[], char processed_path[],
            int client_sockfd, int file_sockfd);

void do_put(char recv_buf[], char send_buf[], char file_buf[],
            char current_dir[], char processed_path[],
            int client_sockfd, int file_sockfd);

#endif // SERVER_CMD_H_INCLUDED

#ifndef SERVER_CMD_H_INCLUDED
#define SERVER_CMD_H_INCLUDED

#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "ftp.h"

#define BUF_SIZE 1024
#define TMP_BUF_SIZE 100
#define FILE_BUF_SIZE 1024

extern char* FTP_ROOT_DIR;

void exec_cmd(char cmd_buf[], char send_buf[],
              char file_buf[], int file_sockfd,
              char current_dir[], char processed_path[]);
void preprocess_path(const char* path, char current_dir[], char processed_path[]);
void do_cd(const char* path, char buf[], char current_dir[], char processed_path[]);
void do_ls(const char* filename, char buf[], char current_dir[], char processed_path[]);
void do_mkdir(const char* path, char buf[], char current_dir[], char processed_path[]);
void do_pwd(char current_dir[], char buf[]);
void do_rm(const char* path, char buf[], char current_dir[], char processed_path[]);
void do_rmdir(const char* path, char buf[], char current_dir[], char processed_path[]);
int check_login(char* username, char* password);
void do_quit(int file_sockfd);

void do_get(const char* filename, char send_buf[], char current_dir[], char processed_path[]);
void do_put(const char* filename, char send_buf[], char current_dir[], char processed_path[]);

int ftp_create_socket(int port);
void* client_thread(void* sockfd);

 struct client_thread_argv
{
    int client_sockfd;
    int data_port;
    int file_sockfd;
};

#endif // SERVER_CMD_H_INCLUDED

#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "ftp.h"
#include <arpa/inet.h>

int ftp_connect_socket(char *ip, int port);

int do_login_cli(char recv_buf[], char send_buf[], int server_sockfd, char* username, char* password);
int do_pasv_cli(char recv_buf[], char send_buf[], int server_sockfd, char* ftp_server_ip);

int exec_cmd_cli(char input_buf[], char recv_buf[], char send_buf[], char file_buf[],
                 int server_sockfd, int file_sockfd);

void do_put_cli(char recv_buf[], char send_buf[], char file_buf[],
                int server_sockfd, int file_sockfd);

void do_get_cli(char recv_buf[], char send_buf[], char file_buf[],
                int server_sockfd, int file_sockfd);

void do_quit_cli(char recv_buf[], char send_buf[], int server_sockfd, int file_sockfd);

void do_mget_cli(char recv_buf[], char send_buf[], char file_buf[],
                int server_sockfd, int file_sockfd);

void do_mput_cli(char recv_buf[], char send_buf[], char file_buf[],
                int server_sockfd, int file_sockfd);

#endif // CLIENT_H_INCLUDED

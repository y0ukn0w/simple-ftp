#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "ftp.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
//#include <fcntl.h>

#define TRUE 1
#define FALSE 0

#define BUF_SIZE 1024
#define TMP_BUF_SIZE 100

void cmd_analysize(char str_buf[], char cmd_buf[]);
int ftp_connect_socket(char *ip, int port);
int pasv_cmd(char recv_buf[], char send_buf[], int server_sockfd, char* ftp_server_ip);

#endif // CLIENT_H_INCLUDED

#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define BUF_SIZE 1024
#define FILE_BUF_SIZE 1024

void send_file(const char* file, int client_file_sockfd, char file_buf[]);
void recv_file(const char* file, int file_sockfd, char file_buf[]);

int check_file_for_recv(const char* filename, char buf[]);
int check_file_for_send(const char* filename, char buf[]);

void display_file(const char* path, const char* filename, char buf[]);
void display_dir(const char* dirname, char buf[]);

#endif // FILE_H_INCLUDED

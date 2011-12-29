AM_CPPFLAGS = -I$(top_srcdir)/include
bin_PROGRAMS = ftpserver
ftpserver_SOURCES = \
check_login.c \
client_thread.c \
do_cd.c \
do_get.c \
do_login.c \
do_ls.c \
do_mkdir.c \
do_pasv.c \
do_put.c \
do_pwd.c \
do_quit.c \
do_rm.c \
do_rmdir.c \
do_server_cmd.c \
exec_cmd.c \
create_socket.c \
ftpserver.c \
preprocess_path.c \
server_thread.c \
user_list.c
ftpserver_LDADD = -lpthread -lcrypt \
				../common/libfile.a


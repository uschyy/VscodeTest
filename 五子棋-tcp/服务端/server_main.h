#ifndef __SERVER_MAIN_H__
#define __SERVER_MAIN_H__
#include "macro.h"


void *connect_handler_read(void *arg);

// void *connect_handler_write(void*arg);
// void *connect_handler(void *arg);//----这个函数专门与客户端对接，负责与客户端的所有通信

int tcp_server_init(char *ip,short port);

int tcp_server_uninit(int sockfd);

void*connect_listen_botton(void*arg);

void*connect_yes_start_again(void *arg);
void * connect_agree_take_back(void *arg);
#endif
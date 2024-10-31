#ifndef __CLIENT_MAIN_H__
#define __CLIENT_MAIN_H__

int tcp_client_init(char **argv,short port);
int close_client_init(int sockfd);
void *recv_data(void * arg);
void *client_listen_botton(void *arg);
void*connect_yes_start_again(void *arg);
void * connect_agree_take_back(void *arg);
#endif
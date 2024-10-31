#ifndef  __INPUT_H__
#define  __INPUT_H__
#include "macro.h"


void get_input(int fd, int *x0, int *y0, int *x1, int *y1,int *starttime);

int process_input(void);
int process_input1(void);
int process_input_yse_or_no(void);
int process_input_move_first_or_scond(void);//先动还是后动

void send_give_up(int sockfd);

void send_start_again(int sockfd);

void send_take_back(int sockfd);
void receive_take_back_aggre(int take_back_step);


void send_exit(int sockfd);

int process_click_down(void *arg);

void receive_give_up();
void receive_start_again();

void click_give_up();
void click_start_again();

// void save_array(int Take_back_array[15][15][15],int array[15][15]);
// void read_take_array(int array[15][15],int Take_back_array[15][15][15]);
void read_take_array(int s);
void save_array();
void printf_save_Array();
#endif 

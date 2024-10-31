#ifndef  __INPUT_H__
#define  __INPUT_H__
#include "macro.h"

int process_input(void);
int process_input1(void);
int process_input_yse_or_no(void);
int process_input_move_first_or_scond(void);

void send_give_up(int sockfd);
void receive_give_up();

void send_start_again(int sockfd);
void receive_start_again();

void send_take_back(int sockfd);
void receive_take_back_aggre(int take_back_step);

void send_exit(int sockfd);

int process_click_down(int sockfd);


void click_give_up();
void click_start_again();


void read_take_array(int s);
void save_array();
void printf_save_Array();
#endif 

#ifndef __START_H__
#define __START_H__

// 表示胜利与失败
#define victory 66
#define defented  99
#define continue_game 100

// 棋盘是否满
#define Qipan_full      1
#define Qipan_unfull    0

// 棋盘初始化成功
#define Qipan_init_success 1


#define SIZE 15    
  
#define W_max  0

// 黑白方的棋子
#define BLACK -1
#define WHILE 1

// 表示是否访问过当前位置上
#define visited   1
#define unvisited 0

// 投降
#define Give_up 10
// 重来
#define start_again 11
// 悔棋
#define take_back 12
// 退出
#define EXIT 13


// 打印按钮图片
void Fixed_picture_printing();

// 执行五子棋游戏的功能
void game_init_with_start(char **argv);

void move_first_or_senond(int select_flag);


#endif
#ifndef __MACRO_H__
#define __MACRO_H__

/*
    macro.h文件是所有需要用到的宏
*/

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

// 表示下棋了
#define click_down 14


// 结束
#define FINISH 0

// 能动与否
#define can_move 1
#define can_not_move 0

// 是否愿意重来
#define agree 1
#define unagree -1

#define down_again 1

#define YES 1
#define NO  0


// 按钮状态
#define botton_click    1
#define botton_unclick -1
#define botton_init     0

#define first_move 123
#define second_move 321

#define had_choose 100
#define no_choose  101
#endif

#ifndef __APP_H__
#define __APP_H__

#include "macro.h"

//  全局棋盘
int Array[SIZE][SIZE];

// 全局棋盘的对应位置是否被点击过
int Array_flag[SIZE][SIZE];

// ---------------------------这里是五子需要用到的函数功能--------------------

// 1.棋盘初始化
int Qipan_init();
// 2.打印棋盘
void printf_Qipan();
// 3.设置胜利
int judge_Qipan_full();
int checkWin(int player);

// 4.运动逻辑
void black_move(int i,int j);
void while_move(int i,int j);

// 5.界面打印到开发板---用%表示当前位置
void printf_photo_to_screen();
#endif
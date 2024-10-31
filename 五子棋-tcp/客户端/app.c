#include <stdio.h>
#include "app.h"
#include "lcd.h"


// 1.棋盘初始化
int Qipan_init()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            Array[i][j]=W_max;
            Array_flag[i][j]=unvisited;
        }        
    }
    return Qipan_init_success;
}
// 2.打印棋盘
void printf_Qipan()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
           printf("%d ",Array[i][j]);
        }        
        printf("\n");
    }
}
// 3.设置棋局会出现的情况
// 3.1棋盘满了--表示平局
int judge_Qipan_full()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
           if (Array[i][j]==W_max)
           {
                return Qipan_unfull;
           }
        }        
    }
    return Qipan_full;
}
// 3.2判断胜利


// 检查指定玩家是否获胜--BLACK方-1 还是WHILE方1
int checkWin(int player) 
{
    // 检查行、列和对角线
    for (int i = 0; i < SIZE; i++) 
    {
        for (int j = 0; j < SIZE - 4; j++) 
        {
            // 检查行
            if (Array[i][j] == player && Array[i][j+1] == player &&
                Array[i][j+2] == player && Array[i][j+3] == player &&
                Array[i][j+4] == player) {
                return victory;
            }
            // 检查列
            if (Array[j][i] == player && Array[j+1][i] == player &&
                Array[j+2][i] == player && Array[j+3][i] == player &&
                Array[j+4][i] == player) {
                return victory;
            }
        }
    }
    // 检查主对角线
    for (int i = 0; i < SIZE - 4; i++) 
    {
        for (int j = 0; j < SIZE - 4; j++) 
        {
            if (Array[i][j] == player && Array[i+1][j+1] == player &&
                Array[i+2][j+2] == player && Array[i+3][j+3] == player &&
                Array[i+4][j+4] == player) {
                return victory;
            }
        }
    }
    // 检查副对角线
    for (int i = 4; i < SIZE-4; i++) 
    {
        for (int j = 0; j < SIZE - 4; j++) 
        {
            if (Array[i][j] == player && Array[i-1][j+1] == player &&
                Array[i-2][j+2] == player && Array[i-3][j+3] == player &&
                Array[i-4][j+4] == player) {
                return victory;
            }
        }
    }

    
    return continue_game;
}




// 4.运动逻辑
void black_move(int i,int j)//黑方行动
{
    Array[i][j]=BLACK;
    Array_flag[i][j]=visited;//当前位置已经访问过
}

void while_move(int i,int j)//白方行动,i,j是你点击的位置%得到的
{
    Array[i][j]=WHILE;
    Array_flag[i][j]=visited;//当前位置已经访问过
}

// 5.界面打印到开发板---用%表示当前位置
// 5.界面打印到开发板---用%表示当前位置
// void printf_photo_to_screen()
// {
//     for (int i = 0; i < SIZE; i++)
//     {
//         for (int j = 0; j < SIZE; j++)
//         {
//             if (Array[i][j]==WHILE)//白
//             {
//                 lcd_draw_bmp("/root/five_qi/use_photo/while.bmp",i*32,j*32);
//             }
//             else if (Array[i][j]==BLACK)//黑
//             {
//                 lcd_draw_bmp("/root/five_qi/use_photo/black.bmp",i*32,j*32);
//             }
//             else
//             {
//                 lcd_draw_bmp("/root/five_qi/use_photo/qipan.bmp",i*32-16,j*32-16);
//             }
//         }
    
//     }
//     Fixed_botton_printing();
// }


void printf_photo_to_screen()
{
    for (int i = 1; i < SIZE; i++)
    {
        for (int j = 1; j < SIZE; j++)
        {

            lcd_draw_bmp("/root/five_qi/use_photo/qipan.bmp",i*32-16,j*32-16);
        
        }   
    }
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (Array[i][j]==WHILE)//白
            {
                lcd_draw_bmp("/root/five_qi/use_photo/while.bmp",i*32,j*32);
            }
            else if (Array[i][j]==BLACK)//黑
            {
                lcd_draw_bmp("/root/five_qi/use_photo/black.bmp",i*32,j*32);
            }
        }   
    }
    Fixed_botton_printing();
}
